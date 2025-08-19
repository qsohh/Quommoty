#pragma once

#include "Pricer.h"

namespace Pricer {
	class CrudeFuturesPricer : public Pricer {
	public:
		CrudeFuturesPricer();
		~CrudeFuturesPricer();

		double price() const override;
		void test_api() override;
	}; // class CrudeFuturesPricer
} //namespace Pricer
