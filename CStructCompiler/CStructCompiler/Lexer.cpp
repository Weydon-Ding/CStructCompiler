#include "Lexer.h"
#include <string>

inline static bool startswith(const char* p, const char* q)
{
	return strncmp(p, q, strlen(q)) == 0;
}

inline Token* Lexer::tokenize(const char* p)
{
	//current_file = file;

	//char *p = file->contents;
	Token head = {};
	Token *cur = &head;

	at_bol = true;
	has_space = false;

	while (*p) {
		// Skip line comments.
		if (startswith(p, "//")) {
			p += 2;
			while (*p != '\n')
				p++;
			has_space = true;
			continue;
		}

		// Skip block comments.
		if (startswith(p, "/*")) {
			const char *q = strstr(p + 2, "*/");
			if (!q)
				error_at(p, "unclosed block comment");
			p = q + 2;
			has_space = true;
			continue;
		}

		// Skip newline.
		if (*p == '\n') {
			p++;
			at_bol = true;
			has_space = false;
			continue;
		}

		// Skip whitespace characters.
		if (isspace(*p)) {
			p++;
			has_space = true;
			continue;
		}

		// Numeric literal
		if (isdigit(*p) || (*p == '.' && isdigit(p[1]))) {
			const char *q = p++;
			for (;;) {
				if (p[0] && p[1] && strchr("eEpP", p[0]) && strchr("+-", p[1]))
					p += 2;
				else if (isalnum(*p) || *p == '.')
					p++;
				else
					break;
			}
			cur = cur->next = new_token(TK_PP_NUM, q, p);
			continue;
		}

		// String literal
		if (*p == '"') {
			cur = cur->next = read_string_literal(p, p);
			p += cur->len;
			continue;
		}

		// UTF-8 string literal
		if (startswith(p, "u8\"")) {
			cur = cur->next = read_string_literal(p, p + 2);
			p += cur->len;
			continue;
		}

		// UTF-16 string literal
		if (startswith(p, "u\"")) {
			cur = cur->next = read_utf16_string_literal(p, p + 1);
			p += cur->len;
			continue;
		}

		// Wide string literal
		if (startswith(p, "L\"")) {
			cur = cur->next = read_utf32_string_literal(p, p + 1, ty_int);
			p += cur->len;
			continue;
		}

		// UTF-32 string literal
		if (startswith(p, "U\"")) {
			cur = cur->next = read_utf32_string_literal(p, p + 1, ty_uint);
			p += cur->len;
			continue;
		}

		// Character literal
		if (*p == '\'') {
			cur = cur->next = read_char_literal(p, p, ty_int);
			cur->val = (char)cur->val;
			p += cur->len;
			continue;
		}

		// UTF-16 character literal
		if (startswith(p, "u'")) {
			cur = cur->next = read_char_literal(p, p + 1, ty_ushort);
			cur->val &= 0xffff;
			p += cur->len;
			continue;
		}

		// Wide character literal
		if (startswith(p, "L'")) {
			cur = cur->next = read_char_literal(p, p + 1, ty_int);
			p += cur->len;
			continue;
		}

		// UTF-32 character literal
		if (startswith(p, "U'")) {
			cur = cur->next = read_char_literal(p, p + 1, ty_uint);
			p += cur->len;
			continue;
		}

		// Identifier or keyword
		int ident_len = read_ident(p);
		if (ident_len) {
			cur = cur->next = new_token(TK_IDENT, p, p + ident_len);
			p += cur->len;
			continue;
		}

		// Punctuators
		int punct_len = read_punct(p);
		if (punct_len) {
			cur = cur->next = new_token(TK_PUNCT, p, p + punct_len);
			p += cur->len;
			continue;
		}

		error_at(p, "invalid token");
	}

	cur = cur->next = new_token(TK_EOF, p, p);
	add_line_numbers(head.next);
	return head.next;
}