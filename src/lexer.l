%{
	#include <algorithm>
	#include <string>
	#include <iostream>
	#include "../scanner.hpp"

	/* msvc2010 requires that we exclude this header file. */
	#define YY_NO_UNISTD_H
	#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
	#undef  YY_DECL
	#define YY_DECL \
		int Origin::Scanner::yylex(Origin::Parser::semantic_type* const lval, Origin::Parser::location_type* loc)
	#define yyterminate() return token::END

	using token = Origin::Parser::token;
	using std::cout;
	using std::string;
	using std::transform;
	using std::stof;
	using std::stod;
	using std::stoi;
%}

%option debug
%option nodefault
%option yyclass="Origin::Scanner"
%option noyywrap
%option c++

%%
%{
	yylval = lval;
%}

[0-9]*?\.[0-9]+([fF](32|64)?)? /** Floating point literal - defaults to F64 */ {
	string dup(yytext);
	transform(dup.begin(), dup.end(), dup.begin(), ::tolower);
	string suffix;
	size_t suffix_start = dup.find('f');
	
	if (suffix_start != string::npos) {
		suffix = dup.substr(suffix_start, string::npos);
		dup = dup.substr(0, suffix_start - 1);
		
		if (suffix.compare("f32") == 0 || suffix.length() == 1) {
			yylval->build<float>(stof(dup));
			return token::F32;
		}
	}
	
	yylval->build<double>(stod(dup));
	return token::F64;
}

[0-9]+(([sSuU])(8|16|32|64))? {
	yylval->build<int>(stoi(yytext));
	return token::S32;
}

[ \t] ; // Ignore whitespace

\n {
	loc->lines();
}

. ;
%%
