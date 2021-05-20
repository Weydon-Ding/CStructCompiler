#include "Lexer.h"
// 赋值语句
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
		DWORD dwLookFace; // 外貌														 \n\
		DWORD dwLookFaceEx; //lookface2：发型、头型										 \n\
		char cLength; // 高度															 \n\
		char cFat; // 胖瘦															     \n\
																						 \n\
		DWORD dwMoney;																	 \n\
		DWORD dwEmoney; // 虚拟货币														 \n\
		INT64 nExp;																		 \n\
																						 \n\
		//LONG lDeed;																	 \n\
		//DWORD dwVirtue;																 \n\
																						 \n\
		int	  dwTutorExp;	//	导师经验												     \n\
		DWORD dwMercenaryExp;	// 佣兵经验												 \n\
																						 \n\
																						 \n\
		int	   nPotential; // 潜能													   \n\
		USHORT usForce; // 力量														   \n\
		USHORT usConstitution; // 体质												   \n\
		USHORT usDexterity; // 敏捷													   \n\
		USHORT usReduceSpeedRate; // 被减的速度的百分比								   \n\
		USHORT usHealth; // 耐力														   \n\
		USHORT usSoul; // 精神														   \n\
																						 \n\
		USHORT usAdditionalPoint;														 \n\
																						 \n\
		UINT64 i64Life;																	 \n\
		UINT64 i64MaxLife; // 客户端放弃计算最大生命了，改由服务器提供						\n\
		USHORT usMana;																	\n\
  		DWORD  dwSoulValue;    // 魂魄数量												\n\
  		DWORD  dwMaxSoulValue; // 最大魂魄数量											\n\
																						 \n\
		SHORT  sPk;																		 \n\
		UCHAR ucLevel;																	 \n\
		UCHAR ucProfession;																 \n\
		UCHAR ucNobility; // 无用														   \n\
		UCHAR ucMetempsychosis; // 无用												   \n\
		UCHAR ucAutoAllot; //无用 													   \n\
																						 \n\
		UCHAR ucTutorLevel; // 导师等级													 \n\
		UCHAR ucMercenaryLevel; // 佣兵等级												 \n\
																						 \n\
		UCHAR	ucNobilityRank; // 爵位												   \n\
		USHORT	usMaxEudemon; // 最大可召唤幻兽数										 \n\
		DWORD	dwExploit; // 功勋值													  \n\
		DWORD	dwBonusPoints;	// 积分点数												 \n\
		UCHAR	ucEudBroodPackSize;	// 玩家身上幻兽孵化器尺寸大小						  \n\
		UCHAR   ucWingLevel;		//异能者翅膀等级(其他职业为0);						  \n\
																						 \n\
		UCHAR	ucEudemonPackSize;	// 玩家身上的幻兽背包尺寸							   \n\
 		UCHAR	ucDemonLev;																 \n\
 		UINT64	u64DemonExp;															 \n\
		DWORD	dwDisableFlag;		// USER_DISABLE_FLAG  								 \n\
		UCHAR	ucGodLevel;			//神等级											  \n\
		UCHAR	ucDeification;		//神格											   \n\
		UINT64	u64GodExp;			//神力(神仙等级经验)								  \n\
																						 \n\
		UINT64	u64IlluButterfly;	// 图鉴mask 蝴蝶									  \n\
		UINT64	u64IlluFish;		// 鱼											\n\
		UINT64	u64IlluShell;		// 贝壳											   \n\
		UINT64	u64IlluHerbal;		// 草药											   \n\
		UINT64	u64IlluOre;			// 矿石											   \n\
		DWORD	dwVipLevel;			// VIP级别										   \n\
		DWORD	dwWood;				// wood												 \n\
																						 \n\
		int     nBusiness;          // 商人号标识										 \n\
		DWORD	dwTokenEmoney;		// 代币											     \n\
		DWORD	dwTmoney;			// 时间点											 \n\
		DWORD	dwSocialClass;		// 玩家层次											 \n\
 		DWORD	dwCmoney;			//可用信用点											 \n\
 		DWORD	dwCmoneyLimit;		//总信用点											 \n\
		DWORD	idEudKernel;		// 当前幻兽核心										 \n\
		DWORD	dwRefineryTimeEnd;	// 灵能炼化到期时间									 \n\
		OBJID	idServer;			// 原服务器ID										 \n\
		SHORT	idCountry;			// 世界树区域(国度)ID							     \n\
		UCHAR	ucBoundaryLev;		// 结界等级											 \n\
		DWORD	dwBoundaryExp;		// 结界经验											 \n\
		DWORD	dwOfferPoint;		// 献祭点											 \n\
		DWORD	dwAutoHangUpLeftTime;	//还可以自动挂机的时间,这个设置已经去掉了,已经无用了\n\
 		DWORD   dwRechargeEmoney;	//累计充值魔石										 \n\
 		DWORD   dwFirstRecharge;	//首次充值											 \n\
		DWORD   dwFirstRechargeEmoney;//首次充值累计魔石值								 \n\
		DWORD	dwGodFireFlag;		// 神火装备位开启情况									 \n\
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

