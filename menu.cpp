#include "3Ddungeon.h"

/************************************
**********���j���[�Z�N�V����*********
***********�ʂ킟�����ꂽ**********
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
	int id;
}TYPE_MENU;

void menu_equip(TCB* thisTCB) {
	TYPE_MENU* var = (TYPE_MENU*)thisTCB->Work;
	SENTAKU* var_child;
	int i;
	//��l�ڂ̃L�����������Ȃ��̂ŁA��l�ڂ̃L�����̑�����������
	while (var->childTask != NULL) {
		switch (var->equip_enum) {
		case E_WHO:
			//�N��
			break;
		case E_WHERE:
			//�ǂ̑�����
			var->childTask = TaskMake(battle_choose, 0x2500);
			var_child = (SENTAKU*)var->childTask->Work;
			for (i = 0; i < (SENTAKU_X*SENTAKU_Y); i++) {
				sprintfDx(var_child->text[i], "%s", item_data[party[0].equip[i]].name);
			}
			break;
		case E_WHAT:
			//�ǂ̃A�C�e����
			break;
		default:
			TaskKill(thisTCB);
			break;
		}
		break;
	}
}

void menu_item(TCB* thisTCB) {

}

void event_menu(TCB* thisTCB) {

}