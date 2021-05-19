#pragma once
#include <iostream>
enum TypeKind
{
	//TY_VOID,
	TY_BOOL,
	TY_CHAR,
	TY_SHORT,
	TY_INT,
	TY_LONG,
	TY_FLOAT,
	TY_DOUBLE,
	TY_LDOUBLE,
	TY_ENUM,
	//TY_PTR,
	//TY_FUNC,
	TY_ARRAY,
	TY_VLA, // variable-length array
	TY_STRUCT,
	TY_UNION,
};

struct Type;
struct Token;
// Struct member
struct Member {
	Member* next;
	Type* ty;
	Token* tok; // for error message
	Token* name;
	int idx;
	int align;
	int offset;

	// Bitfield
	bool is_bitfield;
	int bit_offset;
	int bit_width;
};
struct Type
{
	TypeKind kind;
	int size;           // sizeof() value
	int align;          // alignment
	bool is_unsigned;   // unsigned or signed
	//bool is_atomic;     // true if _Atomic
	Type* origin;       // for type compatibility check

	// Pointer-to or array-of type. We intentionally use the same member
	// to represent pointer/array duality in C.
	//
	// In many contexts in which a pointer is expected, we examine this
	// member instead of "kind" member to determine whether a type is a
	// pointer or not. That means in many contexts "array of T" is
	// naturally handled as if it were "pointer to T", as required by
	// the C spec.
	Type* base;

	// Declaration
	Token* name;
	Token* name_pos;

	// Array
	int array_len;

	// Variable-length array
	//Node* vla_len; // # of elements
	//Obj* vla_size; // sizeof() value

	// Struct
	Member* members;
	bool is_flexible;
	bool is_packed;

	// Function type
	Type* return_ty;
	Type* params;
	bool is_variadic;
	Type* next;
};

// Token
enum TokenKind
{
	TK_IDENT,   // Identifiers
	TK_PUNCT,   // Punctuators
	TK_KEYWORD, // Keywords
	//TK_STR,     // String literals
	//TK_NUM,     // Numeric literals
	TK_PP_NUM,  // Preprocessing numbers
	TK_EOF,     // End-of-file markers
};

// Token type
struct Token
{
	TokenKind kind;   // Token kind
	Token* next;      // Next token
	//int64_t val;      // If kind is TK_NUM, its value
	//long double fval; // If kind is TK_NUM, its value
	const char* loc;        // Token location
	int len;          // Token length
	//Type* ty;         // Used if TK_NUM or TK_STR
	//char* str;        // String literal contents including terminating '\0'

	//File* file;       // Source location
	char* filename;   // Filename
	int line_no;      // Line number
	//int line_delta;   // Line number
	bool at_bol;      // True if this token is at beginning of line
	bool has_space;   // True if this token follows a space character
	//Hideset* hideset; // For macro expansion
	Token* origin;    // If this is expanded from a macro, the original token
};
//Type* ty_void = &(Type) { TY_VOID, 1, 1 };
//Type* ty_bool = &(Type) { TY_BOOL, 1, 1 };
//
//Type* ty_char = &(Type) { TY_CHAR, 1, 1 };
//Type* ty_short = &(Type) { TY_SHORT, 2, 2 };
//Type* ty_int = &(Type) { TY_INT, 4, 4 };
//Type* ty_long = &(Type) { TY_LONG, 8, 8 };
//
//Type* ty_uchar = &(Type) { TY_CHAR, 1, 1, true };
//Type* ty_ushort = &(Type) { TY_SHORT, 2, 2, true };
//Type* ty_uint = &(Type) { TY_INT, 4, 4, true };
//Type* ty_ulong = &(Type) { TY_LONG, 8, 8, true };
//
//Type* ty_float = &(Type) { TY_FLOAT, 4, 4 };
//Type* ty_double = &(Type) { TY_DOUBLE, 8, 8 };
//Type* ty_ldouble = &(Type) { TY_LDOUBLE, 16, 16 };

