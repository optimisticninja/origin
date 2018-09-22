#include <cctype>
#include <fstream>
#include <cassert>

#include "driver.hpp"

using std::ifstream;
using std::bad_alloc;
using std::cerr;

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
	delete scanner;

	try {
		scanner = new Origin::Scanner(&stream);
	} catch (bad_alloc& ba) {
		cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	delete parser;

	try {
		parser = new Origin::Parser((*scanner), (*this));
	} catch (bad_alloc& ba) {
		cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	if (parser->parse()) {
		cerr << "Parse failed!!\n";
	}
}

void Origin::Driver::add_upper()
{
	uppercase++;
	chars++;
	words++;
}

void Origin::Driver::add_lower()
{
	lowercase++;
	chars++;
	words++;
}

void Origin::Driver::add_word(const string& word)
{
	words++;
	chars += word.length();

	for (const char& c : word) {
		if (islower(c)) {
			lowercase++;
		} else if (isupper(c)) {
			uppercase++;
		}
	}
}

void Origin::Driver::add_newline()
{
	lines++;
	chars++;
}

void Origin::Driver::add_char()
{
	chars++;
}

ostream& Origin::Driver::print(ostream& stream)
{
	stream << RED  << "Results: " << RESET << "\n";
	stream << BLUE << "Uppercase: " << RESET << uppercase << "\n";
	stream << BLUE << "Lowercase: " << RESET << lowercase << "\n";
	stream << BLUE << "Lines: " << RESET << lines << "\n";
	stream << BLUE << "Words: " << RESET << words << "\n";
	stream << BLUE << "Characters: " << RESET << chars << "\n";
	return stream;
}

