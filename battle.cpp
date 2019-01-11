/************************************
***********�o�g���Z�N�V����**********
***********�݂�ȗx��[�I�I**********
*************************************/
#include "3Ddungeon.h"

CHARA battler_st[BATTLERSNUM];//�퓬�҂̔\�͒l�Ahp�Ȃǂ̃f�[�^�B

void battle_choose(TCB* thisTCB) {
	SENTAKU* var = (SENTAKU*)thisTCB->Work;
	int i, j;
	for (i = 0; i < SENTAKU_X; i++) {
		for (j = 0; j < SENTAKU_Y; j++) {
			//�I������\������
			//320�͉�ʂ̒���
			if (i * SENTAKU_Y + j == var->num) {
				continue;
			}
			DrawFormatString(CHOOSE_0_X + i*(320 - CHOOSE_0_X), CHOOSE_0_Y + j*FONTSIZE, WordCr, var->text[i * SENTAKU_Y + j]);
		}
	}
	DrawFormatString(CHOOSE_0_X - FONTSIZE + (var->choice / SENTAKU_Y)*(320 - CHOOSE_0_X), CHOOSE_0_Y + (var->choice % SENTAKU_Y)*FONTSIZE, WordCr, "��");
	//����
	if (ONInputBuff & PAD_INPUT_1) {
		*(var->ret_p) = var->id[var->choice];//���񂾃^�X�N�Ƃ̌�M�͂܂����̂Ń|�C���^�ŋL������
		TaskKill(thisTCB);
	}
	//�L�����Z��
	if (ONInputBuff & PAD_INPUT_2) {
		*(var->ret_p) = -1;//���܂̓L�����Z��=-1
		TaskKill(thisTCB);
	}
	//��L�[
	if (ONInputBuff & PAD_INPUT_UP) {
		var->choice--;
		if (var->choice < 0)var->choice = var->num - 1;
	}
	//���L�[
	if (ONInputBuff & PAD_INPUT_DOWN) {
		var->choice++;
		if (var->choice > var->num - 1)var->choice = 0;
	}
	//���L�[
	//�E�L�[
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
			//�������邩�̃^�X�N����
			var->childTask = TaskMake(battle_choose, thisTCB->Prio + 0x100, thisTCB->Parent);
			//������
			var_child = (SENTAKU*)var->childTask->Work;
			var_child->ret_p = &var->waza_id;
			//�����A�C�R���ʒu����
			temp_i = item_data[battler_chara[var_parent->counter].equip[var->equip]];
			while (temp_i.waza[0] == -1) {
				var->equip++;
				temp_i = item_data[battler_chara[var_parent->counter].equip[var->equip]];
			}
			sprintfDx(var_child->text[0], "%-15s:%d", temp_i.name, 0);
			var_child->id[0] = temp_i.waza[0];
			//�U���Ɩh��
			var_child->num = 2;
			//�����ŗL�Z
			for (i = 1; i < SOUBIWAZA; i++) {
				if (item_data[battler_chara[var_parent->counter].equip[var->equip]].waza[i] != -1) {
					//�Z���A�Zid�����
					sprintfDx(var_child->text[i], "%-15s:%d", waza_data[temp_i.waza[i]].name, waza_data[temp_i.waza[i]].cost);
					var_child->id[i] = temp_i.waza[i];
					var_child->num++;
				}
			}
			//�h��
			sprintfDx(var_child->text[var_child->num - 1], "�h��");
			//��X�ς�邩��
			var_child->id[var_child->num - 1] = COMM_DEF;
			var_child->choice = 0;
			var->flow = WHOM;
			break;
		case WHOM:
			//�N�ɍU������̂��^�X�N����
			//�S�̍U���̏ꍇ�A�^�X�N������s��Ȃ��H
			if (var->waza_id == -1) {
				var->flow = WHAT;
				break;
			}
			if (waza_data[var->waza_id].type == SKILL_DEF/*�Zid�ŎQ�Ƃ����f�[�^���h��̏ꍇ*/) {
				var->flow = COMMAND_END;
				break;
			}
			var->childTask = TaskMake(battle_choose, thisTCB->Prio + 0x100, thisTCB->Parent);
			//������
			var_child = (SENTAKU*)var->childTask->Work;
			//�G�̐�����Q��(����1�ŌŒ�)
			//var_child->num = ((TYPE_BATTLE*)thisTCB->Parent->Work)->group_id;
			var_child->num = 0;
			for (i = BATTLERSNUM - MAXMONSTER; i < BATTLERSNUM; i++) {
				if (var_parent->battler[i].chara_id > NO_EXSIT) {
					//���O�\��
					sprintfDx(var_child->text[var_child->num], monster_data[var_parent->battler[i].chara_id].name);
					var_child->id[var_child->num] = i;
					var_child->num++;
				}
			}
			var_child->ret_p = &var->whom;
			var->flow = COMMAND_END;
			break;
		case COMMAND_END:
			//�g���Z�ƃ^�[�Q�b�g���w��
			//battler.command�Abattler.taisyou�ɑ��
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
				//MP����(�{�Ԃ͍s������)
				battler_chara[var_parent->counter].cur_st.mp -= waza_data[var->waza_id].cost;
				//�J�E���^�𑝂₷
				var_parent->counter++;
			}
			TaskKill(thisTCB);
			break;
		}
	}

	//���Ŕ���
	if (PTaskDead(thisTCB)) {
		TaskKill(thisTCB);
	}
}

