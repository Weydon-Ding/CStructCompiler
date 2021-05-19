#include "Lexer.h"
// ¸³ÖµÓï¾ä
// [Type] Identifier = value;

int main(int argc, char** argv)
{
	const char* protocal =" \
		int i;\n\
		struct MyStruct\n\
		{\
			//int; //error syntax\n\
		};\
		typedef unsigned int OBJID;\
		// ÉùÃ÷Óï¾ä\n\
		// typedef Type [±êÊ¶·û];\n\
	\
		// warning\n\
		int;\
		struct {};\
		typedef;\
		typedef int;\
		typedef struct {};\
		typedef struct MyStruct2 {};\
		// ¸³ÖµÓï¾ä\n\
		// [Type] Identifier = value;\n\
		struct MyStruct \
		{\
			char[111];\
			int id;\
		};";
	Lexer t;
	Token* r = t.Tokenize(protocal);

	return 0;
}