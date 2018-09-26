#pragma once

#include <string>
#include <istream>
#include <type_traits>

#include "util/typeview.hpp"
#include "scanner.hpp"
#include "generated/parser.tab.hh"

using std::string;
using std::cout;
using std::endl;
using std::ostream;
using std::istream;
using Origin::Parser;
using Origin::Scanner;

namespace Origin
{
	class Driver
	{
	private:
		Parser* parser = nullptr;
		Scanner* scanner = nullptr;
		void parse_helper(istream& stream);
		
	public:
		Driver() = default;
		virtual ~Driver();
		void parse(string& filename);
		void parse(istream& iss);
		ostream& print(ostream& stream);
		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		void add_number(T num)
		{
			cout << "\t{decltype=" << type_name<decltype(num)>() << ", val=" << num << "}\n";
		}
	};
}

