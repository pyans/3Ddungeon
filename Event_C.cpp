#include "3Ddungeon.h"

EVENT gameevent[LEVEL][EVENT_IN_A_FLOOR];	//�C�x���g�f�[�^���̂���

int event_init(void) {
	//floor 0
	//�K�o�K�o�������Ԃ�����Ȃ�
	//�܂����\�߂�����ˁA���傤���Ȃ���(�Ë�)

	//�R�[�h���
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
	//���݃t���O
	gameevent[0][0].condition[0] = EVENT_EXSIT;
	gameevent[0][1].condition[0] = EVENT_EXSIT;
	gameevent[0][2].condition[0] = EVENT_EXSIT;
	gameevent[0][3].condition[0] = EVENT_EXSIT;
	//�����t���O
	//���͍��Ȃ�
	//�ʒu
	gameevent[0][0].eventPOS = {29,29};
	gameevent[0][1].eventPOS = { 19,19 };//{19,19};
	gameevent[0][2].eventPOS = { 28,29 };//{19,19};
	gameevent[0][3].eventPOS = { 30,29 };//{19,19};
	return 1;
}

void event_moneyplus(TCB *thisTCB) {
	info.money += thisTCB->Work[0];//�^������Đ����Ɏ���
}

void event_itemchange(TCB *thisTCB) {
	//int i;
	//info.item[i]
	//�A�C�e���͋��炭char�^�ɂȂ�\��
}

void event_question(TCB* thisTCB) {
	//�X�⃁�j���[�Ɏg���\��
	//���̂Ƃ���V�X�e���ϐ����g�p
	QUESTION* var = (QUESTION*)thisTCB->Work;
	DrawFormatString(64, 400, WordCr, "�͂�");
	DrawFormatString(64, 424, WordCr, "������");
	//�J�[�\���̎g�p��ύX�A�m�F����ׂ�
	DrawFormatString(32, 400 + 24 * var->cursor, WordCr, "��");
	if (ONInputBuff & PAD_INPUT_UP) {
		var->cursor = 1 - var->cursor;//��{�^��
	}
	if (ONInputBuff & PAD_INPUT_DOWN) {
		var->cursor = 1 - var->cursor;//���{�^��
	}
	if (ONInputBuff & PAD_INPUT_1) {			//A�{�^���ɔ�������OFF
												//��񑗐M
												//�e�^�X�N�ɑI���𑗂�
		*(var->pointer) = var->cursor;
		TaskKill(thisTCB);
	}
}

