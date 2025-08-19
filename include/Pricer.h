#pragma once

#include <string>
#include <vector>

#include <ql/time/date.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/handle.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>

namespace Pricer {
	/* CostOfCarry mode only available for storable commodities,
	*  ExternalForwardAnchor mode principally aims commodities like electricity.*/
	enum class PricingMode { CostOfCarry, ExternalForwardAnchor };

	/* Averaging pricing for delivery period. */
	enum class AveragingMode { NoAverage, CalendarDay, BusinessDay };

	QuantLib::Date string2date(std::string);

	class Pricer {
	public:
		Pricer();
		virtual ~Pricer();

		// Basic settings
		void setEvaluationDate(const QuantLib::Date&);
		QuantLib::Date getEvaluationDate() const;
		void setDeliveryDate(const QuantLib::Date&);
		QuantLib::Date getDeliveryDate() const;
		void setPricingMode(PricingMode);
		PricingMode getPricingMode() const;
		void setAveragingMode(AveragingMode);
		AveragingMode getAveragingMode() const;

		// Spot, rate and curve
		void setSpot(double);
		void setCurveFromFlat(double, const std::string&);
		void setCurveFromVector(
			const std::vector<QuantLib::Date>&,
			const std::vector<double>&,
			const std::string&);
		void setZeroRateFromFlat(double);
		void setZeroRateFromVector(
			const std::vector<QuantLib::Date>&,
			const std::vector<double>&);
		void setCostOfCarryFromFlat(double);
		void setCostOfCarryFromVector(
			const std::vector<QuantLib::Date>&,
			const std::vector<double>&);
		void setConvenienceYieldFromFlat(double);
		void setConvenienceYieldFromVector(
			const std::vector<QuantLib::Date>&,
			const std::vector<double>&);

		/* First construct the overall discount curve, 
		*  then calculate the DF. */
		// void constructDiscountCurve(bool);

		// Forward Price Anchor for non-storable commodities
		void setForwardAnchors(
			const std::vector<QuantLib::Date>&,
			const std::vector<double>&);
		void clearForwardAnchors();

		// Pricing and other APIs
		virtual double price() const;
		double getSpot() const;

		// Testing APIs
		virtual void test_api();

	private:
		// Private variables
		QuantLib::Calendar cal_{ QuantLib::TARGET() };
		QuantLib::DayCounter dc_{ QuantLib::Actual365Fixed() };
		QuantLib::Date evalDate_;
		QuantLib::Date delivery_;
		double spot_{ 0.0 };
		PricingMode pricing_mode_{ PricingMode::CostOfCarry };
		AveragingMode averaging_mode_{ AveragingMode::NoAverage };
		QuantLib::Handle<QuantLib::YieldTermStructure> rTS_; // zero rate
		QuantLib::Handle<QuantLib::YieldTermStructure> cTS_; // cost of carry
		QuantLib::Handle<QuantLib::YieldTermStructure> yTS_; // convenience yield
		// QuantLib::Handle<QuantLib::YieldTermStructure> discount_;
		std::vector<QuantLib::Date> forward_dates_;
		std::vector<double> forward_prices_;

	protected:
		// Protected APIs
		QuantLib::Time tau_() const;
		double getDiscountFactorOn_(const QuantLib::Date&) const;
		double getDiscountFactor_() const;
		void ensureCarryDefaults_() const;
		void requireCoverage_(const QuantLib::Handle<QuantLib::YieldTermStructure>&) const;
		// CHECK coverage of rTs, cTS and yTS
		double forwardPriceOn_(const QuantLib::Date&) const;
		double priceDeliveryAveragingCalendar_() const;
		double priceDeliveryAveragingBusinessDay_() const;
	}; // class Pricer
} // namespace Pricer