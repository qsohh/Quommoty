#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <ql/quantlib.hpp>

#include "utils/print_utils.h"
#include "ElectricityFuturesPricer.h"
#include "Pricer.h"

namespace Pricer {
	ElectricityFuturesPricer::ElectricityFuturesPricer() {
		setAveragingMode(AveragingMode::CalendarDay);
		setPricingMode(PricingMode::ExternalForwardAnchor);
	}
	ElectricityFuturesPricer::~ElectricityFuturesPricer() {}

	double ElectricityFuturesPricer::price() const {
		QL_REQUIRE(getPricingMode() == PricingMode::ExternalForwardAnchor, "Electricity Futures Pricer only supports Forward Anchor mode.");
		QL_REQUIRE(this->getAveragingMode() == AveragingMode::CalendarDay, "Electricity Futures Pricer only supports Calendar Averaging mode.");
		return Pricer::price();
	}

	void ElectricityFuturesPricer::test_api() {

	}

} // namespace Pricer