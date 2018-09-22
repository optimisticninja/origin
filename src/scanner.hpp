#pragma once

#if !defined(yyFlexLexerOnce)
	#include <FlexLexer.h>
#endif

#include "generated/parser.tab.hh"
#include "generated/location.hh"

using std::istream;

namespace Origin
{
	class Scanner : public yyFlexLexer
	{
	public:
		Scanner(istream* in) : yyFlexLexer(in) {};
		virtual ~Scanner() {};
		// get rid of override virtual function warning
		using FlexLexer::yylex;

		virtual	int yylex(Parser::semantic_type* const lval, Parser::location_type* location);
	private:
		Parser::semantic_type* yylval = nullptr;
	};
}

