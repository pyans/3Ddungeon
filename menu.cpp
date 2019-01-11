#include "3Ddungeon.h"

/************************************
**********メニューセクション*********
***********ぬわぁぁん疲れた**********
*************************************/

typedef enum ENUM_05 {
	E_CANCEL = -1,
	E_WHO = 0,
	E_WHERE = 1,
	E_WHAT = 2,
	E_CHANGE = 3
}ENUM_EQUIP;

typedef struct TYPE_13 {
	TCB* childTask;
	ENUM_EQUIP equip_enum;
	int who;
	int place;
	int id;
}TYPE_MENU;

void menu_equip(TCB* thisTCB) {
	TYPE_MENU* var = (TYPE_MENU*)thisTCB->Work;
	SENTAKU* var_child;
	int i,temp;
	if (var->childTask == NULL || var->childTask->Flag != _USE) {
		switch (var->equip_enum) {
		case E_WHO:
			//誰の
			var->childTask = TaskMake(battle_choose, 0x2500,thisTCB);
			var_child = (SENTAKU*)var->childTask->Work;
			var_child->num = 0;
			for (i = 0; i < (PARTY_NINZU); i++) {
				sprintfDx(var_child->text[i], "%d:%s", i,battler_chara[i].name);
				var_child->id[i] = i;
				var_child->num++;
			}
			var_child->ret_p = &var->who;
			var->equip_enum = E_WHERE;
			break;
		case E_WHERE:
			//どの装備を
			if (var->who == -1) {
				var->equip_enum = E_CANCEL;
				break;
			}
			var->childTask = TaskMake(battle_choose, 0x2500, thisTCB);
			var_child = (SENTAKU*)var->childTask->Work;
			for (i = 0; i < (SENTAKU_X*SENTAKU_Y); i++) {
				sprintfDx(var_child->text[i], "%s", item_data[battler_chara[var->who].equip[i]].name);
				var_child->id[i] = i;
			}
			var_child->num = EQUIPSIZE;
			var_child->ret_p = &var->place;
			var->equip_enum = E_WHAT;
			break;
		case E_WHAT:
			//どのアイテムと
			if (var->place == -1) {
				var->equip_enum = E_WHO;
				break;
			}
			var->childTask = TaskMake(battle_choose, 0x2500, thisTCB);
			var_child = (SENTAKU*)var->childTask->Work;
			for (i = 0; i < (SENTAKU_X*SENTAKU_Y); i++) {
				sprintfDx(var_child->text[i], "%s", item_data[info.item[i]].name);
				var_child->id[i] = i;
			}
			var_child->num = EQUIPSIZE;
			var_child->ret_p = &var->id;
			var->equip_enum = E_CHANGE;
			break;
		case E_CHANGE:
			//変更
			if (var->id == -1) {
				var->equip_enum = E_WHERE;
				break;
			}
			temp = battler_chara[var->who].equip[var->place];
			battler_chara[var->who].equip[var->place] =info.item[var->id];
			info.item[var->id] = temp;
			var->equip_enum = E_WHERE;
			break;
		default:
			TaskKill(thisTCB);
			systeminfo.canmove = 1;
			break;
		}
	}
}

void menu_item(TCB* thisTCB) {

}

void event_menu(TCB* thisTCB) {

}