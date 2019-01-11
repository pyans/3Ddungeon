#include "3Ddungeon.h"

EVENT gameevent[LEVEL][EVENT_IN_A_FLOOR];	//イベントデータそのもの

int event_init(void) {
	//floor 0
	//ガバガバだが時間が足りない
	//まあ発表近いからね、しょうがないね(妥協)

	//コード代入
	unsigned char temp[EVENT_IN_A_FLOOR][CODEMAX] = {
		{START,MES,0x00,ACSESS,EVENT_NULL,ENDEVENT},
		{START,CHOOSE,0x01,IF_NOZERO,0x06,MES,0x05,ENDEVENT},
		{START,MES,0x02,BATTLE,0x00,MES,0x03,ACSESS,
		EVENT_NULL,ENDEVENT},
		{START,MES,0x06,HEAL,ENDEVENT}
	};
	memcpy_s(gameevent[0][0].code,CODEMAX, temp[0],CODEMAX);
	memcpy_s(gameevent[0][1].code, CODEMAX, temp[1], CODEMAX);
	memcpy_s(gameevent[0][2].code, CODEMAX, temp[2], CODEMAX);
	memcpy_s(gameevent[0][3].code, CODEMAX, temp[3], CODEMAX);
	//存在フラグ
	gameevent[0][0].condition[0] = EVENT_EXSIT;
	gameevent[0][1].condition[0] = EVENT_EXSIT;
	gameevent[0][2].condition[0] = EVENT_EXSIT;
	gameevent[0][3].condition[0] = EVENT_EXSIT;
	//復活フラグ
	//今は作らない
	//位置
	gameevent[0][0].eventPOS = {29,29};
	gameevent[0][1].eventPOS = { 19,19 };//{19,19};
	gameevent[0][2].eventPOS = { 28,29 };//{19,19};
	gameevent[0][3].eventPOS = { 30,29 };//{19,19};
	return 1;
}

void event_moneyplus(TCB *thisTCB) {
	info.money += thisTCB->Work[0];//型を作って正式に実装
}

void event_itemchange(TCB *thisTCB) {
	//int i;
	//info.item[i]
	//アイテムは恐らくchar型になる予定
}

void event_question(TCB* thisTCB) {
	//店やメニューに使う予定
	//今のところシステム変数を使用
	QUESTION* var = (QUESTION*)thisTCB->Work;
	DrawFormatString(64, 400, WordCr, "はい");
	DrawFormatString(64, 424, WordCr, "いいえ");
	//カーソルの使用を変更、確認するべし
	DrawFormatString(32, 400 + 24 * var->cursor, WordCr, "→");
	if (ONInputBuff & PAD_INPUT_UP) {
		var->cursor = 1 - var->cursor;//上ボタン
	}
	if (ONInputBuff & PAD_INPUT_DOWN) {
		var->cursor = 1 - var->cursor;//下ボタン
	}
	if (ONInputBuff & PAD_INPUT_1) {			//Aボタンに反応してOFF
												//情報送信
												//親タスクに選択を送る
		*(var->pointer) = var->cursor;
		TaskKill(thisTCB);
	}
}

