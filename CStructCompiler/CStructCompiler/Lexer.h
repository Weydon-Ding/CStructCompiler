#pragma once
#include "Token.h"
#include <assert.h>

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
	static Token *append_tokens(Token *tok1, Token *tok2) {
		if (!tok1 || tok1->kind == TK_EOF)
			return tok2;

		Token *t = tok1;
		while (t->next->kind != TK_EOF)
			t = t->next;
		t->next = tok2;
		return tok1;
	}

	//TODO: ºêÌæ»»
};
