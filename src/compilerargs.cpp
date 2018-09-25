#include "compilerargs.hpp"

#include <iostream>

using std::cout;
using std::endl;

CompilerArgs::CompilerArgs(const int argc, const char** argv)
{
	this->desc.add_options()
		("help,h", "listing of compiler options")
		("files", po::value<vector<string>> (&input_filenames), "input files");
	po::options_description all_options;
	all_options.add(desc);
	po::positional_options_description pos;
	pos.add("files", -1);
	po::store(po::command_line_parser(argc, argv)
		.options(all_options)
		.positional(pos)
		.run(), 
		this->vm
	);
	po::notify(this->vm);
}

CompilerArgs::~CompilerArgs()
{
}

bool CompilerArgs::contains(string arg_name)
{
	return this->vm.count(arg_name);
}

vector<string>& CompilerArgs::input_files()
{
	return this->input_filenames;
}

void CompilerArgs::help()
{
	cout << this->desc << endl;
}
