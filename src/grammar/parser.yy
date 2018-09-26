%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {Origin}
%define parser_class_name {Parser}

%code requires 
{
	namespace Origin 
	{
		class Driver;
		class Scanner;
	}

	// The following definitions is missing when %locations isn't used
	#ifndef YY_NULLPTR
	#if defined __cplusplus && 201103L <= __cplusplus
	#define YY_NULLPTR nullptr
	#else
	#define YY_NULLPTR 0
	#endif
	#endif
}

%parse-param { Scanner& scanner  }
%parse-param { Driver& driver  }

%code
{
	#include <iostream>
	#include <cstdint>
	#include <cstdlib>
	#include <fstream>
	#include "../grammar/driver.hpp"
	#undef yylex
	#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token				END		0     "end of file"
%token	<uint8_t>	U8
%token	<uint16_t>	U16
%token	<uint32_t>	U32
%token	<uint64_t>	U64
%token	<int8_t>		S8
%token	<int16_t>	S16
%token	<int32_t>	S32
%token	<int64_t>	S64
%token	<float>		F32
%token	<double>	F64

%locations

%%

list_option : END | list END;

list
	: item
	| list item
	;

item
	: U8	| U16 | U32 | U64 | S8 | S16 | S32 | S64 | F32 | F64 { driver.add_number($1); }
	;

%%

void Origin::Parser::error(const location_type& l, const std::string& err_message)
{
	std::cerr << "ERROR: " << err_message << " at " << l << "\n";
}

