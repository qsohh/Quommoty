#include <iostream>
#include <string>

#include <ql/quantlib.hpp>

#include "utils/print_utils.h"
#include "Pricer.h"
#include "ElectricityFuturesPricer.h"

#include "demo.h"

int main() {
	demo::test_electricity();
	demo::test_crude_oil();

	return 0;
}