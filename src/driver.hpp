#pragma once

#include <string>
#include <cstddef>
#include <istream>

#include "scanner.hpp"
#include "generated/parser.tab.hh"

using std::size_t;
using std::string;
using std::ostream;
using std::istream;
using Origin::Parser;
using Origin::Scanner;

namespace Origin
{
	class Driver
	{
	private:
		size_t  chars = 0;
		size_t  words = 0;
		size_t  lines = 0;
		size_t  uppercase = 0;
		size_t  lowercase = 0;
		Parser* parser = nullptr;
		Scanner* scanner = nullptr;
		const string RED = "\033[1;31m";
		const string BLUE = "\033[1;36m";
		const string RESET = "\033[0m";
		void parse_helper(istream& stream);
		
	public:
		Driver() = default;
		virtual ~Driver();
		void parse(string& filename);
		void parse(istream& iss);
		void add_upper();
		void add_lower();
		void add_word(const string& word);
		void add_newline();
		void add_char();
		ostream& print(ostream& stream);
	};
}

