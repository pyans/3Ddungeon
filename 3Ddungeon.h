#pragma once
#include "dxlib.h"
#include "Task.h"
#include "CODE.h"
#include "other.h"
#include "event.h"
#include "battle.h"

//変数宣言
extern EVENT gameevent[LEVEL][EVENT_IN_A_FLOOR];		//イベントデータ
extern const char* message_text[LEVEL][MESNUMMAX];		//メッセージのデータ
extern int monster_img[MONSTERNUM];						//モンスターのイメージバッファ
extern MONSTER_DATA monster_data[MONSTERNUM];			//モンスターデータ
extern const M_GROUP hensei[LEVEL][HENSEI_P];			//敵編成データ
extern const WAZA_DATA waza_data[WAZAMAX];				//技データ
extern ITEM item_data[ITEMNUM];							//アイテムのデータ
extern PLAYER_POS playerpos;							//プレイヤー位置情報
extern CHARA party[PARTY_NINZU];						//パーティ情報
extern SYS_INFO systeminfo;								//システム情報
extern PLAYER_INFO info;								//プレイヤー情報