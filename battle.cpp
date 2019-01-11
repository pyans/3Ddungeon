/************************************
***********バトルセクション**********
***********みんな踊れー！！**********
*************************************/
#include "3Ddungeon.h"

CHARA battler_st[BATTLERSNUM];//戦闘者の能力値、hpなどのデータ。

void battle_choose(TCB* thisTCB) {
	SENTAKU* var = (SENTAKU*)thisTCB->Work;
	int i, j;
	for (i = 0; i < SENTAKU_X; i++) {
		for (j = 0; j < SENTAKU_Y; j++) {
			//選択肢を表示する
			//320は画面の中央
			if (i * SENTAKU_Y + j == var->num) {
				continue;
			}
			DrawFormatString(CHOOSE_0_X + i*(320 - CHOOSE_0_X), CHOOSE_0_Y + j*FONTSIZE, WordCr, var->text[i * SENTAKU_Y + j]);
		}
	}
	DrawFormatString(CHOOSE_0_X - FONTSIZE + (var->choice / SENTAKU_Y)*(320 - CHOOSE_0_X), CHOOSE_0_Y + (var->choice % SENTAKU_Y)*FONTSIZE, WordCr, "→");
	//決定
	if (ONInputBuff & PAD_INPUT_1) {
		*(var->ret_p) = var->id[var->choice];//死んだタスクとの交信はまずいのでポインタで記憶する
		TaskKill(thisTCB);
	}
	//キャンセル
	if (ONInputBuff & PAD_INPUT_2) {
		*(var->ret_p) = -1;//いまはキャンセル=-1
		TaskKill(thisTCB);
	}
	//上キー
	if (ONInputBuff & PAD_INPUT_UP) {
		var->choice--;
		if (var->choice < 0)var->choice = var->num - 1;
	}
	//下キー
	if (ONInputBuff & PAD_INPUT_DOWN) {
		var->choice++;
		if (var->choice > var->num - 1)var->choice = 0;
	}
	//左キー
	//右キー
	if (PTaskDead(thisTCB)) {
		TaskKill(thisTCB);
	}
}

void battle_player_command(TCB *thisTCB) {
	TYPE_P_COMMAND* var = (TYPE_P_COMMAND*)thisTCB->Work;
	TYPE_BATTLE *var_parent = (TYPE_BATTLE*)thisTCB->Parent->Work;
	SENTAKU* var_child;
	int i;
	if (var->childTask == NULL || var->childTask->Flag != _USE) {
		ITEM temp_i;
		switch (var->flow) {
		case WHAT:
			//何をするかのタスク制作
			var->childTask = TaskMake(battle_choose, thisTCB->Prio + 0x100, thisTCB->Parent);
			//初期化
			var_child = (SENTAKU*)var->childTask->Work;
			var_child->ret_p = &var->waza_id;
			//初期アイコン位置決定
			temp_i = item_data[battler_chara[var_parent->counter].equip[var->equip]];
			while (temp_i.waza[0] == -1) {
				var->equip++;
				temp_i = item_data[battler_chara[var_parent->counter].equip[var->equip]];
			}
			sprintfDx(var_child->text[0], "%-15s:%d", temp_i.name, 0);
			var_child->id[0] = temp_i.waza[0];
			//攻撃と防御
			var_child->num = 2;
			//装備固有技
			for (i = 1; i < SOUBIWAZA; i++) {
				if (item_data[battler_chara[var_parent->counter].equip[var->equip]].waza[i] != -1) {
					//技名、技idを入力
					sprintfDx(var_child->text[i], "%-15s:%d", waza_data[temp_i.waza[i]].name, waza_data[temp_i.waza[i]].cost);
					var_child->id[i] = temp_i.waza[i];
					var_child->num++;
				}
			}
			//防御
			sprintfDx(var_child->text[var_child->num - 1], "防御");
			//後々変わるかも
			var_child->id[var_child->num - 1] = COMM_DEF;
			var_child->choice = 0;
			var->flow = WHOM;
			break;
		case WHOM:
			//誰に攻撃するのかタスク制作
			//全体攻撃の場合、タスク制作を行わない？
			if (var->waza_id == -1) {
				var->flow = WHAT;
				break;
			}
			if (waza_data[var->waza_id].type == SKILL_DEF/*技idで参照したデータが防御の場合*/) {
				var->flow = COMMAND_END;
				break;
			}
			var->childTask = TaskMake(battle_choose, thisTCB->Prio + 0x100, thisTCB->Parent);
			//初期化
			var_child = (SENTAKU*)var->childTask->Work;
			//敵の数から参照(今は1で固定)
			//var_child->num = ((TYPE_BATTLE*)thisTCB->Parent->Work)->group_id;
			var_child->num = 0;
			for (i = BATTLERSNUM - MAXMONSTER; i < BATTLERSNUM; i++) {
				if (var_parent->battler[i].chara_id > NO_EXSIT) {
					//名前表示
					sprintfDx(var_child->text[var_child->num], monster_data[var_parent->battler[i].chara_id].name);
					var_child->id[var_child->num] = i;
					var_child->num++;
				}
			}
			var_child->ret_p = &var->whom;
			var->flow = COMMAND_END;
			break;
		case COMMAND_END:
			//使う技とターゲットを指定
			//battler.command、battler.taisyouに代入
			if (var->whom == -1) {
				var->flow = WHAT;
				var->whom = 0;
				break;
			}
			else if (PTaskDead(thisTCB) == FALSE) {
				var_parent->battler[var_parent->counter].command = var->waza_id;
				var_parent->battler[var_parent->counter].taisyou = var->whom;
				var_parent->battler[var_parent->counter].weppon = battler_chara[var_parent->counter].equip[var->equip];
				var_parent->battler[var_parent->counter].speed = battler_chara[var_parent->counter].cur_st.agi*(1 + GetRand(255) / 256);
				//MP消費(本番は行動時に)
				battler_chara[var_parent->counter].cur_st.mp -= waza_data[var->waza_id].cost;
				//カウンタを増やす
				var_parent->counter++;
			}
			TaskKill(thisTCB);
			break;
		}
	}

	//消滅判定
	if (PTaskDead(thisTCB)) {
		TaskKill(thisTCB);
	}
}

