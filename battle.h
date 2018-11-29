#pragma once
#include "3Ddungeon.h"
//�o�g���ɕK�v�ȍ\����

typedef enum ENUM_01 {
	BATTLE_END = -1,
	BATTLE_START = 0,
	SETUP = 1,
	PLAYER_COMM = 2,
	ENEMY_COMM = 3,
	SORT = 4,
	ACT = 5,
	TURNEND = 6
}BATTLE_FLOW;

//�퓬�҂̃f�[�^
#define NO_EXSIT -1	//�L����id��-1�̃L�����͑��݂��Ȃ�
//�n�b�V���̓��A0~3�܂ł͎�l�������ŌŒ�
#define HERO_1 0
#define HERO_2 1
#define HERO_3 2
#define HERO_4 3

typedef struct TYPE_0C_A {
	int turn;
	int chara_id;
	int weppon;
	int command;
	int taisyou;
	int speed;
	int hp;
}BATTLER;

#define BATTLERSNUM 16		//�ő�퓬�Q���l��
//�퓬�p�\����
typedef struct TYPE_0C {
	int group_id;
	BATTLER battler[BATTLERSNUM];
	char winner;
	BATTLE_FLOW flow;
	TCB* childTask;
	int counter;
}TYPE_BATTLE;

//�퓬�_���[�W�p
typedef enum ENUM_03 {
	END_DM = -1,
	MESS = 0,
	EFF = 1,
	DAMEGE = 2,
	DEATH = 3
}ACT_ENUM;

typedef struct TYPE_11 {
	ACT_ENUM act_enum;
	TCB* childTask;
	int acter;
	int defecer;
}BATTLE_ACT;

#define CHOOSE_0_X 48
#define CHOOSE_0_Y 376

typedef enum ENUM_02 {
	COMMAND_END = -1,
	WHAT = 0,
	WHOM = 1
}COMMAND_FLOW;

typedef struct TYPE_0A {
	int who;			//�s�׎�
	int whom;			//�Ώ�
	int waza_id;		//�g�p�Z
	int equip;			//�����ʒu
	COMMAND_FLOW flow;
	TCB* childTask;
}TYPE_P_COMMAND;

#define SENTAKU_X 2
#define SENTAKU_Y 4

//�֐��Q
extern void battle_player_command(TCB *thisTCB);
extern void enemy_command(TCB* thisTCB);
extern void swap_battler(BATTLER* a, BATTLER* b);
extern void sort_battler(TCB *thisTCB);
extern int acter_damage_cul(STATUS st, ITEM weppon, WAZA_DATA waza);
extern int defencer_damage_cul(STATUS st, int def, int su_damege);
extern int damege_clu(TCB* thisTCB);
extern int damege_clu(TCB* thisTCB);
extern void battle_act(TCB* thisTCB);
extern void battle_choose(TCB* thisTCB);
extern void event_battle(TCB* thisTCB);
extern void init_event_battle(TCB* thisTCB);