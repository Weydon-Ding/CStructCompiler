int i;
struct MyStruct
{
	//int; //error syntax
};
typedef unsigned int OBJID;


// �������
// typedef Type [��ʶ��];

// warning
int;
struct {};
typedef;
typedef int;
typedef struct {};
typedef struct MyStruct2 {};
#include "Lexer.h"
// ��ֵ���
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