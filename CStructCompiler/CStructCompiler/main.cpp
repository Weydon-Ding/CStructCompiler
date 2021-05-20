#include "Lexer.h"
// ��ֵ���
// [Type] Identifier = value;
#include <iostream>
using namespace std;
// Print tokens to stdout. Used for -E.
static void print_tokens(Token *tok) {
	int line = 1;
	for (; tok->kind != TK_EOF; tok = tok->next) {
		if (line > 1 && tok->at_bol)
			cout << "\n";
		if (tok->has_space && !tok->at_bol)
			cout << " ";
		printf("%.*s", tok->len, tok->loc);
		line++;
	}
	cout << "\n";
}
int main(int argc, char** argv)
{
	const char* protocal = "\
	#pragma pack(push, 4)																	 \n\
	typedef struct {																	 \n\
		unsigned short unMsgSize;														 \n\
		unsigned short unMsgType;														 \n\
																						 \n\
		OBJID id;																		 \n\
		DWORD dwLookFace; // ��ò														 \n\
		DWORD dwLookFaceEx; //lookface2�����͡�ͷ��										 \n\
		char cLength; // �߶�															 \n\
		char cFat; // ����															     \n\
																						 \n\
		DWORD dwMoney;																	 \n\
		DWORD dwEmoney; // �������														 \n\
		INT64 nExp;																		 \n\
																						 \n\
		//LONG lDeed;																	 \n\
		//DWORD dwVirtue;																 \n\
																						 \n\
		int	  dwTutorExp;	//	��ʦ����												     \n\
		DWORD dwMercenaryExp;	// Ӷ������												 \n\
																						 \n\
																						 \n\
		int	   nPotential; // Ǳ��													   \n\
		USHORT usForce; // ����														   \n\
		USHORT usConstitution; // ����												   \n\
		USHORT usDexterity; // ����													   \n\
		USHORT usReduceSpeedRate; // �������ٶȵİٷֱ�								   \n\
		USHORT usHealth; // ����														   \n\
		USHORT usSoul; // ����														   \n\
																						 \n\
		USHORT usAdditionalPoint;														 \n\
																						 \n\
		UINT64 i64Life;																	 \n\
		UINT64 i64MaxLife; // �ͻ��˷���������������ˣ����ɷ������ṩ						\n\
		USHORT usMana;																	\n\
  		DWORD  dwSoulValue;    // ��������												\n\
  		DWORD  dwMaxSoulValue; // ����������											\n\
																						 \n\
		SHORT  sPk;																		 \n\
		UCHAR ucLevel;																	 \n\
		UCHAR ucProfession;																 \n\
		UCHAR ucNobility; // ����														   \n\
		UCHAR ucMetempsychosis; // ����												   \n\
		UCHAR ucAutoAllot; //���� 													   \n\
																						 \n\
		UCHAR ucTutorLevel; // ��ʦ�ȼ�													 \n\
		UCHAR ucMercenaryLevel; // Ӷ���ȼ�												 \n\
																						 \n\
		UCHAR	ucNobilityRank; // ��λ												   \n\
		USHORT	usMaxEudemon; // �����ٻ�������										 \n\
		DWORD	dwExploit; // ��ѫֵ													  \n\
		DWORD	dwBonusPoints;	// ���ֵ���												 \n\
		UCHAR	ucEudBroodPackSize;	// ������ϻ��޷������ߴ��С						  \n\
		UCHAR   ucWingLevel;		//�����߳��ȼ�(����ְҵΪ0);						  \n\
																						 \n\
		UCHAR	ucEudemonPackSize;	// ������ϵĻ��ޱ����ߴ�							   \n\
 		UCHAR	ucDemonLev;																 \n\
 		UINT64	u64DemonExp;															 \n\
		DWORD	dwDisableFlag;		// USER_DISABLE_FLAG  								 \n\
		UCHAR	ucGodLevel;			//��ȼ�											  \n\
		UCHAR	ucDeification;		//���											   \n\
		UINT64	u64GodExp;			//����(���ɵȼ�����)								  \n\
																						 \n\
		UINT64	u64IlluButterfly;	// ͼ��mask ����									  \n\
		UINT64	u64IlluFish;		// ��											\n\
		UINT64	u64IlluShell;		// ����											   \n\
		UINT64	u64IlluHerbal;		// ��ҩ											   \n\
		UINT64	u64IlluOre;			// ��ʯ											   \n\
		DWORD	dwVipLevel;			// VIP����										   \n\
		DWORD	dwWood;				// wood												 \n\
																						 \n\
		int     nBusiness;          // ���˺ű�ʶ										 \n\
		DWORD	dwTokenEmoney;		// ����											     \n\
		DWORD	dwTmoney;			// ʱ���											 \n\
		DWORD	dwSocialClass;		// ��Ҳ��											 \n\
 		DWORD	dwCmoney;			//�������õ�											 \n\
 		DWORD	dwCmoneyLimit;		//�����õ�											 \n\
		DWORD	idEudKernel;		// ��ǰ���޺���										 \n\
		DWORD	dwRefineryTimeEnd;	// ������������ʱ��									 \n\
		OBJID	idServer;			// ԭ������ID										 \n\
		SHORT	idCountry;			// ����������(����)ID							     \n\
		UCHAR	ucBoundaryLev;		// ���ȼ�											 \n\
		DWORD	dwBoundaryExp;		// ��羭��											 \n\
		DWORD	dwOfferPoint;		// �׼���											 \n\
		DWORD	dwAutoHangUpLeftTime;	//�������Զ��һ���ʱ��,��������Ѿ�ȥ����,�Ѿ�������\n\
 		DWORD   dwRechargeEmoney;	//�ۼƳ�ֵħʯ										 \n\
 		DWORD   dwFirstRecharge;	//�״γ�ֵ											 \n\
		DWORD   dwFirstRechargeEmoney;//�״γ�ֵ�ۼ�ħʯֵ								 \n\
		DWORD	dwGodFireFlag;		// ���װ��λ�������									 \n\
		// for str packer																 \n\
		char	szBuf[1];																 \n\
	}MSG_Info;																			 \n\
#pragma pack(pop)";
	Lexer t;
	Token* r = t.Tokenize(protocal);
	//while (r)
	//{
	//	if (r->at_bol)
	//		cout << endl;
	//	if (r->has_space)
	//		cout << " ";
	//	for (int i = 0; i < r->len; ++i)
	//	{
	//		const char temp = *(r->loc + i);
	//		cout << temp;
	//	}
	//	r = r->next;
	//}
	print_tokens(r);

	return 0;
}

