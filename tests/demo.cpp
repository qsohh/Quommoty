#include <vector>

#include <ql/quantlib.hpp>

#include "utils/print_utils.h"
#include "CrudeFuturesPricer.h"
#include "ElectricityFuturesPricer.h"
#include "demo.h"

namespace demo {
	void test_electricity() {
		Pricer::ElectricityFuturesPricer EFP;
		QuantLib::Date d1(1, QuantLib::September, 2025);
		QuantLib::Date d2(1, QuantLib::October, 2025);
		QuantLib::Date d3(1, QuantLib::November, 2025);
		std::vector<QuantLib::Date> ForwardDates = { d1, d2, d3 };
		std::vector<double> ForwardPrices = { 90, 85, 100 };
		EFP.setForwardAnchors(ForwardDates, ForwardPrices);

		EFP.setEvaluationDate(QuantLib::Date(1, QuantLib::July, 2025));
		EFP.setDeliveryDate(QuantLib::Date(17, QuantLib::October, 2025));

		utils::print("Electricity Future price evaluated at", EFP.getEvaluationDate(), "for", EFP.getDeliveryDate(), "is:", EFP.price());
	}

	void test_crude_oil() {
		Pricer::CrudeFuturesPricer CFP;
		QuantLib::Date d1(1, QuantLib::September, 2025);
		QuantLib::Date d2(1, QuantLib::October, 2025);
		QuantLib::Date d3(1, QuantLib::November, 2025);
		std::vector<QuantLib::Date> ZRdates = { d1, d2, d3 };
		std::vector<double> ZRates = { 0.021, 0.020, 0.019 };
		QuantLib::Date d4(12, QuantLib::August, 2025);
		QuantLib::Date d5(1, QuantLib::January, 2026);
		std::vector<QuantLib::Date> Ydates = { d4, d5 };
		std::vector<double> Yields = { 0.005, 0.007 };

		CFP.setSpot(90.0);
		CFP.setEvaluationDate(QuantLib::Date(1, QuantLib::July, 2025));
		CFP.setDeliveryDate(QuantLib::Date(17, QuantLib::October, 2025));
		CFP.setZeroRateFromVector(ZRdates, ZRates);
		CFP.setCostOfCarryFromFlat(0.003);
		CFP.setConvenienceYieldFromVector(Ydates, Yields);

		utils::print("Crude Oil spot price:", CFP.getSpot());
		utils::print("Future price evaluated at", CFP.getEvaluationDate(), "for", CFP.getDeliveryDate(), "is:", CFP.price());
	}
} // namespace demo