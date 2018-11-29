#pragma once
#include "3Ddungeon.h"

//~~~~~構造体~~~~~~

//イベント型
typedef struct TYPE_04 {
	int id;
	int imagebuf;
	POS eventPOS;
	unsigned char condition[16];
	unsigned char code[CODEMAX];
}EVENT;

//選択肢
typedef struct TYPE_15 {
	int* pointer;
	int cursor;
}QUESTION;

//イベント実行タスク用
typedef struct {
	int event_id;	//進行中のイベントid
	int ix;			//インデックスレジスタ
	int id;			//返ってきた選択
	TCB* childTask;	//子タスクへのポインタ
}T_EVENT_CONTROL;

//関数群
extern void event_moneyplus(TCB *thisTCB);
extern void event_itemchange(TCB *thisTCB);
extern void event_question(TCB* thisTCB);
extern void event_boot(TCB *thisTCB);
extern int event_init(void);

//変数
extern EVENT gameevent[LEVEL][EVENT_IN_A_FLOOR];	//イベントデータそのもの