void enemy_command(TCB* thisTCB) {
	TYPE_BATTLE *var = (TYPE_BATTLE*)thisTCB->Work;
	//とりあえず、一番上の武器の一番目の技を使う
	var->battler[var->counter].command = item_data[monster_data[var->battler[var->counter].chara_id].equip[0]].waza[0];
	var->battler[var->counter].weppon = monster_data[var->battler[var->counter].chara_id].equip[0];
	var->counter++;
}

//速度交換用
void swap_battler(BATTLER* a, BATTLER* b) {
	BATTLER temp = *a;
	*a = *b;
	*b = temp;
}

//速度ソート
void sort_battler(TCB *thisTCB) {
	TYPE_BATTLE* var = (TYPE_BATTLE*)thisTCB->Work;
	int i = 0, j = 0;
	//バブルソート
	for (i = 0; i < BATTLERSNUM; i++) {
		for (j = BATTLERSNUM - 1; j > i; j--) {
			//早い順に小さくなるように
			if (var->battler[j].speed > var->battler[j - 1].speed) {
				//スワップ
				int temp = var->battler[j].turn;
				var->battler[j].turn = var->battler[j - 1].turn;
				var->battler[j - 1].turn = temp;
			}
		}
	}
}

//ダメージ計算関数
int acter_damage_cul(STATUS st, ITEM weppon, WAZA_DATA waza) {
	int lv = st.lv;
	int status;
	int wep = weppon.seinou;
	int c = waza.seityou;
	//依存能力を調べる
	switch (waza.izon) {
	case IZON_STR:
		status = st.str;
		break;
	case IZON_POW:
		status = st.pow;
		break;
	default:
		status = 0;
		break;
	}
	int damege;
	//成長度が影響していない
	damege = waza.iryoku + (double)((status + 100) / 100) * (double)((lv * (64 - lv) * (status / 2)*wep / 64) + (GetRand(256) + 256)*(wep + c + (status / 2))*lv / 1024);
	return damege;
}

int defencer_damage_cul(STATUS st, int def, int su_damege) {
	int damege = su_damege * (double)(128 - def - (st.mus / 2)) / 128;
	return damege - def - st.mus;
}

