#pragma once
#include "3Ddungeon.h"

//~~~~~�\����~~~~~~

//�C�x���g�^
typedef struct TYPE_04 {
	int id;
	int imagebuf;
	POS eventPOS;
	unsigned char condition[16];
	unsigned char code[CODEMAX];
}EVENT;

//�I����
typedef struct TYPE_15 {
	int* pointer;
	int cursor;
}QUESTION;

//�C�x���g���s�^�X�N�p
typedef struct {
	int event_id;	//�i�s���̃C�x���gid
	int ix;			//�C���f�b�N�X���W�X�^
	int id;			//�Ԃ��Ă����I��
	TCB* childTask;	//�q�^�X�N�ւ̃|�C���^
}T_EVENT_CONTROL;

//�֐��Q
extern void event_moneyplus(TCB *thisTCB);
extern void event_itemchange(TCB *thisTCB);
extern void event_question(TCB* thisTCB);
extern void event_boot(TCB *thisTCB);
extern int event_init(void);

//�ϐ�
extern EVENT gameevent[LEVEL][EVENT_IN_A_FLOOR];	//�C�x���g�f�[�^���̂���