void enemy_command(TCB* thisTCB) {
	TYPE_BATTLE *var = (TYPE_BATTLE*)thisTCB->Work;
	//�Ƃ肠�����A��ԏ�̕���̈�Ԗڂ̋Z���g��
	var->battler[var->counter].command = item_data[monster_data[var->battler[var->counter].chara_id].equip[0]].waza[0];
	var->battler[var->counter].weppon = monster_data[var->battler[var->counter].chara_id].equip[0];
	var->counter++;
}

//���x�����p
void swap_battler(BATTLER* a, BATTLER* b) {
	BATTLER temp = *a;
	*a = *b;
	*b = temp;
}

//���x�\�[�g
void sort_battler(TCB *thisTCB) {
	TYPE_BATTLE* var = (TYPE_BATTLE*)thisTCB->Work;
	int i = 0, j = 0;
	//�o�u���\�[�g
	for (i = 0; i < BATTLERSNUM; i++) {
		for (j = BATTLERSNUM - 1; j > i; j--) {
			//�������ɏ������Ȃ�悤��
			if (var->battler[j].speed > var->battler[j - 1].speed) {
				//�X���b�v
				int temp = var->battler[j].turn;
				var->battler[j].turn = var->battler[j - 1].turn;
				var->battler[j - 1].turn = temp;
			}
		}
	}
}

