#pragma once

#include <vector>
#include <string>
#include <optional>
#include <unordered_map>
#include <sstream>
#include "token.h"
#include <stdarg.h>
#include "lexer-data.h"


class Lexer{
	public:
		Lexer(std::string& str);
		
		std::vector<Token> lex();
		
		std::vector<std::string> lines;
	private:
		
		std::string& m_str;
		size_t m_index = 0;
		
		struct PeekRet{
			bool h;
			char v;
		};
		
		PeekRet peek(int offset = 0) const;
		char inc();
		Token lexIdent(uint&,uint&);
		Token lexNumber(uint&,uint&);
		bool lexComment(uint&,uint&);
		bool lexString(uint&,uint&,std::vector<Token>& tokens);
};






