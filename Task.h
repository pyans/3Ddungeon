#pragma once
#define TASK_WORK_SIZE 1024
#define MAX_TASK_COUNT 256
#define _USE (1<<0)

//�^�X�N�p�\����
typedef struct TYPE_00 {
	void(*Exec)(TYPE_00*);					//���s�֐��ւ̃|�C���^
	TYPE_00* Prev;							//�O�^�X�N�ւ̃|�C���^
	TYPE_00* Next;							//���^�X�N�ւ̃|�C���^
	TYPE_00* Parent;						//�e�^�X�N�ւ̃|�C���^
	unsigned int Flag;						//�g�p���t���O
	unsigned int Prio;						//�D��x
	int Imagebuf;							//�O���t�B�b�N�o�b�t�@
	int Work[(TASK_WORK_SIZE + 3) / 4];		//�f�[�^�̈�(�\���̂�����Ė������L���X�g)
}TCB;

#ifndef TASK_INC_DONE
#define TASK_INC_DONE
#define _CRT_SECURE_NO_WARNINGS TRUE		//�댯�Ȋ֐��̏���
#include "Dxlib.h"

//�^�X�N�֘A�֐�
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
