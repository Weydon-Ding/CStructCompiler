int i;
struct MyStruct
{
	//int; //error syntax
};
typedef unsigned int OBJID;


// 声明语句
// typedef Type [标识符];

// warning
int;
struct {};
typedef;
typedef int;
typedef struct {};
typedef struct MyStruct2 {};
#include "Lexer.h"
// 赋值语句
// [Type] Identifier = value;

int main(int argc, char** argv)
{
	const char* protocal =
		"struct MyStruct \
		{\
			int id;\
		};";
	Lexer t;
	Token* r = t.Tokenize(protocal);

	return 0;
}