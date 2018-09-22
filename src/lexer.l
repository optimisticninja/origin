%{
	#include <string>
	#include "../scanner.hpp"

	/* msvc2010 requires that we exclude this header file. */
	#define YY_NO_UNISTD_H
	#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
	#undef  YY_DECL
	#define YY_DECL \
		int Origin::Scanner::yylex(Origin::Parser::semantic_type* const lval, Origin::Parser::location_type* loc)
	#define yyterminate() return token::END

	using token = Origin::Parser::token;
	using std::string;
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

[a-z]		{
			return token::LOWER;
		}

[A-Z]		{
			return token::UPPER;
		}

[a-zA-Z]+	{
			yylval->build<string>(yytext);
			return token::WORD;
		}

\n		{
			loc->lines();
			return token::NEWLINE;
		}

.        	{
			return token::CHAR;
		}
%%
