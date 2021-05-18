#include "Lexer.h"
#include <string>
#include <assert.h>

inline static bool startswith(const char* p, const char* q)
{
	return strncmp(p, q, strlen(q)) == 0;
}

// Read a UTF-8-encoded Unicode code point from a source file.
// We assume that source files are always in UTF-8.
//
// UTF-8 is a variable-width encoding in which one code point is
// encoded in one to four bytes. One byte UTF-8 code points are
// identical to ASCII. Non-ASCII characters are encoded using more
// than one byte.
uint32_t decode_utf8(const char** new_pos, const char* p) {
	if ((unsigned char)*p < 128) {
		*new_pos = p + 1;
		return *p;
	}

	const char *start = p;
	int len;
	uint32_t c;

	if ((unsigned char)*p >= 0b11110000) {
		len = 4;
		c = *p & 0b111;
	}
	else if ((unsigned char)*p >= 0b11100000) {
		len = 3;
		c = *p & 0b1111;
	}
	else if ((unsigned char)*p >= 0b11000000) {
		len = 2;
		c = *p & 0b11111;
	}
	else {
		//error_at(start, "invalid UTF-8 sequence");
	}

	for (int i = 1; i < len; i++) {
		assert((unsigned char)p[i] >> 6 == 0b10);
		c = (c << 6) | (p[i] & 0b111111);
	}

	*new_pos = p + len;
	return c;
}
static bool in_range(uint32_t* range, uint32_t c) {
	for (int i = 0; range[i] != -1; i += 2)
		if (range[i] <= c && c <= range[i + 1])
			return true;
	return false;
}
// [https://www.sigbus.info/n1570#D] C11 allows not only ASCII but
// some multibyte characters in certan Unicode ranges to be used in an
// identifier.
//
// This function returns true if a given character is acceptable as
// the first character of an identifier.
//
// For example, ¾ (U+00BE) is a valid identifier because characters in
// 0x00BE-0x00C0 are allowed, while neither ⟘ (U+27D8) nor '　'
// (U+3000, full-width space) are allowed because they are out of range.
bool is_ident1(uint32_t c) {
	static uint32_t range[] = {
	  '_', '_', 'a', 'z', 'A', 'Z', '$', '$',
	  0x00A8, 0x00A8, 0x00AA, 0x00AA, 0x00AD, 0x00AD, 0x00AF, 0x00AF,
	  0x00B2, 0x00B5, 0x00B7, 0x00BA, 0x00BC, 0x00BE, 0x00C0, 0x00D6,
	  0x00D8, 0x00F6, 0x00F8, 0x00FF, 0x0100, 0x02FF, 0x0370, 0x167F,
	  0x1681, 0x180D, 0x180F, 0x1DBF, 0x1E00, 0x1FFF, 0x200B, 0x200D,
	  0x202A, 0x202E, 0x203F, 0x2040, 0x2054, 0x2054, 0x2060, 0x206F,
	  0x2070, 0x20CF, 0x2100, 0x218F, 0x2460, 0x24FF, 0x2776, 0x2793,
	  0x2C00, 0x2DFF, 0x2E80, 0x2FFF, 0x3004, 0x3007, 0x3021, 0x302F,
	  0x3031, 0x303F, 0x3040, 0xD7FF, 0xF900, 0xFD3D, 0xFD40, 0xFDCF,
	  0xFDF0, 0xFE1F, 0xFE30, 0xFE44, 0xFE47, 0xFFFD,
	  0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD,
	  0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD,
	  0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD,
	  0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, -1,
	};

	return in_range(range, c);
}
// Returns true if a given character is acceptable as a non-first
// character of an identifier.
bool is_ident2(uint32_t c) {
	static uint32_t range[] = {
	  '0', '9', '$', '$', 0x0300, 0x036F, 0x1DC0, 0x1DFF, 0x20D0, 0x20FF,
	  0xFE20, 0xFE2F, -1,
	};

	return is_ident1(c) || in_range(range, c);
}
// Read an identifier and returns the length of it.
// If p does not point to a valid identifier, 0 is returned.
static int read_ident(const char* start) {
	const char* p = start;
	uint32_t c = decode_utf8(&p, p);
	if (!is_ident1(c))
		return 0;

	for (;;) {
		const char* q;
		c = decode_utf8(&q, p);
		if (!is_ident2(c))
			return p - start;
		p = q;
	}
}
// Read a punctuator token from p and returns its length.
static int read_punct(const char* p) {
	/*
	static const char* kw[] = {
	  "<<=", ">>=", "...", "==", "!=", "<=", ">=", "->", "+=",
	  "-=", "*=", "/=", "++", "--", "%=", "&=", "|=", "^=", "&&",
	  "||", "<<", ">>", "##",
	};

	for (int i = 0; i < sizeof(kw) / sizeof(*kw); i++)
		if (startswith(p, kw[i]))
			return strlen(kw[i]);
	*/
	return ispunct(*p) ? 1 : 0;
}
// Initialize line info for all tokens.
static void add_line_numbers(Token* tok, const char* contents) {
	const char* p = contents;
	int n = 1;

	do {
		if (p == tok->loc) {
			tok->line_no = n;
			tok = tok->next;
		}
		if (*p == '\n')
			n++;
	} while (*p++);
}

Token* Lexer::Tokenize(const char* contents)
{
	Token head = {};
	Token* cur = &head;
	const char* p = contents;

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
			const char* q = strstr(p + 2, "*/");
			assert(q); // unclosed block comment
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

		/*
		// Numeric literal
		if (isdigit(*p) || (*p == '.' && isdigit(p[1]))) {
			const char* q = p++;
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
		*/

		// Identifier or keyword
		int ident_len = read_ident(p);
		if (ident_len > 0) {
			cur = cur->next = new_token(TK_IDENT, p, p + ident_len);
			p += cur->len;
			continue;
		}

		// Punctuators
		int punct_len = read_punct(p);
		if (punct_len > 0) {
			cur = cur->next = new_token(TK_PUNCT, p, p + punct_len);
			p += cur->len;
			continue;
		}

		assert(!p); // invalid token
	}

	cur = cur->next = new_token(TK_EOF, p, p);
	add_line_numbers(head.next, contents);
	return head.next;
}