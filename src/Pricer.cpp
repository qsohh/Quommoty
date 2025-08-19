#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <ql/quantlib.hpp>

#include "utils/print_utils.h"
#include "Pricer.h"

namespace Pricer {
	Pricer::Pricer() {}
	Pricer::~Pricer() {}

	void Pricer::setEvaluationDate(const QuantLib::Date& d) {
		this->evalDate_ = d;
		QuantLib::Settings::instance().evaluationDate() = d;
	}

	QuantLib::Date Pricer::getEvaluationDate() const {
		QL_REQUIRE(this->evalDate_ != QuantLib::Date(), "Evaluation date is not yet set.");
		return this->evalDate_;
	}

	void Pricer::setDeliveryDate(const QuantLib::Date& d) {
		this->delivery_ = d;
	}

	QuantLib::Date Pricer::getDeliveryDate() const {
		QL_REQUIRE(this->delivery_ != QuantLib::Date(), "Delivery date is not yet set.");
		return this->delivery_;
	}

	void Pricer::setPricingMode(PricingMode m) {
		this->pricing_mode_ = m;
	}

	PricingMode Pricer::getPricingMode() const {
		return this->pricing_mode_;
	}

	void Pricer::setAveragingMode(AveragingMode a) {
		this->averaging_mode_ = a;
	}

	AveragingMode Pricer::getAveragingMode() const {
		return this->averaging_mode_;
	}

	void Pricer::setSpot(double s) {
		this->spot_ = s;
	}

	void Pricer::setCurveFromFlat(double r = 0.0, const std::string& var_name = "ZeroRate") {
		// [auto] [boost::shared_ptr<QuantLib::FlatForward>]
		auto ts = QuantLib::ext::make_shared<QuantLib::FlatForward>(
			this->evalDate_, r, this->dc_, QuantLib::Continuous, QuantLib::Annual
		);
		if (var_name == "ZeroRate") {
			this->rTS_ = QuantLib::Handle<QuantLib::YieldTermStructure>(ts);
		}
		else if (var_name == "CostOfCarry") {
			this->cTS_ = QuantLib::Handle<QuantLib::YieldTermStructure>(ts);
		}
		else if (var_name == "ConvenienceYield") {
			this->yTS_ = QuantLib::Handle<QuantLib::YieldTermStructure>(ts);
		}
		else { QL_FAIL("Wrong variable name. Should be ZeroRate or CostOfCarry or ConvenienceYield."); }
		this->pricing_mode_ = PricingMode::CostOfCarry;
	}

	void Pricer::setCurveFromVector(
		const std::vector<QuantLib::Date>& dates,
		const std::vector<double>& rates,
		const std::string& var_name = "ZeroRate") {
		QL_REQUIRE(dates.size() == rates.size(), "dates/rates size mismatch.");
		QL_REQUIRE(dates.size() >= 2, "Need at least 2 values.");
		QL_REQUIRE(std::is_sorted(dates.begin(), dates.end()), "dates not strictly increasing.");
		// [auto] [boost::shared_ptr<QuantLib::ZeroCurve>]
		auto ts = QuantLib::ext::make_shared<QuantLib::ZeroCurve>(
			dates, rates, this->dc_);
		ts->enableExtrapolation(true);
		if (var_name == "ZeroRate") {
			this->rTS_ = QuantLib::Handle<QuantLib::YieldTermStructure>(ts);
		}
		else if (var_name == "CostOfCarry") {
			this->cTS_ = QuantLib::Handle<QuantLib::YieldTermStructure>(ts);
		}
		else if (var_name == "ConvenienceYield") {
			this->yTS_ = QuantLib::Handle<QuantLib::YieldTermStructure>(ts);
		}
		this->pricing_mode_ = PricingMode::CostOfCarry;
	}

	void Pricer::setZeroRateFromFlat(double r) {
		this->setCurveFromFlat(r, "ZeroRate");
	}

	void Pricer::setZeroRateFromVector(
		const std::vector<QuantLib::Date>& dates,
		const std::vector<double>& rates) {
		this->setCurveFromVector(dates, rates, "ZeroRate");
	}

