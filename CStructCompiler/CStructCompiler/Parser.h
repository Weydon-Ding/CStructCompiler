#pragma once
#include "Container.h"
#include "Context.h"

class Parser
{
	static Token* parse_typedef(Token *tok, Type *basety);
	//Container* Interprete(Context);
	static Context context;
};

Token* Parser::parse_typedef(Token *tok, Type *basety) {
	bool first = true;

	//while (!consume(&tok, tok, ";")) {
	//	if (!first)
	//		tok = context.Skip(tok, ",");
	//	first = false;

	//	Type *ty = declarator(&tok, tok, basety);
	//	assert(ty->name);
	//	push_scope(get_ident(ty->name))->type_def = ty;
	//}
	return tok;
}