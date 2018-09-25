#include <iostream>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <vector>

#include <boost/program_options.hpp>

#include "compilerargs.hpp"
#include "driver.hpp"


namespace po = boost::program_options;
using std::vector;
using std::cout;
using std::exception;
using std::cerr;
using std::endl;

int main(const int argc, const char** argv)
{
	try {
		CompilerArgs args = CompilerArgs(argc, argv);
		
		if (args.contains("help")) {
			args.help();
			return EXIT_SUCCESS;
		}
		
		if (args.contains("files")) {
			Origin::Driver driver;
			vector<string> input_files = args.input_files();
			for (string input_file : input_files) {
				cout << "File: " << input_file << endl;
				driver.parse(input_file);
				driver.print(cout) << endl;
			}
		}
	} catch (exception& e) {
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