int damege_clu(TCB* thisTCB) {
	BATTLE_ACT* var = (BATTLE_ACT*)thisTCB->Work;
	TYPE_BATTLE* var_parent = (TYPE_BATTLE*)thisTCB->Parent->Work;
	int acter = var->acter;
	int defencer = var->defecer;
	WAZA_DATA waza = waza_data[var_parent->battler[acter].command];
	STATUS act_st, def_st;
	ITEM weppon;
	int lv;
	bool atk_is_monster = 0;
	bool def_is_monster = 1;
	int atk_monster_id, def_monster_id;
	int atk_party_id, def_party_id;
	//モンスターと人を調べる
	if (acter >= (BATTLERSNUM - MAXMONSTER)) {
		atk_is_monster = TRUE;
		atk_monster_id = var_parent->battler[acter].chara_id;
	}
	else {
		atk_is_monster = FALSE;
		atk_party_id = acter;
	}
	if (defencer >= (BATTLERSNUM - MAXMONSTER)) {
		def_is_monster = TRUE;
		def_monster_id = var_parent->battler[defencer].chara_id;
	}
	else {
		def_is_monster = FALSE;
		def_party_id = defencer;
	}
	//能力値コピー
	if (atk_is_monster) {
		//最終的に現在ステータスを使う
		act_st = monster_data[atk_monster_id].st;
	}
	else {
		act_st = battler_chara[atk_party_id].cur_st;
	}
	if (def_is_monster) {
		//最終的に現在ステータスを使う
		def_st = monster_data[def_monster_id].st;
	}
	else {
		def_st = battler_chara[def_party_id].cur_st;
	}
	//武器データ
	weppon = item_data[var_parent->battler[acter].weppon];
	//技の参照を調べる
	//技のタイプを調べる
	//省略
	int damege = 0;
	//素ダメ計算
	damege = max(0, acter_damage_cul(act_st, weppon, waza));
	//防御計算
	int i, def = 0, prevdef = 0;
	for (i = 0; i < ZOKUSEI; i++) {
		unsigned char bit = 0x01 << i;
		if (waza.zokusei & bit) {
			//初物か前の防御力より低ければ
			if (prevdef == 0 || def < prevdef) {
				if (def_is_monster) {
					//ここもいじる
					def = battler_chara[def_monster_id].def[i];
				}
				else {
					def = battler_chara[def_party_id].def[i];
				}
			}
		}
	}
	damege = max(0, defencer_damage_cul(def_st, def, damege));
	return damege;
}

void battle_act(TCB* thisTCB) {
	//優先度:0x2301
	//変数宣言
	BATTLE_ACT* var = (BATTLE_ACT*)thisTCB->Work;
	TYPE_BATTLE* var_parent = (TYPE_BATTLE*)thisTCB->Parent->Work;
	int d;
	if (var->childTask == NULL || var->childTask->Flag != _USE) {
		switch (var->act_enum) {
		case MESS:
			//メッセージ
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0100, thisTCB);
			if (var->acter >= (BATTLERSNUM - MAXMONSTER)) {
				//敵行動
				sprintfDx((char*)(var->childTask->Work), "%sの行動！", monster_data[var_parent->battler[var->acter].chara_id].name);
			}
			else {
				sprintfDx((char*)(var->childTask->Work), "%sの行動！", battler_chara[var->acter].name);
			}
			var->act_enum = EFF;
			if (var_parent->battler[var->acter].command == COMM_DEF) {
				if (var->acter >= (BATTLERSNUM - MAXMONSTER)) {
					//敵行動
					sprintfDx((char*)(var->childTask->Work), "%sは身を守った！", monster_data[var_parent->battler[var->acter].chara_id].name);
				}
				else {
					sprintfDx((char*)(var->childTask->Work), "%sは身を守った！", battler_chara[var->acter].name);
				}
				var->act_enum = END_DM;
			}
			break;
		case EFF:
			//エフェクト
			var->act_enum = DAMEGE;
			break;
		case DAMEGE:
			//ダメージ
			d = damege_clu(thisTCB);
			//注意！このままでは能力値やHPの変化がわからない
			//データ構造の見直しを測れ
			//とりあえずHPだけBATTLERに追加
			if (waza_data[var_parent->battler[var->defecer].command].type == SKILL_DEF) {
				d = d / 2;
			}
			battler_chara[var->defecer].cur_st.hp = max(0, (battler_chara[var->defecer].cur_st.hp - d));
			//メッセージ
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0100, thisTCB);
			if (var->defecer >= (BATTLERSNUM - MAXMONSTER)) {
				//敵行動
				sprintfDx((char*)(var->childTask->Work), "%sに %dのダメージ！", monster_data[var_parent->battler[var->defecer].chara_id].name, d);
			}
			else {
				sprintfDx((char*)(var->childTask->Work), "%sに %dのダメージ！", battler_chara[var->defecer].name, d);
			}
			var->act_enum = DEATH;
			break;
		case DEATH:
			//死亡確認
			if (battler_chara[var->defecer].cur_st.hp == 0) {
				var->childTask = TaskMake(message, thisTCB->Prio + 0x0100, thisTCB);
				if (var->defecer >= (BATTLERSNUM - MAXMONSTER)) {
					//敵行動
					sprintfDx((char*)(var->childTask->Work), "%sを倒した！", monster_data[var_parent->battler[var->defecer].chara_id].name);
				}
				else {
					sprintfDx((char*)(var->childTask->Work), "%sは倒れた……\nしかし、今は開発中なので復活した!", battler_chara[var->defecer].name);
				}
				var_parent->battler[var->defecer].chara_id = -1;
			}
			var->act_enum = END_DM;
			break;
		default:
			//カウンタ増加
			var_parent->counter++;
			TaskKill(thisTCB);
			break;
		}
	}

}

