#pragma once
#define TASK_WORK_SIZE 1024
#define MAX_TASK_COUNT 256
#define _USE (1<<0)

//タスク用構造体
typedef struct TYPE_00 {
	void(*Exec)(TYPE_00*);					//実行関数へのポインタ
	TYPE_00* Prev;							//前タスクへのポインタ
	TYPE_00* Next;							//次タスクへのポインタ
	TYPE_00* Parent;						//親タスクへのポインタ
	unsigned int Flag;						//使用中フラグ
	unsigned int Prio;						//優先度
	int Imagebuf;							//グラフィックバッファ
	int Work[(TASK_WORK_SIZE + 3) / 4];		//データ領域(構造体を作って無理やりキャスト)
}TCB;

#ifndef TASK_INC_DONE
#define TASK_INC_DONE
#define _CRT_SECURE_NO_WARNINGS TRUE		//危険な関数の処理
#include "Dxlib.h"

//タスク関連関数
void TaskHead(TCB*);
void InitTask(void);
void TaskExec(void);
TCB* TaskMake(void(*exec)(TCB*), unsigned int, TCB* parent = NULL);
void TaskKill(TCB*);
void TaskChange(TCB*, void(*nextexec)(TCB*));
int PTaskDead(TCB*);

#endif

extern TCB Task[MAX_TASK_COUNT];
#pragma once