void event_control(TCB *thisTCB) {
	//現在優先度:0x2200
	//RPGの心臓部、イベントの管理タスク
	//関数に引き渡されたイベント番号からイベント型の情報からデータを読み出し、逐次実行する
	//別のタスクが必要なものは適宜別タスクを作って実行し、その間(子タスクが使用中の間)コントロールを中止する
	T_EVENT_CONTROL* var = (T_EVENT_CONTROL*)thisTCB->Work;
	TCB* childTask2;
	int i;
	//戦闘後の硬直回避
	systeminfo.canmove = 0;
	//コードに応じてループを回し、逐次実行する
	unsigned char temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];		//長ったらしいので代入
	while (temp != ENDEVENT && (var->childTask == NULL || !(var->childTask->Flag & _USE))) {
		//イベント終了まで、かつ子タスクが死んでいる間
		switch (temp) {
			//caseを作る
		case ENDEVENT:
			continue;
			break;
		case START:
			break;
		case MES:
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0001, thisTCB);	//タスク処理が必要な場合、タスクを制作
			var->ix++;
			temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
			sprintfDx((char*)(var->childTask->Work), message_text[playerpos.nowfloor][temp]);
			break;
		case CHOOSE:
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0001, thisTCB);	//タスク処理が必要な場合、タスクを制作
			var->ix++;
			temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
			sprintfDx((char*)(var->childTask->Work), message_text[playerpos.nowfloor][temp]);
			childTask2 = TaskMake(event_question, thisTCB->Prio + 0x0002, thisTCB);	//タスク処理が必要な場合、タスクを制作
																					//カーソル位置初期化はzeromemoryで実行済み
			((QUESTION*)childTask2->Work)->pointer = &var->id;
			break;
		case IF_ZERO:
			//idが0の場合ix=次の数へジャンプ
			var->ix++;
			if (var->id == 0) {
				var->ix = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
				//tempを更新しないといけない
				temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
				continue;
			}
			break;
		case IF_NOZERO:
			//idが0の場合ix=次の数へジャンプ
			var->ix++;
			if (var->id != 0) {
				var->ix = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
				//tempを更新しないといけない
				temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
				continue;
			}
			break;
		case GOTO:
			var->ix++;
			var->ix = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
			break;
		case TREASURE:
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0001, thisTCB);	//タスク処理が必要な場合、タスクを制作
			var->ix++;
			sprintfDx((char*)(var->childTask->Work), "宝箱の中身は%dだった！\nあなたは%sを袋にしまった。", item_data[var->ix].name, item_data[var->ix].name);
			for (i = 0; i < ITEMMAX; i++) {
				if (info.item[i] == 0xFF) {
					info.item[i] = var->ix;
					break;
				}
			}
			break;
		case BATTLE:
			//戦闘タスクを作成
			var->childTask = TaskMake(init_event_battle, thisTCB->Prio + 0x0100, thisTCB);
			//編成データの送信
			var->ix++;
			temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
			((TYPE_BATTLE*)var->childTask->Work)->group_id = temp;
			break;
		case ACSESS:
			//イベント状態変更
			//今は再起動しないイベントを作成
			var->ix++;
			gameevent[playerpos.nowfloor][var->event_id].condition[0] = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
			break;
		case HEAL:
			//回復
			var->ix++;
			battler_chara[0].cur_st.hp = battler_chara[0].bas_st.hp;
			battler_chara[0].cur_st.mp = battler_chara[0].bas_st.mp;
		default:
			break;
		}
		//更新
		var->ix++;
		temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
		if (var->ix > CODEMAX) {
			TaskKill(thisTCB);
			systeminfo.canmove = 1;
			return;
		}
	}
	if (temp == ENDEVENT && (var->childTask == NULL || !(var->childTask->Flag & _USE))/*コードが一通り実行された*/) {
		TaskKill(thisTCB);
		systeminfo.canmove = 1;
	}
}


void event_boot(TCB *thisTCB) {
	//現在優先度:0x2200
	//イベントの起動をチェックする
	T_EVENT_CONTROL* var = (T_EVENT_CONTROL*)thisTCB->Work;
	int i = 0;
	while (i < EVENT_IN_A_FLOOR) {
		if (gameevent[playerpos.nowfloor][i].condition[EVENT_ISEXSIT] == EVENT_NULL) {
			i++;
			continue;
		}
		if (poscheck(playerpos.pos, gameevent[playerpos.nowfloor][i].eventPOS)) {
			/*イベント発生位置に立っている && イベント発生条件を満たす*/
			systeminfo.canmove = 0;							//表示中は移動禁止(イベント中の方がええかも)
			TaskChange(thisTCB, event_control);
			var->event_id = i;							//イベントidを設定
			var->ix = 0;								//インデックスレジスタの初期化
			var->childTask = NULL;						//childTaskの初期化
			break;
		}
		i++;
	}
	if (systeminfo.canmove == 1) {
		encount(thisTCB);
		TaskKill(thisTCB);
	}
}