void event_battle(TCB* thisTCB) {
	//優先度:0x2300
	TYPE_BATTLE* var = (TYPE_BATTLE*)thisTCB->Work;
	int i = 0;
	unsigned int ikiteru = 0;
	if (var->childTask == NULL || var->childTask->Flag != _USE) {
		switch (var->flow) {
		case BATTLE_END:
			//プレイヤーとモンスター、どちらが死んだか調べ
			//勝った場合成長や戦利品、負けた場合ゲームオーバーへ
			//各々の生存フラグ
			ikiteru = 0;
			for (i = 0; i < BATTLERSNUM; i++) {
				if (var->battler[i].chara_id > NO_EXSIT) {
					ikiteru = ikiteru | 1 << i;
				}
			}
			if (!(ikiteru & 0xff)) {
				//systeminfo.game_over = 1;
				//ゲームオーバー処理がややこしいので、今は生き返る
				battler_chara[0].cur_st.hp = battler_chara[0].bas_st.hp;
				//動けるようにする
				systeminfo.canmove = 1;
				TaskKill(thisTCB);
			}
			else if (!(ikiteru & 0xff00)) {
				//動けるようにする
				systeminfo.canmove = 1;
				TaskKill(thisTCB);
			}
			else {
				var->flow = SETUP;
			}
			break;
		case BATTLE_START:
			//モンスター名の表示
			//今はメインモンスター名の表示
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0001, thisTCB);
			sprintfDx((char*)(var->childTask->Work), "%sが　現れた！\n", monster_data[var->battler[BATTLERSNUM - MAXMONSTER].chara_id].name);
			var->flow = SETUP;
			break;
		case SETUP:
			//セットアップタスクを作る
			var->flow = PLAYER_COMM;
			break;
		case PLAYER_COMM:
			//もし残りの味方がいれば、味方の行動決定を行う
			//そうでなければ、敵行動決定タスクを制作
			if (var->counter < (BATTLERSNUM - MAXMONSTER)) {
				//カウンタはループ変数
				if (var->battler[var->counter].chara_id > NO_EXSIT) {
					//プレイヤーキャラが存在する数だけタスクを作る
					var->childTask = TaskMake(battle_player_command, thisTCB->Prio + 0x0100, thisTCB);
					TYPE_P_COMMAND* var_child = (TYPE_P_COMMAND*)var->childTask->Work;
					var_child->who = var->counter;
					var_child->equip = 0;
					var_child->flow = WHAT;
				}
				else {
					//いないなら飛ばす
					var->counter++;
				}
			}
			else {
				//敵行動決定タスクへ
				//カウンタはそのまま
				var->flow = ENEMY_COMM;
			}
			break;
		case ENEMY_COMM:
			//全て決定していれば、行動準決定へ
			//そうでなければ次の敵行動決定
			if (var->battler[var->counter].chara_id > NO_EXSIT) {
				//プレイヤーキャラが存在する数だけタスクを作る
				enemy_command(thisTCB);
			}
			else {
				//いないなら飛ばす
				var->counter++;
			}
			if (var->counter >= BATTLERSNUM) {
				var->flow = SORT;
				//カウンタの初期化
				var->counter = 0;
			}
			break;
		case SORT:
			//敵味方のidを行動値順ソート
			//その後行動させる
			//フォールダウン(breakなし)もあり？
			sort_battler(thisTCB);
			var->flow = ACT;
			break;
		case ACT:
			//ソートした順番に技を行う
			//技id、対象、行為者id等が必要？
			//行為ごとに全滅を判定
			//デバッグタスク挿入
			//行動者捜索
			for (i = 0; i < BATTLERSNUM; i++) {
				if (var->battler[i].turn == var->counter) {
					break;
				}
			}
			//ターン終了判定
			if (i >= BATTLERSNUM) {
				var->flow = TURNEND;
				var->counter = 0;
				break;
			}
			//そうでなく、なおかつ行為者が存在すればタスク制作
			else if (var->battler[i].chara_id > NO_EXSIT) {
				var->childTask = TaskMake(battle_act, thisTCB->Prio + 0x0100, thisTCB);
				BATTLE_ACT* var_child2 = (BATTLE_ACT*)var->childTask->Work;
				var_child2->act_enum = MESS;
				var_child2->acter = i;
				var_child2->defecer = var->battler[i].taisyou;
				//対象不在の時は飛ばす
				if (var->battler[var_child2->defecer].chara_id == NO_EXSIT) {
					TaskKill(var->childTask);
					var->counter++;
				}
			}
			else {
				//カウンタ増加
				var->counter++;
			}
			break;
		case TURNEND:
			//その他処理
			//デバッグの為戦闘終了させる
			var->flow = BATTLE_END;
			break;
		}
	}

	//モンスター画像表示
	int count_monster = 0;
	for (i = BATTLERSNUM-1; i > BATTLERSNUM - MAXMONSTER-1; i--) {
		if (var->battler[i].chara_id > NO_EXSIT) {
			DrawRotaGraph(192+64*count_monster, 240, 4.0, 0, monster_img[var->battler[i].chara_id], TRUE);
			count_monster++;
		}
	}

	//消滅判定
	if (PTaskDead(thisTCB)) {
		TaskKill(thisTCB);
	}
}

