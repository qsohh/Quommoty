#pragma once

#include "Pricer.h"

namespace Pricer {
	class ElectricityFuturesPricer : public Pricer {
	public:
		ElectricityFuturesPricer();
		~ElectricityFuturesPricer();

		double price() const override;
		void test_api() override;
	}; // class ElectricityFuturesPricer
} //namespace Pricer
