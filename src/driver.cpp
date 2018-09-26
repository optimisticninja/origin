#include <cctype>
#include <fstream>
#include <cassert>
#include <string>
#include <type_traits>
#include <cmath>

#include "driver.hpp"
#include "util/ansicolors.hpp"

using std::ifstream;
using std::bad_alloc;
using std::cout;
using std::cerr;
using std::endl;

Origin::Driver::~Driver()
{
	delete scanner;
	scanner = nullptr;
	delete parser;
	parser = nullptr;
}

void Origin::Driver::parse(string& fname)
{
	ifstream in_file(fname);

	if (!in_file.good()) {
		exit(EXIT_FAILURE);
	}

	parse_helper(in_file);
}

void Origin::Driver::parse(istream& stream)
{
	if (!stream.good() && stream.eof()) {
		return;
	}

	parse_helper(stream);
}


void Origin::Driver::parse_helper(istream& stream)
{
	delete this->scanner;

	try {
		this->scanner = new Origin::Scanner(&stream);
	} catch (bad_alloc& ba) {
		cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	delete this->parser;

	try {
		this->parser = new Origin::Parser((*this->scanner), (*this));
	} catch (bad_alloc& ba) {
		cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	if (this->parser->parse()) {
		cerr << "Parse failed!!\n";
	}
}

ostream& Origin::Driver::print(ostream& stream)
{
	stream << RED  << "Results: " << RESET << "\n";
	return stream;
}
