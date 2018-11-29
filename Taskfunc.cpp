#include "Task.h"

void InitTask(void) {
	TCB* tmpTCB;

	ZeroMemory(Task, sizeof(TCB)*MAX_TASK_COUNT);

	tmpTCB = Task;

	tmpTCB->Exec = TaskHead;

	tmpTCB->Prio = 0x0000;

	tmpTCB->Next = Task;

	tmpTCB->Flag = _USE;
}

void TaskExec(void) {
	TCB* execTCB;

	execTCB = Task;
	do {
		execTCB->Exec(execTCB);
		execTCB = execTCB->Next;
	} while (execTCB->Prio != 0x0000);
}

TCB* TaskMake(void(*exec)(TCB*), unsigned int prio, TCB* parent) {
	TCB* newTCB;
	TCB* prevTCB;
	TCB* nextTCB;
	int id;

	for (id = 0; id < MAX_TASK_COUNT; id++) {
		if (!(Task[id].Flag & _USE))break;
	}

	if (id == MAX_TASK_COUNT)return NULL;

	newTCB = &Task[id];

	prevTCB = Task;

	while (prevTCB->Next->Prio != 0x0000) {
		if (prevTCB->Next->Prio > prio)break;
		prevTCB = prevTCB->Next;
	}
	nextTCB = prevTCB->Next;

	ZeroMemory(newTCB, sizeof(TCB));

	newTCB->Exec = exec;
	newTCB->Prio = prio;
	newTCB->Parent = parent;

	newTCB->Prev = prevTCB;
	newTCB->Next = nextTCB;
	prevTCB->Next = newTCB;
	nextTCB->Prev = newTCB;

	newTCB->Flag = _USE;

	return newTCB;
}

void TaskKill(TCB* killTCB) {
	killTCB->Prev->Next = killTCB->Next;
	killTCB->Next->Prev = killTCB->Prev;

	killTCB->Flag = 0;
}

void TaskChange(TCB* chgTCB, void(*nextexec)(TCB*)) {
	chgTCB->Exec = nextexec;
}

int PTaskDead(TCB* thisTCB) {
	if (thisTCB->Parent != NULL && (thisTCB->Parent->Flag & _USE)) {
		return 0;
	}
	else {
		return 1;
	}
}
