#pragma once

#include "../generated/parser.tab.hh"

using Origin::Parser;

namespace AST
{
	enum BinOp {
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE
	};
	
	class Root
	{
	private:
		Parser::location_type& location;
		
	public:
		Root() = default;
		virtual ~Root();
	};
	
	template<typename T> class Expression : Root
	{
	private:
		T* _lhs = nullptr;
		T* _rhs = nullptr;
		
	public:
		Expression() = default;
		virtual ~Expression();
		
		T& lhs()
		{
			return this->_lhs;
		}
		
		T& rhs()
		{
			return this->_rhs;
		}
	};
	
	template<typename T> class BinOpExpression : Expression<T>
	{
	private:
		BinOp _binop;
		
	public:
		BinOp binop()
		{
			return this->_binop;
		}
	};
};
