#pragma once
#include "dxlib.h"
#include "Task.h"
#include "CODE.h"
#include "other.h"
#include "event.h"
#include "battle.h"

//�ϐ��錾
extern EVENT gameevent[LEVEL][EVENT_IN_A_FLOOR];		//�C�x���g�f�[�^
extern const char* message_text[LEVEL][MESNUMMAX];		//���b�Z�[�W�̃f�[�^
extern int monster_img[MONSTERNUM];						//�����X�^�[�̃C���[�W�o�b�t�@
extern MONSTER_DATA monster_data[MONSTERNUM];			//�����X�^�[�f�[�^
extern const M_GROUP hensei[LEVEL][HENSEI_P];			//�G�Ґ��f�[�^
extern const WAZA_DATA waza_data[WAZAMAX];				//�Z�f�[�^
extern ITEM item_data[ITEMNUM];							//�A�C�e���̃f�[�^
extern PLAYER_POS playerpos;							//�v���C���[�ʒu���
extern CHARA party[PARTY_NINZU];						//�p�[�e�B���
extern SYS_INFO systeminfo;								//�V�X�e�����
extern PLAYER_INFO info;								//�v���C���[���