#include "3Ddungeon.h"

const WAZA_DATA waza_data[WAZAMAX] = {
	{
		//id_0
		"剣",			//名前
		36,				//威力
		3,				//成長度
		240,			//命中
		0,				//コスト
		IZON_STR,		//依存能力値
		ATK_SLASH,		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_ATK		//技タイプ
	},

	{
		//id_1
		"棍棒",			//名前
		50,				//威力
		4,				//成長度
		216,			//命中
		0,				//コスト
		IZON_STR,		//依存能力値
		ATK_HIT,		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_ATK		//技タイプ
	},

	{
		//id_2
		"槍",			//名前
		30,				//威力
		3,				//成長度
		248,			//命中
		0,				//コスト
		IZON_STR,		//依存能力値
		ATK_SPEAR,		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_ATK		//技タイプ
	},

	{
		//id_3
		"弓",			//名前
		32,				//威力
		2,				//成長度
		232,			//命中
		0,				//コスト
		IZON_WIL,		//依存能力値
		ATK_SHOOT,		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_ATK		//技タイプ
	},

	{
		//id_4
		"斧",			//名前
		50,				//威力
		4,				//成長度
		208,			//命中
		0,				//コスト
		IZON_STR,		//依存能力値
		(ATK_SLASH | ATK_HIT),		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_ATK		//技タイプ
	},

	{
		//id_5
	},

	{
		//id_6
	},

	{
		//id_7
		"身を守る",			//名前
		0,			//威力
		0,				//成長度
		255,			//命中
		0,				//コスト
		IZON_STR,		//依存能力値
		ATK_ENERGY,		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_DEF		//技タイプ
	},

	{
		//id_8
		"傷薬",			//名前
		100,			//威力
		0,				//成長度
		255,			//命中
		0,				//コスト
		IZON_STR,		//依存能力値
		ATK_ENERGY,		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_HEAL		//技タイプ
	},

	{
		//id_9
		"ニ段切り",			//名前
		100,			//威力
		2,				//成長度
		255,			//命中
		2,				//コスト
		IZON_STR,		//依存能力値
		ATK_SLASH,		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_ATK		//技タイプ
	},

	{
		//id_A
		"袈裟切り",			//名前
		160,			//威力
		5,				//成長度
		224,			//命中
		2,				//コスト
		IZON_STR,		//依存能力値
		ATK_SLASH,		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_ATK		//技タイプ
	},

	{
		//id_B
		"兜割り",		//名前
		200,			//威力
		5,				//成長度
		224,			//命中
		3,				//コスト
		IZON_STR,		//依存能力値
		(ATK_SLASH | ATK_HIT),		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_ATK		//技タイプ
	},

	{
		//id_C
		"満月大根切り",		//名前
		2400,			//威力
		15,				//成長度
		255,			//命中
		13,				//コスト
		IZON_STR,		//依存能力値
		ATK_SLASH ,		//タイプ
		ATKSP_NOFLUGS,	//特殊フラグ
		SKILL_ATK		//技タイプ
	},
};