#pragma once

#include "map-wrap.h"

inline const Map<int> Tokens ={
	//Error
	{"ERROR",-2},
	
	// Types
	{"int", 2}, {"float", 2}, {"double", 2}, {"char", 2}, {"bool", 2}, {"void", 2}, {"auto", 2}, 
	{"short", 2}, {"long", 2}, {"unsigned", 2}, {"signed", 2}, {"wchar_t", 2}, {"size_t", 2},
	
	// Keywords
	{"fn", 4}, {"let", 4}, {"return", 4}, {"if", 4}, {"else", 4}, {"for", 4}, {"while", 4}, 
	{"do", 4}, {"switch", 4}, {"case", 4}, {"default", 4}, {"break", 4}, {"continue", 4}, 
	{"namespace", 4}, {"class", 4}, {"struct", 4}, {"enum", 4}, {"union", 4}, {"typedef", 4}, 
	{"template", 4}, {"public", 4}, {"private", 4}, {"protected", 4}, {"static", 4}, 
	{"const", 4}, {"constexpr", 4}, {"mutable", 4}, {"virtual", 4}, {"override", 4}, 
	{"explicit", 4}, {"inline", 4}, {"friend", 4}, {"using", 4}, {"new", 4}, {"delete", 4}, 
	{"try", 4}, {"catch", 4}, {"throw", 4}, {"noexcept", 4}, {"operator", 4}, {"this", 4}, 
	{"nullptr", 4}, {"true", 4}, {"false", 4}, {"sizeof", 4}, {"alignof", 4}, {"typeid", 4}, 
	{"static_assert", 4}, {"dynamic_cast", 4}, {"reinterpret_cast", 4}, {"const_cast", 4},
	
	// Operators
	{"++", 5}, {"--", 5}, {"==", 5}, {"!=", 5}, {">=", 5}, {"<=", 5}, {"&&", 5}, {"||", 5},
	{"->", 5}, {"+=", 5}, {"-=", 5}, {"*=", 5}, {"/=", 5}, {"%=", 5}, {"&=", 5}, {"|=", 5},
	{"^=", 5}, {"<<=", 5}, {">>=", 5}, {"::", 5}, {"?", 5}, {":", 5}, 
	{"=", 5}, {"+", 5}, {"-", 5}, {"*", 5}, {"/", 5}, {"%", 5}, {"!", 5}, {"&", 5}, {"|", 5},
	{"^", 5}, {"~", 5}, {"<", 5}, {">", 5}, {"<<", 5}, {">>", 5},
	
	// Symbols
	{";", 3}, {"{", 3}, {"}", 3}, {"(", 3}, {")", 3}, {"[", 3}, {"]", 3}, {",", 3}, {"...", 3}, {"#", 3}, {".", 3},
};
inline const BiMap TokenTypes ={
	{"ERROR",-2},
	{"Ident", 0},
	{"Const", 1},
	{"Type", 2},
	{"Symbol", 3},
	{"Keyword", 4},
	{"Operator", 5},
	{"String", 6},
};
inline const Map<int> StringTokens = {
	{"\"", 6},
	{"\'", 6},
};
struct CommentType {
	std::string start;  // Opening sequence (e.g., "/*")
	std::string end;    // Closing sequence (e.g., "*/"), empty for single-line comments
	bool isMultiline;   // True for block comments, false for single-line comments
};

inline const std::vector<CommentType> CommentTypes = {
	{"/'", "'/", true},
	{"/*", "*/", true},
	{"//", "", false},
};