void init_event_battle(TCB* thisTCB) {
	TYPE_BATTLE* var = (TYPE_BATTLE*)thisTCB->Work;
	int i, j, k;
	for (i = 0; i < BATTLERSNUM; i++) {
		//仲間の存在に応じて仲間のデータ代入(turn0~7)
		//終了したらモンスターデータ代入(8~15)
		//編成データがガバガバなので注意
		//今は8以上ならイベントバトル扱い
		if ((BATTLERSNUM - i) <= MAXMONSTER) {
			if (var->group_id < HENSEI_P) {
				//グループidが編成パターン数より下→通常エンカウント
				var->battler[i].chara_id = hensei[playerpos.nowfloor][var->group_id].id[i - MAXMONSTER];
				if (var->battler[i].chara_id > NO_EXSIT) {
					battler_chara[i].cur_st.hp = monster_data[var->battler[i].chara_id].st.hp;
					//守備力の更新
					for (k = 0; k < ZOKUSEI; k++) {
						battler_chara[i].def[k] = 0;
					}
					for (j = 4; j < EQUIPSIZE; j++) {
						for (k = 0; k < ZOKUSEI; k++) {
							battler_chara[i].def[k] += item_data[monster_data[var->battler[i].chara_id].equip[j]].def[k];
						}
					}
				}
			}
			else {
				//グループidが編成パターン数以上→イベントバトル(実装中の為今は適当なidを代入)
				var->battler[i].chara_id = hensei[playerpos.nowfloor][var->group_id % 8].id[i - MAXMONSTER];
			}
		}
		else {
			if (i < PARTY_NINZU) {
				if (battler_chara[i].bas_st.hp < 1) {
					//元ステータスHPが0以下のキャラは存在しないものとする
					var->battler[i].chara_id = -1;
				}
				else {
					//キャラidの3バイト目に書き込む
					var->battler[i].chara_id = i << 16;
					var->battler[i].st_data = &battler_chara[i];
					//守備力の更新
					for (k = 0; k < ZOKUSEI; k++) {
						battler_chara[i].def[k] = 0;
					}
					for (j = 4; j < EQUIPSIZE; j++) {
						for (k = 0; k < ZOKUSEI; k++) {
							battler_chara[i].def[k] += item_data[battler_chara[i].equip[j]].def[k];
						}
					}
				}
			}
			else {
				//今はいないです
				var->battler[i].chara_id = -1;
			}
		}
		var->battler[i].turn = i;
	}
	//カウンタを初期化()
	var->counter = 0;
	TaskChange(thisTCB, event_battle);
}