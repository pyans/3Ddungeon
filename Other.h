#pragma once

#include "CODE.h"			//読み込み
#include "Task.h"

//#define DEBUG				//デバッグ

#define PI 3.14156			//エフェクト等に
#define LEVEL 16			//階数
#define MAPSIZE 32			//マップサイズ
#define INPUTBUFFLAME 16	//連続入力の記憶をどれだけするか
#define FONTSIZE 20			//フォントサイズ
#define CODEMAX 128			//イベントコードの長さ
#define MESNUMMAX 256		//メッセージの最大数
#define PARTY_NINZU 4		//パーティ人数
#define ITEMMAX 64			//所持アイテム最大数
#define EQUIPSIZE 8			//装備場所数
#define NAMEMAX 16			//名前文字数(マルチバイト文字の可能性大)
#define WAZAMAX 256			//ワザ最大数(適当)
#define ZOKUSEI 8			//斬殴突射熱冷電状

//FLOORMAP:マップと背景バッファ(増えるかも)
#define WALL 1
#define SPACE 0
typedef struct TYPE_01 {
	int backbuf;
	char map[MAPSIZE][MAPSIZE];
}FLOORMAP;

//POS:様々なオブジェクトの座標(Zはまだ足さない)
typedef struct TYPE_02 {
	int pos_x;
	int pos_y;
}POS;

//向きの列挙体
typedef enum ENUM_00 {
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
}DIR;

//PLAYER_POS:プレイヤー位置情報の格納
typedef struct TYPE_03 {
	POS pos;
	DIR dir;
	int nowfloor;								//今いる階
}PLAYER_POS;

//イベントデータ
#define EVENT_IN_A_FLOOR 32	//1階当たりのイベント存在数
//存在フラグ系
#define EVENT_EXSIT 0x01
#define EVENT_NULL	0x00

//コード位置用
#define EVENT_ISEXSIT 0		//イベントが存在するかどうか

//全体情報
typedef struct TYPE_05 {
	int money;
	int maxfloor;
	int item[ITEMMAX];
}PLAYER_INFO;

//ステータスデータ(サンプル)
typedef struct TYPE_06 {
	int lv;
	int maxhp;
	int maxmp;
	int str;
	int agi;
	int mus;
	int pow;
	int wil;
	int chr;
}STATUS;

//キャラデータ(サンプル)
typedef struct TYPE_07 {
	char name[NAMEMAX];
	STATUS st;
	int hp;
	int mp;
	int def[ZOKUSEI];
	int equip[EQUIPSIZE];
}CHARA;

//メニューや戦闘の選択
#define CHOOSEMAX 8
#define WAZALENGTH 64
typedef struct TYPE_EX01 {
	int num;
	int choice;
	int id[CHOOSEMAX];
	int *ret_p;
	char text[CHOOSEMAX][WAZALENGTH];
}SENTAKU;

//アイテムデータ(サンプル)
#define SOUBIWAZA 5			//装備技数
#define ITEMNUM 256			//アイテムデータ数
#define ITEMNAME 32			//アイテム名の長さ
//武器種類(と装備部位)
#define EQUIP_WEP 0x01
#define EQUIP_SHI 0x02
#define EQUIP_HANDS (EQUIP_WEP | EQUIP_SHI)
#define EQUIP_ITEM 0x04
#define EQUIP_CLOTH 0x08
#define EQUIP_BODY 0x10
#define EQUIP_HEAD 0x20
#define EQUIP_ARMS 0x40
#define EQUIP_REGS 0x80
#define EQUIP_FULL 0xF0
#define EQUIP_SOME 0x00
//装備耐性
#define EQUIP_NOGURAD 0x00
typedef struct TYPE_08 {
	char name[ITEMNAME];	//名前
	unsigned char equip_flags;		//どこに装備できるか
	int seinou;				//性能
	int zyuryou;			//重さ
	int taisei;				//装備耐性
	int def[ZOKUSEI];		//属性ごとの防御力
	int waza[SOUBIWAZA];	//固有技
}ITEM;

//モンスター等のサイズ
typedef struct TYPE_0D {
	int x;
	int y;
}IMAGE_SIZE;

//モンスターデータ
#define MONSETR_NAME 32
typedef struct TYPE_0E {
	char name[MONSETR_NAME];
	STATUS st;
	IMAGE_SIZE size;
	int equip[EQUIPSIZE];
	int koudou;
	int exp;				//ないし上昇レベル
	int gold;				//ゴールド
	int drop[2];			//ドロップアイテム:ノーマルとレア
	int skill;				//スキルレベルで判定する場合
}MONSTER_DATA;

//出現パターンデータ
#define MAXMONSTER 8		//出現モンスター数
#define MONSTERNUM 256		//モンスター数
#define HENSEI_P 8			//敵編成パターン数
typedef struct TYPE_0B {
	//0は存在しないことを表す
	int id[MAXMONSTER];		//モンスターid
}M_GROUP;

//技データ
#define WAZA_NAME 32
//依存能力値
#define IZON_STR 0
#define IZON_AGI 1
#define IZON_MUS 2
#define IZON_POW 3
#define IZON_WIL 4
#define IZON_CHR 5
//攻撃属性
#define ATK_SLASH 0x01
#define ATK_HIT 0x02
#define ATK_SPEAR 0x04
#define ATK_SHOOT 0x08
#define ATK_HEAT 0x10
#define ATK_ICE 0x20
#define ATK_THUNDER 0x40
#define ATK_POIZUN 0x80
#define ATK_ENERGY 0x00
//特殊フラグ
#define ATKSP_NOFLUGS 0x0000
//技のタイプ
#define SKILL_ATK 0x01
#define SKILL_HEAL 0x10
#define SPELL_ATK 0x20
#define SKILL_DEF 0x40
#define ITEM_HEAL 0x80
//特殊
#define COMM_DEF 0x07
typedef struct TYPE_10 {
	char name[WAZA_NAME];	//技名
	int iryoku;				//威力
	int seityou;			//成長度
	int meityu;				//命中
	short cost;				//コスト
	short izon;				//依存能力値
	short zokusei;			//攻撃属性
	int special_flug;		//特攻などの特殊フラグ
	short type;				//技のタイプ(攻撃、回復、状態異常等)
}WAZA_DATA;

//システム情報
typedef struct TYPE_09 {
	int canmove;
	int cursor;
	int encount;
	int game_over;
}SYS_INFO;

//以下:	い	つ	も	の
extern unsigned int PadInputBuff[INPUTBUFFLAME];
extern unsigned int ONInputBuff;
extern unsigned int OFFInputBuff;
extern unsigned int WordCr;
//列挙型変換用
#define STR(var) #var

//関数宣言
extern int soutai_col(int xplus, int yplus);			//移動時等の相対座標計算
extern int event_init();								//イベント初期化
extern void init_item();								//アイテム初期化
extern void message(TCB *thisTCB);
extern void encount(TCB* thisTCB);
extern int poscheck(POS a, POS b);