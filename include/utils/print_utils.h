#pragma once

#include <iostream>
#include <typeinfo>

namespace utils {
	template<typename... Args>
	void print(Args&&... args) { ((std::cout << args << ' '), ...) << '\n'; }

	template<typename T>
	std::string type() {
		using TR = typename std::remove_reference<T>::type;
		std::string name = typeid(TR).name();
		if (std::is_const<TR>::value) { name += " const"; }
		if (std::is_volatile<TR>::value) { name += " volatile"; }
		if (std::is_lvalue_reference<T>::value) { name += "&"; }
		else if (std::is_rvalue_reference<T>::value) { name += "&&"; }
		return name;
	}

} // namespace utils