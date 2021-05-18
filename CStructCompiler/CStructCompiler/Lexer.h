#pragma once
#include "Token.h"

class Lexer
{
public:
	Token* tokenize(const char* text);
	bool at_bol, has_space;
};
