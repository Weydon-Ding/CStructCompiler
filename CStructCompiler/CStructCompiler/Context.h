#pragma once
#include "Lexer.h"

// Consumes the current token if it matches `op`.
bool equal(Token* tok, const char* op) {
	return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

bool consume(Token** rest, Token* tok, const char* str) {
	if (equal(tok, str)) {
		*rest = tok->next;
		return true;
	}
	*rest = tok;
	return false;
}

class Context
{
public:
	//Token* tokens;
	Token* Skip(Token* tok, const char* op);
};

inline Token* Context::Skip(Token* tok, const char* op)
{
	assert(equal(tok, op));
	return tok->next;
}