	void Pricer::setCostOfCarryFromFlat(double r) {
		this->setCurveFromFlat(r, "CostOfCarry");
	}

	void Pricer::setCostOfCarryFromVector(
		const std::vector<QuantLib::Date>& dates,
		const std::vector<double>& rates) {
		this->setCurveFromVector(dates, rates, "CostOfCarry");
	}

	void Pricer::setConvenienceYieldFromFlat(double r) {
		this->setCurveFromFlat(r, "ConvenienceYield");
	}

	void Pricer::setConvenienceYieldFromVector(
		const std::vector<QuantLib::Date>& dates,
		const std::vector<double>& rates) {
		this->setCurveFromVector(dates, rates, "ConvenienceYield");
	}

	void Pricer::setForwardAnchors(
		const std::vector<QuantLib::Date>& dates,
		const std::vector<double>& prices) {
		QL_REQUIRE(dates.size() == prices.size(), "dates/rates size mismatch.");
		QL_REQUIRE(dates.size() >= 2, "Need at least 2 values.");
		QL_REQUIRE(std::is_sorted(dates.begin(), dates.end()), "dates not strictly increasing.");
		this->forward_dates_ = dates;
		this->forward_prices_ = prices;
		this->setPricingMode(PricingMode::ExternalForwardAnchor);
	}

	void Pricer::clearForwardAnchors() {
		this->forward_dates_.clear();
		this->forward_prices_.clear();
	}

	double Pricer::getSpot() const {
		return this->spot_;
	}

	double Pricer::price() const {
		QL_REQUIRE(this->delivery_ != QuantLib::Date(), "Delivery date not set.");
		// QuantLib::Time tau = this->tau();

		switch (this->pricing_mode_) {
		case PricingMode::CostOfCarry: {
			QL_REQUIRE(this->spot_ > 0, "Spot should set to positive.");
			this->ensureCarryDefaults_();
			// Check the status of other two rates?
			return this->spot_ / this->getDiscountFactor_();
		} // case PricingMode::CostOfCarry
		case PricingMode::ExternalForwardAnchor: {
			switch (this->averaging_mode_) {
			case AveragingMode::NoAverage: {
				return this->forwardPriceOn_(this->delivery_);
			}
			case AveragingMode::CalendarDay: {
				return this->priceDeliveryAveragingCalendar_();
			}
			case AveragingMode::BusinessDay: {
				return this->priceDeliveryAveragingBusinessDay_();
			}
			default:
				QL_FAIL("Unkown Averaging Mode.");
			} // switch (this->averaging_mode_)
		} // case PricingMode::ExternalForwardAnchor
		default:
			QL_FAIL("Wrong Pricing Mode.");
		} // switch (this->pricing_mode_)
	}

	QuantLib::Time Pricer::tau_() const {
		return this->dc_.yearFraction(evalDate_, delivery_);
	}

	double Pricer::getDiscountFactorOn_(const QuantLib::Date& d) const {
		return this->rTS_->discount(d) * this->cTS_->discount(d) / this->yTS_->discount(d);
	}

	double Pricer::getDiscountFactor_() const {
		return this->getDiscountFactorOn_(this->delivery_);
	}

	void Pricer::ensureCarryDefaults_() const {
		// QL_REQUIRE(!this->cTS_.empty(), "Cost of Carry empty.");
		// QL_REQUIRE(!this->yTS_.empty(), "Convenience yield empty.");
		this->requireCoverage_(rTS_);
		this->requireCoverage_(cTS_);
		this->requireCoverage_(yTS_);
	}

	void Pricer::requireCoverage_(const QuantLib::Handle<QuantLib::YieldTermStructure>& h) const {
		QL_REQUIRE(!h.empty(), "Curve handle is empty.");
		QL_REQUIRE(this->delivery_ != QuantLib::Date(), "Delivery date not set.");
		const auto& curve_link = h.currentLink().get();
		QL_REQUIRE(this->delivery_ >= curve_link->referenceDate(), "Delivery date is before the curve's reference date.");
		QL_REQUIRE(curve_link->allowsExtrapolation() || this->delivery_ <= curve_link->maxDate(), "Delivery date beyond curve maxDate and extrapolation not allowed.");
	}