//�_���[�W�v�Z�֐�
int acter_damage_cul(STATUS st, ITEM weppon, WAZA_DATA waza) {
	int lv = st.lv;
	int status;
	int wep = weppon.seinou;
	int c = waza.seityou;
	//�ˑ��\�͂𒲂ׂ�
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
	//�����x���e�����Ă��Ȃ�
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
	//�����X�^�[�Ɛl�𒲂ׂ�
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
	//�\�͒l�R�s�[
	if (atk_is_monster) {
		//�ŏI�I�Ɍ��݃X�e�[�^�X���g��
		act_st = monster_data[atk_monster_id].st;
	}
	else {
		act_st = battler_chara[atk_party_id].cur_st;
	}
	if (def_is_monster) {
		//�ŏI�I�Ɍ��݃X�e�[�^�X���g��
		def_st = monster_data[def_monster_id].st;
	}
	else {
		def_st = battler_chara[def_party_id].cur_st;
	}
	//����f�[�^
	weppon = item_data[var_parent->battler[acter].weppon];
	//�Z�̎Q�Ƃ𒲂ׂ�
	//�Z�̃^�C�v�𒲂ׂ�
	//�ȗ�
	int damege = 0;
	//�f�_���v�Z
	damege = max(0, acter_damage_cul(act_st, weppon, waza));
	//�h��v�Z
	int i, def = 0, prevdef = 0;
	for (i = 0; i < ZOKUSEI; i++) {
		unsigned char bit = 0x01 << i;
		if (waza.zokusei & bit) {
			//�������O�̖h��͂��Ⴏ���
			if (prevdef == 0 || def < prevdef) {
				if (def_is_monster) {
					//������������
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
	//�D��x:0x2301
	//�ϐ��錾
	BATTLE_ACT* var = (BATTLE_ACT*)thisTCB->Work;
	TYPE_BATTLE* var_parent = (TYPE_BATTLE*)thisTCB->Parent->Work;
	int d;
	if (var->childTask == NULL || var->childTask->Flag != _USE) {
		switch (var->act_enum) {
		case MESS:
			//���b�Z�[�W
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0100, thisTCB);
			if (var->acter >= (BATTLERSNUM - MAXMONSTER)) {
				//�G�s��
				sprintfDx((char*)(var->childTask->Work), "%s�̍s���I", monster_data[var_parent->battler[var->acter].chara_id].name);
			}
			else {
				sprintfDx((char*)(var->childTask->Work), "%s�̍s���I", battler_chara[var->acter].name);
			}
			var->act_enum = EFF;
			if (var_parent->battler[var->acter].command == COMM_DEF) {
				if (var->acter >= (BATTLERSNUM - MAXMONSTER)) {
					//�G�s��
					sprintfDx((char*)(var->childTask->Work), "%s�͐g��������I", monster_data[var_parent->battler[var->acter].chara_id].name);
				}
				else {
					sprintfDx((char*)(var->childTask->Work), "%s�͐g��������I", battler_chara[var->acter].name);
				}
				var->act_enum = END_DM;
			}
			break;
		case EFF:
			//�G�t�F�N�g
			var->act_enum = DAMEGE;
			break;
		case DAMEGE:
			//�_���[�W
			d = damege_clu(thisTCB);
			//���ӁI���̂܂܂ł͔\�͒l��HP�̕ω����킩��Ȃ�
			//�f�[�^�\���̌������𑪂�
			//�Ƃ肠����HP����BATTLER�ɒǉ�
			if (waza_data[var_parent->battler[var->defecer].command].type == SKILL_DEF) {
				d = d / 2;
			}
			battler_chara[var->defecer].cur_st.hp = max(0, (battler_chara[var->defecer].cur_st.hp - d));
			//���b�Z�[�W
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0100, thisTCB);
			if (var->defecer >= (BATTLERSNUM - MAXMONSTER)) {
				//�G�s��
				sprintfDx((char*)(var->childTask->Work), "%s�� %d�̃_���[�W�I", monster_data[var_parent->battler[var->defecer].chara_id].name, d);
			}
			else {
				sprintfDx((char*)(var->childTask->Work), "%s�� %d�̃_���[�W�I", battler_chara[var->defecer].name, d);
			}
			var->act_enum = DEATH;
			break;
		case DEATH:
			//���S�m�F
			if (battler_chara[var->defecer].cur_st.hp == 0) {
				var->childTask = TaskMake(message, thisTCB->Prio + 0x0100, thisTCB);
				if (var->defecer >= (BATTLERSNUM - MAXMONSTER)) {
					//�G�s��
					sprintfDx((char*)(var->childTask->Work), "%s��|�����I", monster_data[var_parent->battler[var->defecer].chara_id].name);
				}
				else {
					sprintfDx((char*)(var->childTask->Work), "%s�͓|�ꂽ�c�c\n�������A���͊J�����Ȃ̂ŕ�������!", battler_chara[var->defecer].name);
				}
				var_parent->battler[var->defecer].chara_id = -1;
			}
			var->act_enum = END_DM;
			break;
		default:
			//�J�E���^����
			var_parent->counter++;
			TaskKill(thisTCB);
			break;
		}
	}

}

void event_battle(TCB* thisTCB) {
	//�D��x:0x2300
	TYPE_BATTLE* var = (TYPE_BATTLE*)thisTCB->Work;
	int i = 0;
	unsigned int ikiteru = 0;
	if (var->childTask == NULL || var->childTask->Flag != _USE) {
		switch (var->flow) {
		case BATTLE_END:
			//�v���C���[�ƃ����X�^�[�A�ǂ��炪���񂾂�����
			//�������ꍇ������험�i�A�������ꍇ�Q�[���I�[�o�[��
			//�e�X�̐����t���O
			ikiteru = 0;
			for (i = 0; i < BATTLERSNUM; i++) {
				if (var->battler[i].chara_id > NO_EXSIT) {
					ikiteru = ikiteru | 1 << i;
				}
			}
			if (!(ikiteru & 0xff)) {
				//systeminfo.game_over = 1;
				//�Q�[���I�[�o�[��������₱�����̂ŁA���͐����Ԃ�
				battler_chara[0].cur_st.hp = battler_chara[0].bas_st.hp;
				//������悤�ɂ���
				systeminfo.canmove = 1;
				TaskKill(thisTCB);
			}
			else if (!(ikiteru & 0xff00)) {
				//������悤�ɂ���
				systeminfo.canmove = 1;
				TaskKill(thisTCB);
			}
			else {
				var->flow = SETUP;
			}
			break;
		case BATTLE_START:
			//�����X�^�[���̕\��
			//���̓��C�������X�^�[���̕\��
			var->childTask = TaskMake(message, thisTCB->Prio + 0x0001, thisTCB);
			sprintfDx((char*)(var->childTask->Work), "%s���@���ꂽ�I\n", monster_data[var->battler[BATTLERSNUM - MAXMONSTER].chara_id].name);
			var->flow = SETUP;
			break;
		case SETUP:
			//�Z�b�g�A�b�v�^�X�N�����
			var->flow = PLAYER_COMM;
			break;
		case PLAYER_COMM:
			//�����c��̖���������΁A�����̍s��������s��
			//�����łȂ���΁A�G�s������^�X�N�𐧍�
			if (var->counter < (BATTLERSNUM - MAXMONSTER)) {
				//�J�E���^�̓��[�v�ϐ�
				if (var->battler[var->counter].chara_id > NO_EXSIT) {
					//�v���C���[�L���������݂��鐔�����^�X�N�����
					var->childTask = TaskMake(battle_player_command, thisTCB->Prio + 0x0100, thisTCB);
					TYPE_P_COMMAND* var_child = (TYPE_P_COMMAND*)var->childTask->Work;
					var_child->who = var->counter;
					var_child->equip = 0;
					var_child->flow = WHAT;
				}
				else {
					//���Ȃ��Ȃ��΂�
					var->counter++;
				}
			}
			else {
				//�G�s������^�X�N��
				//�J�E���^�͂��̂܂�
				var->flow = ENEMY_COMM;
			}
			break;
		case ENEMY_COMM:
			//�S�Č��肵�Ă���΁A�s���������
			//�����łȂ���Ύ��̓G�s������
			if (var->battler[var->counter].chara_id > NO_EXSIT) {
				//�v���C���[�L���������݂��鐔�����^�X�N�����
				enemy_command(thisTCB);
			}
			else {
				//���Ȃ��Ȃ��΂�
				var->counter++;
			}
			if (var->counter >= BATTLERSNUM) {
				var->flow = SORT;
				//�J�E���^�̏�����
				var->counter = 0;
			}
			break;
		case SORT:
			//�G������id���s���l���\�[�g
			//���̌�s��������
			//�t�H�[���_�E��(break�Ȃ�)������H
			sort_battler(thisTCB);
			var->flow = ACT;
			break;
		case ACT:
			//�\�[�g�������ԂɋZ���s��
			//�Zid�A�ΏہA�s�׎�id�����K�v�H
			//�s�ׂ��ƂɑS�ł𔻒�
			//�f�o�b�O�^�X�N�}��
			//�s���ґ{��
			for (i = 0; i < BATTLERSNUM; i++) {
				if (var->battler[i].turn == var->counter) {
					break;
				}
			}
			//�^�[���I������
			if (i >= BATTLERSNUM) {
				var->flow = TURNEND;
				var->counter = 0;
				break;
			}
			//�����łȂ��A�Ȃ����s�׎҂����݂���΃^�X�N����
			else if (var->battler[i].chara_id > NO_EXSIT) {
				var->childTask = TaskMake(battle_act, thisTCB->Prio + 0x0100, thisTCB);
				BATTLE_ACT* var_child2 = (BATTLE_ACT*)var->childTask->Work;
				var_child2->act_enum = MESS;
				var_child2->acter = i;
				var_child2->defecer = var->battler[i].taisyou;
				//�Ώەs�݂̎��͔�΂�
				if (var->battler[var_child2->defecer].chara_id == NO_EXSIT) {
					TaskKill(var->childTask);
					var->counter++;
				}
			}
			else {
				//�J�E���^����
				var->counter++;
			}
			break;
		case TURNEND:
			//���̑�����
			//�f�o�b�O�̈א퓬�I��������
			var->flow = BATTLE_END;
			break;
		}
	}

	//�����X�^�[�摜�\��
	int count_monster = 0;
	for (i = BATTLERSNUM-1; i > BATTLERSNUM - MAXMONSTER-1; i--) {
		if (var->battler[i].chara_id > NO_EXSIT) {
			DrawRotaGraph(192+64*count_monster, 240, 4.0, 0, monster_img[var->battler[i].chara_id], TRUE);
			count_monster++;
		}
	}

	//���Ŕ���
	if (PTaskDead(thisTCB)) {
		TaskKill(thisTCB);
	}
}

void init_event_battle(TCB* thisTCB) {
	TYPE_BATTLE* var = (TYPE_BATTLE*)thisTCB->Work;
	int i, j, k;
	for (i = 0; i < BATTLERSNUM; i++) {
		//���Ԃ̑��݂ɉ����Ē��Ԃ̃f�[�^���(turn0~7)
		//�I�������烂���X�^�[�f�[�^���(8~15)
		//�Ґ��f�[�^���K�o�K�o�Ȃ̂Œ���
		//����8�ȏ�Ȃ�C�x���g�o�g������
		if ((BATTLERSNUM - i) <= MAXMONSTER) {
			if (var->group_id < HENSEI_P) {
				//�O���[�vid���Ґ��p�^�[������艺���ʏ�G���J�E���g
				var->battler[i].chara_id = hensei[playerpos.nowfloor][var->group_id].id[i - MAXMONSTER];
				if (var->battler[i].chara_id > NO_EXSIT) {
					battler_chara[i].cur_st.hp = monster_data[var->battler[i].chara_id].st.hp;
					//����͂̍X�V
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
				//�O���[�vid���Ґ��p�^�[�����ȏと�C�x���g�o�g��(�������̈׍��͓K����id����)
				var->battler[i].chara_id = hensei[playerpos.nowfloor][var->group_id % 8].id[i - MAXMONSTER];
			}
		}
		else {
			if (i < PARTY_NINZU) {
				if (battler_chara[i].bas_st.hp < 1) {
					//���X�e�[�^�XHP��0�ȉ��̃L�����͑��݂��Ȃ����̂Ƃ���
					var->battler[i].chara_id = -1;
				}
				else {
					//�L����id��3�o�C�g�ڂɏ�������
					var->battler[i].chara_id = i << 16;
					var->battler[i].st_data = &battler_chara[i];
					//����͂̍X�V
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
				//���͂��Ȃ��ł�
				var->battler[i].chara_id = -1;
			}
		}
		var->battler[i].turn = i;
	}
	//�J�E���^��������()
	var->counter = 0;
	TaskChange(thisTCB, event_battle);
}