#pragma once
#include "Token.h"

class Lexer
{
public:
	Token* Tokenize(const char* contents);
	bool at_bol, has_space;
	// Create a new token.
	Token* new_token(TokenKind kind, const char* start, const char* end) {
		Token* tok = (Token*)malloc(sizeof(Token));
		memset(tok, 0, sizeof(Token));
		tok->kind = kind;
		tok->loc = start;
		tok->len = end - start;
		//tok->file = current_file;
		//tok->filename = current_file->display_name;
		tok->at_bol = at_bol;
		tok->has_space = has_space;

		at_bol = has_space = false;
		return tok;
	}
};