	double Pricer::forwardPriceOn_(const QuantLib::Date& d) const {
		// Linear interpolation for forward price
		QL_REQUIRE(!this->forward_dates_.empty(), "Should set forward anchors.");
		long int x = d.serialNumber();
		QL_REQUIRE((x >= this->forward_dates_.front().serialNumber()) && (x <= this->forward_dates_.back().serialNumber()), "Target date not in data range.");
		auto it = std::upper_bound(this->forward_dates_.begin(), this->forward_dates_.end(), d);
		size_t i = size_t(it - this->forward_dates_.begin() - 1);
		long int x1 = this->forward_dates_[i].serialNumber();
		long int x2 = this->forward_dates_[i + 1].serialNumber();
		double y1 = this->forward_prices_[i];
		double y2 = this->forward_prices_[i + 1];
		double w = double(x - x1) / double(x2 - x1);
		return y1 * (1.0 - w) + y2 * w;
	}

	double Pricer::priceDeliveryAveragingCalendar_() const {
		// Averaging forward prices of the Delivery Month
		QL_REQUIRE(!this->forward_dates_.empty(), "Should set forward anchors.");
		QuantLib::Date d1(1, this->delivery_.month(), this->delivery_.year());
		QuantLib::Date d2 = QuantLib::Date::endOfMonth(d1); // ! Different to cal_.endOfMonth(d1)
		QL_REQUIRE((d1 >= this->forward_dates_.front()) && (d2 <= this->forward_dates_.back()), "Delivery month not covered by data range.");
		std::vector<QuantLib::Date> days;
		days.push_back(d1);
		// Note: for (QuantLib::Date& d : this->forward_dates_), the complier troubles to define the type
		for (auto& d : this->forward_dates_) {
			if (d1 < d and d2 > d) { days.push_back(d); }
		}
		days.push_back(d2);
		// Averaging in the Delivery Month
		double num = 0.0;
		double den = this->dc_.yearFraction(d1, d2);
		for (size_t i = 0; (i + 1) < days.size(); i++) {
			QuantLib::Date da = days[i];
			QuantLib::Date db = days[i + 1];
			double Ya = this->forwardPriceOn_(da);
			double Yb = this->forwardPriceOn_(db);
			double Delta_t = this->dc_.yearFraction(da, db);
			num += 0.5 * (Ya + Yb) * Delta_t;
		}
		return num/den;
	}

	double Pricer::priceDeliveryAveragingBusinessDay_() const {
		QL_REQUIRE(!this->forward_dates_.empty(), "Should set forward anchors.");
		QuantLib::Date d1(1, this->delivery_.month(), this->delivery_.year());
		// If the first of the delivery month is not a business day
		if (!this->cal_.isBusinessDay(d1)) { d1 = this->cal_.adjust(d1); }
		QuantLib::Date d2 = this->cal_.endOfMonth(d1); // Last businiss day
		QL_REQUIRE((d1 >= this->forward_dates_.front()) && (d2 <= this->forward_dates_.back()), "Delivery month not covered by data range.");
		double num = 0.0;
		double den = 0.0;
		for (QuantLib::Date d = d1; d <= d2; d++) {
			if (this->cal_.isBusinessDay(d)) {
				num += this->forwardPriceOn_(d);
				den += 1.0;
			}
		}
		QL_REQUIRE(den > 0.0, "No Business day during the delivery period.");
		return num/den;
	}

	void Pricer::test_api() {
		try {
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "unknown error" << std::endl;
		}
	}

	QuantLib::Date string2date(std::string datestring) {
		// format of datestring: "YYYY-MM-DD", e.g., "2025-08-01"
		int y, m, d;
		char temp; // Skipping '-'
		std::istringstream iss(datestring);
		iss >> y >> temp >> m >> temp >> d;
		QuantLib::Month QM = static_cast<QuantLib::Month>(m);
		return QuantLib::Date(d, QM, y);
	}

} // namespace Pricer