#pragma once

#include <vector>
#include <string>
#include <boost/program_options.hpp>

using std::string;
using std::vector;

namespace po = boost::program_options;

class CompilerArgs
{
private:
	vector<string> input_filenames;
	po::options_description desc;
	po::variables_map vm;

public:
	CompilerArgs(const int argc, const char** argv);
	virtual ~CompilerArgs();
	vector<string>& input_files();
	bool contains(string arg_name);
	void help();
};
