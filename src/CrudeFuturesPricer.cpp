#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <ql/quantlib.hpp>

#include "utils/print_utils.h"
#include "CrudeFuturesPricer.h"
#include "Pricer.h"

namespace Pricer {
	CrudeFuturesPricer::CrudeFuturesPricer() {}
	CrudeFuturesPricer::~CrudeFuturesPricer() {}

	double CrudeFuturesPricer::price() const {
		QL_REQUIRE(getPricingMode() == PricingMode::CostOfCarry, "Crude Oil Futures Pricer only supports Cost of Carry Mode.");
		QL_REQUIRE(this->getAveragingMode() == AveragingMode::NoAverage, "Crude Oil Futures Pricer only supports NoAverage mode.");
		return Pricer::price();
	}

	void CrudeFuturesPricer::test_api() {

	}

} // namespace Pricer