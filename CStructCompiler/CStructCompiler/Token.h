#pragma once

// Token
typedef enum {
	TK_IDENT,   // Identifiers
	TK_PUNCT,   // Punctuators
	TK_KEYWORD, // Keywords
	TK_STR,     // String literals
	TK_NUM,     // Numeric literals
	TK_PP_NUM,  // Preprocessing numbers
	TK_EOF,     // End-of-file markers
} TokenKind;

// Token type
typedef struct Token Token;
struct Token {
	TokenKind kind;   // Token kind
	Token *next;      // Next token
	int64_t val;      // If kind is TK_NUM, its value
	long double fval; // If kind is TK_NUM, its value
	char *loc;        // Token location
	int len;          // Token length
	Type *ty;         // Used if TK_NUM or TK_STR
	char *str;        // String literal contents including terminating '\0'

	File *file;       // Source location
	char *filename;   // Filename
	int line_no;      // Line number
	int line_delta;   // Line number
	bool at_bol;      // True if this token is at beginning of line
	bool has_space;   // True if this token follows a space character
	Hideset *hideset; // For macro expansion
	Token *origin;    // If this is expanded from a macro, the original token
};


// Create a new token.
static Token *new_token(TokenKind kind, char *start, char *end) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->loc = start;
	tok->len = end - start;
	tok->file = current_file;
	tok->filename = current_file->display_name;
	tok->at_bol = at_bol;
	tok->has_space = has_space;

	at_bol = has_space = false;
	return tok;
}