void event_control(TCB *thisTCB) {
	//���ݗD��x:0x2200
	//RPG�̐S�����A�C�x���g�̊Ǘ��^�X�N
	//�֐��Ɉ����n���ꂽ�C�x���g�ԍ�����C�x���g�^�̏�񂩂�f�[�^��ǂݏo���A�������s����
	//�ʂ̃^�X�N���K�v�Ȃ��͓̂K�X�ʃ^�X�N������Ď��s���A���̊�(�q�^�X�N���g�p���̊�)�R���g���[���𒆎~����
	T_EVENT_CONTROL* var = (T_EVENT_CONTROL*)thisTCB->Work;
	TCB* childTask2;
	int i;
	//�퓬��̍d�����
	systeminfo.canmove = 0;
	//�R�[�h�ɉ����ă��[�v���񂵁A�������s����
	unsigned char temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];		//�������炵���̂ő��
	while (temp != ENDEVENT && (var->childTask == NULL || !(var->childTask->Flag & _USE))) {
		//�C�x���g�I���܂ŁA���q�^�X�N������ł����
		switch (temp) {
			//case�����
		case ENDEVENT:
			continue;
			break;
		case START:
			break;
		case MES:
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0001, thisTCB);	//�^�X�N�������K�v�ȏꍇ�A�^�X�N�𐧍�
			var->ix++;
			temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
			sprintfDx((char*)(var->childTask->Work), message_text[playerpos.nowfloor][temp]);
			break;
		case CHOOSE:
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0001, thisTCB);	//�^�X�N�������K�v�ȏꍇ�A�^�X�N�𐧍�
			var->ix++;
			temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
			sprintfDx((char*)(var->childTask->Work), message_text[playerpos.nowfloor][temp]);
			childTask2 = TaskMake(event_question, thisTCB->Prio + 0x0002, thisTCB);	//�^�X�N�������K�v�ȏꍇ�A�^�X�N�𐧍�
																					//�J�[�\���ʒu��������zeromemory�Ŏ��s�ς�
			((QUESTION*)childTask2->Work)->pointer = &var->id;
			break;
		case IF_ZERO:
			//id��0�̏ꍇix=���̐��փW�����v
			var->ix++;
			if (var->id == 0) {
				var->ix = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
				//temp���X�V���Ȃ��Ƃ����Ȃ�
				temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
				continue;
			}
			break;
		case IF_NOZERO:
			//id��0�̏ꍇix=���̐��փW�����v
			var->ix++;
			if (var->id != 0) {
				var->ix = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
				//temp���X�V���Ȃ��Ƃ����Ȃ�
				temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
				continue;
			}
			break;
		case GOTO:
			var->ix++;
			var->ix = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
			break;
		case TREASURE:
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0001, thisTCB);	//�^�X�N�������K�v�ȏꍇ�A�^�X�N�𐧍�
			var->ix++;
			sprintfDx((char*)(var->childTask->Work), "�󔠂̒��g��%d�������I\n���Ȃ���%s��܂ɂ��܂����B", item_data[var->ix].name, item_data[var->ix].name);
			for (i = 0; i < ITEMMAX; i++) {
				if (info.item[i] == 0xFF) {
					info.item[i] = var->ix;
					break;
				}
			}
			break;
		case BATTLE:
			//�퓬�^�X�N���쐬
			var->childTask = TaskMake(init_event_battle, thisTCB->Prio + 0x0100, thisTCB);
			//�Ґ��f�[�^�̑��M
			var->ix++;
			temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
			((TYPE_BATTLE*)var->childTask->Work)->group_id = temp;
			break;
		case ACSESS:
			//�C�x���g��ԕύX
			//���͍ċN�����Ȃ��C�x���g���쐬
			var->ix++;
			gameevent[playerpos.nowfloor][var->event_id].condition[0] = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
			break;
		case HEAL:
			//��
			var->ix++;
			battler_chara[0].cur_st.hp = battler_chara[0].bas_st.hp;
			battler_chara[0].cur_st.mp = battler_chara[0].bas_st.mp;
		default:
			break;
		}
		//�X�V
		var->ix++;
		temp = gameevent[playerpos.nowfloor][var->event_id].code[var->ix];
		if (var->ix > CODEMAX) {
			TaskKill(thisTCB);
			systeminfo.canmove = 1;
			return;
		}
	}
	if (temp == ENDEVENT && (var->childTask == NULL || !(var->childTask->Flag & _USE))/*�R�[�h����ʂ���s���ꂽ*/) {
		TaskKill(thisTCB);
		systeminfo.canmove = 1;
	}
}


void event_boot(TCB *thisTCB) {
	//���ݗD��x:0x2200
	//�C�x���g�̋N�����`�F�b�N����
	T_EVENT_CONTROL* var = (T_EVENT_CONTROL*)thisTCB->Work;
	int i = 0;
	while (i < EVENT_IN_A_FLOOR) {
		if (gameevent[playerpos.nowfloor][i].condition[EVENT_ISEXSIT] == EVENT_NULL) {
			i++;
			continue;
		}
		if (poscheck(playerpos.pos, gameevent[playerpos.nowfloor][i].eventPOS)) {
			/*�C�x���g�����ʒu�ɗ����Ă��� && �C�x���g���������𖞂���*/
			systeminfo.canmove = 0;							//�\�����͈ړ��֎~(�C�x���g���̕�����������)
			TaskChange(thisTCB, event_control);
			var->event_id = i;							//�C�x���gid��ݒ�
			var->ix = 0;								//�C���f�b�N�X���W�X�^�̏�����
			var->childTask = NULL;						//childTask�̏�����
			break;
		}
		i++;
	}
	if (systeminfo.canmove == 1) {
		encount(thisTCB);
		TaskKill(thisTCB);
	}
}