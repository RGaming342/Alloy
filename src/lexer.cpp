#include "../include/lexer.h"
#include <algorithm>



std::vector<std::string> SortTokensByLength(const Map& tokens) {
	std::vector<std::string> sortedTokens;
	// Extract keys from the map
	for (const auto& pair : tokens) {
		sortedTokens.push_back(pair.first);
	}
	// Sort by string length in descending order
	std::sort(sortedTokens.begin(), sortedTokens.end(), 
		[](const std::string& a, const std::string& b) {
			return a.length() > b.length();
		});
	return sortedTokens;
}

Lexer::Lexer(std::string& str) : m_str(str) {}

std::vector<Token> Lexer::lex() {
	std::istringstream iss(m_str);
	std::string line;
	lines.push_back("this should not be seen");
	while (std::getline(iss, line)) {
		lines.push_back(line);
	}
	uint lineNumber = 1;
	uint columnNumber = 1;
	std::vector<Token> tokens;
	auto sortedTokens = SortTokensByLength(Tokens);
	while (peek().h) {
		begin:
		if (peek().v == '\n'){
			inc();
			lineNumber++;
			columnNumber=1;
			continue;
		}
		if (std::isspace(peek().v)){
			inc();
			columnNumber++;
			continue;
		}
		
		if (std::isalpha(peek().v)||peek().v=='_') {
			tokens.push_back(lexIdent(lineNumber,columnNumber));
		} else if (std::isdigit(peek().v)) {
			tokens.push_back(lexNumber(lineNumber,columnNumber));
		} else {
			char currentChar = peek().v;
			
			if(lexComment(lineNumber,columnNumber)){
				continue;
			}
			
			if(lexString(lineNumber,columnNumber,tokens)){
				continue;
			}
			
			bool tokenMatched = false;
			for (const auto& key : sortedTokens) {
				if (m_str.substr(m_index, key.length()) == key) {
					tokens.push_back({ Tokens[key], key, lineNumber, columnNumber });
					m_index += key.length();
					columnNumber += key.length();
					tokenMatched = true;
					continue;
				}
			}
			if(!tokenMatched){
				tokens.push_back({ TokenTypes["ERROR"], ""+currentChar, lineNumber, columnNumber });
				columnNumber++;
				m_index++;
			}
		}
	}
	m_index = 0;
	return tokens;
}

Lexer::PeekRet Lexer::peek(int offset) const {
	if (m_index + offset >= m_str.size()) {
		return {0,'\0'};
	}
	return {1,m_str[m_index + offset]};
}

char Lexer::inc() {
	return m_str.at(m_index++);
}

Token Lexer::lexIdent(uint& ln,uint& cn) {
	std::string buf;
	uint tmp=cn;
	buf.push_back(inc());
	cn++;
	while (peek().h && (std::isalnum(peek().v) || peek().v == '_')) {
		buf.push_back(inc());
		cn++;
	}
	int tokenType = Tokens[buf];
	if (tokenType != -1) {
		return { tokenType, buf, ln, tmp };
	}
	return { TokenTypes["Ident"], buf, ln, tmp };
}

Token Lexer::lexNumber(uint& ln, uint& cn) {
	std::string buf;
	uint tmp=cn;
	buf.push_back(inc());
	cn++;
	while (peek().h && std::isdigit(peek().v)) {
		buf.push_back(inc());
		cn++;
	}
	if (peek().h && (peek().v == 'b' || peek().v == 'x')) {
		char ch = inc();
		cn++;
		buf.clear();
		while (peek().h && std::isdigit(peek().v)) {
			buf.push_back(inc());
			cn++;
		}
		int val;
		if(safe_stoi(buf, (ch=='b')?2:16, val)){
			buf = std::to_string(val);
		}else{
			return { TokenTypes["ERROR"], buf, ln, tmp };
		}
	}else if (peek().h && peek().v == '.') {
		buf.push_back(inc());
		cn++;
		while (peek().h && std::isdigit(peek().v)) {
			buf.push_back(inc());
			cn++;
		}
		if(buf.back()=='.'){
			return { TokenTypes["ERROR"], buf, ln, tmp };
		}
	}
	return { TokenTypes["Const"], buf, ln, tmp };
}

bool Lexer::lexComment(uint& ln,uint& cn) {
	for (const auto& comment : CommentTypes) {
		// Check if the current sequence matches the start of a comment
		bool matchesStart = true;
		for (size_t i = 0; i < comment.start.size(); i++) {
			if (!peek(i).h || peek(i).v != comment.start[i]) {
				matchesStart = false;
				break;
			}
		}
		
		if (matchesStart) {
			// Move past the start sequence
			for (size_t i = 0; i < comment.start.size(); i++) {
				inc();
			}
			cn += comment.start.size();
			
			if (comment.isMultiline) {
				while (peek().h) {
					// Check for the end sequence
					bool matchesEnd = true;
					for (size_t i = 0; i < comment.end.size(); i++) {
						if (!peek(i).h || peek(i).v != comment.end[i]) {
							matchesEnd = false;
							break;
						}
					}
					
					if (matchesEnd) {
						// Move past the end sequence
						for (size_t i = 0; i < comment.end.size(); i++) {
							inc();
						}
						cn += comment.end.size();
						return true;
					}
					
					// Handle newlines correctly
					if (peek().v == '\n') {
						ln++;
						cn = 1;
					} else {
						cn++;
					}
					inc();
				}
			} else {
				// Single-line comment: consume until newline
				while (peek().h && peek().v != '\n') {
					inc();
				}
				inc();
				ln++;
				cn = 1;
				return true;
			}
		}
	}
	return false;
}

bool Lexer::lexString(uint& ln,uint& cn,std::vector<Token>& tokens){
	uint tmpcn = cn;
	uint tmpln = ln;
	for (const auto& str : StringTokens) {
		bool matchesStart = true;
		for (size_t i = 0; i < str.first.size(); i++) {
			if (!peek(i).h || peek(i).v != str.first[i]) {
				matchesStart = false;
				break;
			}
		}
		if (matchesStart) {
			std::string buf;
			// Move past the start sequence
			for (size_t i = 0; i < str.first.size(); i++) {
				inc();
			}
			cn += str.first.size();
			
			while (peek().h) {
				// Check for the end sequence
				bool matchesEnd = true;
				for (size_t i = 0; i < str.first.size(); i++) {
					if (!peek(i).h || peek(i).v != str.first[i]) {
						matchesEnd = false;
						break;
					}
				}
				
				if (matchesEnd) {
					// Move past the end sequence
					for (size_t i = 0; i < str.first.size(); i++) {
						inc();
					}
					cn += str.first.size();
					tokens.push_back({TokenTypes["String"],buf,tmpln,tmpcn});
					return true;
				}
				
				// Handle newlines correctly
				if (peek().v == '\n') {
					ln++;
					cn = 1;
				} else {
					cn++;
				}
				buf.push_back(inc());
			}
		}
	}
	return false;
}
