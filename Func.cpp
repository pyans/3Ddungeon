#include "Task.h"
#include "3Ddungeon.h"

TCB Task[MAX_TASK_COUNT];

static int   g_SCENE_CHANGE_COMM;				//�V�[���J�ڗp
unsigned int PadInputBuff[INPUTBUFFLAME];		//�{�^�����͎擾�@�ȉ������l
unsigned int ONInputBuff;
unsigned int OFFInputBuff;
unsigned int WordCr = 0xffffff;

void KeyControll(TCB* thisTCB) {
	//�L�[���͎擾
	int i;
	for (i = INPUTBUFFLAME-1; i > 0; i--) {
		//����!:�K���Â��o�b�t�@����X�V�ɂ����邱��
		PadInputBuff[i] = PadInputBuff[i - 1];
	}
	PadInputBuff[0] = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	ONInputBuff = (PadInputBuff[0] & ~PadInputBuff[1]);	//Inputbuf: not		-> pushed
	OFFInputBuff = (~PadInputBuff[0] & PadInputBuff[1]);	//Inputbuf: pushed	-> not
}

//�V�X�e����� func.cpp/line:26
SYS_INFO systeminfo = { 1,0,0 };
#define STRMAX 512								//�Œ����b�Z�[�W��

/*****************
******************
*�C�x���g�p�֐��Q*
******************
******************/
//���b�Z�[�W�^ func.cpp/line 34
typedef struct {
	char mes[STRMAX];
}T_MESSAGE;

//�V�X�e�����b�Z�[�W
typedef struct {
	POS pos;
	char mes[STRMAX];
}T_CMESSAGE;

//�����摜�o�b�t�@
int cmesbuf[128];
//����
#define ZIKAN_X 14
#define ZIKAN_Y 16
void char_message(TCB *thisTCB) {
	T_CMESSAGE *var = (T_CMESSAGE*)thisTCB->Work;
	int i;
	int line = 0, col = 0;
	//����HP�\���Ȃ̂�(��ł�����ƒ�������)
	sprintfDx(var->mes, "HP:%d/%d\nMP:%d/%d", party[0].hp, party[0].st.maxhp, party[0].mp, party[0].st.maxmp);
	for (i = 0; i < strlen(var->mes); i++) {
		char temp = var->mes[i];
		//���s
		if ( temp== '\n') {
			line++;
			col = 0;
		}
		//�\���ł��镶���̂ݕ\������
		if ((temp > '!') && (temp < 'Z')) {
			DrawRotaGraph(var->pos.pos_x + ZIKAN_X*col, var->pos.pos_y + line*ZIKAN_Y, 2.0, 0,cmesbuf[temp], TRUE, FALSE);
			col++;
		}
	}
	if (PTaskDead(thisTCB)) {
		TaskKill(thisTCB);
	}
}

void message(TCB *thisTCB) {					//���b�Z�[�W�\��
	T_MESSAGE *var = (T_MESSAGE*)thisTCB->Work;
	//���b�Z�[�W�̕\��
	DrawFormatString(32, 376, WordCr,var->mes);
	if (ONInputBuff & PAD_INPUT_1) {			//A�{�^���ɔ�������OFF
		TaskKill(thisTCB);
	}
}

//�O���t�B�b�N�`��֐�
typedef struct TYPE_0F {
	POS pos;
	double ex_rate;
	double angle;
	int imagebuf;
}DRAW;

void draw_something(TCB* thisTCB) {
	DRAW* var = (DRAW*)thisTCB->Work;
	DrawRotaGraph(var->pos.pos_x, var->pos.pos_y, var->ex_rate, var->angle, var->imagebuf, TRUE);
	if (PTaskDead(thisTCB)) {
		TaskKill(thisTCB);
	}
}

FLOORMAP dungeonmap[LEVEL];						//�_���W�����}�b�v

//�����̏��
PLAYER_INFO info;								//�l�X�ȏ��
CHARA party[PARTY_NINZU] = {};					//���C�������o�[
CHARA sub_member[16];								//����Ȃǂɑҋ@������T�u�����o�[

int poscheck(POS a, POS b) {
	//�|�W�V������v
	if (a.pos_x == b.pos_x && a.pos_y == b.pos_y) {
		//��v�Ȃ�1��
		return 1;
	}
	else {
		//�s��v�Ȃ�0��
		return 0;
	}
}

void encount(TCB* thisTCB) {
	systeminfo.encount += GetRand(12)+4;
#ifdef DEBUG
	printfDx("ENCOUNTER:%d\n",systeminfo.encount);
#endif
	//�G���J�E���g����
	//���̊m���ŒB�������G���J�E���g����ɂ���ăG���J�E���g�l������
	//256�ȏ�Ő퓬����
	if (systeminfo.encount >= 256) {
		systeminfo.encount = 0;
		systeminfo.canmove = 0;
		TCB *task = TaskMake(init_event_battle, thisTCB->Prio + 0x0100, thisTCB->Parent);
		//�Ґ���7�����A�g
		((TYPE_BATTLE*)task->Work)->group_id = (GetRand(255) / 36);
	}
}

int compase_buf;
void compase(TCB* thisTCB) {
	//�R���p�X�\���^�X�N
	DrawRotaGraph(368, 128, 1.0, (-(int)playerpos.dir)*PI / 2, compase_buf, TRUE);
}

//��l��
PLAYER_POS playerpos;		//�v���C���[�̈ʒu�ƌ����A���݃t���A

//�P�\�t���[��:
#define YUYOFLAME 8

//�w�肳�ꂽ���W�̕ǔ���(��l���Ƃ̑��΍��W)
int soutai_col(int xplus, int yplus) {
	//�w�肳�ꂽ���W�̕ǔ���(��l���Ƃ̑��΍��W)
	int x = playerpos.pos.pos_x + xplus;		//�I�[�o�[�t���[����
	int y = playerpos.pos.pos_y + yplus;		//����
	if (x < 0 || x >= MAPSIZE)return -1;
	if (y < 0 || y >= MAPSIZE)return -1;
	int a = dungeonmap[playerpos.nowfloor].map[x][y];		//�L���X�g����
	return a;
}

void move(TCB *thisTCB) {
	//�ړ�
	//8�t���[���A�����͂������H(�������̂ŃJ�b�g)
	int Input8buf = 0x00000000;
	int tempdir = (int)playerpos.dir;
	//for (i = 0; i < YUYOFLAME; i++) {
	//	Input8buf = Input8buf & PadInputBuff[i];
	//}

	//�����ύX
	if ((ONInputBuff | Input8buf) & PAD_INPUT_LEFT) {
		tempdir = (int)(playerpos.dir) - 1;			//��������
	}
	if ((ONInputBuff | Input8buf) & PAD_INPUT_RIGHT) {
		tempdir = (int)(playerpos.dir) + 1;			//�E������
	}
	if ((ONInputBuff | Input8buf) & PAD_INPUT_DOWN) {
		tempdir = (int)(playerpos.dir) + 2;			//�U��Ԃ�Γz������
	}
	//��������
	if (tempdir < 0)
	{
		playerpos.dir = WEST;					//-1�Ȃ琼����
	}
	else {
		playerpos.dir = (DIR)(tempdir % 4);		//DIR�^�ɕϊ�
	}

	if ((ONInputBuff | Input8buf) & PAD_INPUT_UP) {
		//����O�i
		switch (playerpos.dir)
		{
		case NORTH:
			if (soutai_col(0, -1) == SPACE) {
				playerpos.pos.pos_y--;			//�k�ֈ��
			}
			break;
		case EAST:
			if (soutai_col(1, 0) == SPACE) {
				playerpos.pos.pos_x++;			//���ֈ��
			}
			break;
		case SOUTH:
			if (soutai_col(0, 1) == SPACE) {
				playerpos.pos.pos_y++;			//��ֈ��
			}
			break;
		case WEST:
			if (soutai_col(-1, 0) == SPACE) {
				playerpos.pos.pos_x--;			//���ֈ��
			}
			break;
		default:
			break;
		}
		//�f�o�b�O���
#ifdef DEBUG
		printfDx("POS:x=%d,y=%d\ndir=%d\n", playerpos.pos.pos_x, playerpos.pos.pos_y, (int)(playerpos.dir));
#endif
		//�C�x���g�����`�F�b�N
		TaskMake(event_boot, thisTCB->Prio + 0x0100,thisTCB->Parent);
	}
}

void exec_hero(TCB* thisTCB) {
	//���s
	//�ړ��A���j���[�J�Ȃ�
	if (systeminfo.canmove) {
		move(thisTCB);
		//���j���[����(�f�o�b�O��)
		if (ONInputBuff & PAD_INPUT_4) {
			systeminfo.canmove = 0;
			TaskMake(menu_equip,0x2500,thisTCB);
		}
	}
	if (!(thisTCB->Parent->Flag & _USE)) {
		//�V�[���I���ƂƂ��ɏ���(parent = exec03_gamescene)
		TaskKill(thisTCB);
	}
}

void init_hero(TCB* thisTCB) {
	//������:���͉����Ȃ�
	//exec�ɕύX
	TaskChange(thisTCB, exec_hero);
}

//���ꂢ��H�����ꂢ��(�|�̈ӎv)
#define WALL_RIGHT 2
#define WALL_CENTER 1
#define WALL_LEFT 0

//�}�b�v�`��
#define MINI_MS 4				//�~�j�}�b�v�T�C�Y
int wallbuf[MINI_MS][3];								//�ǃO��[�[��][����]

void exec_mapdraw(TCB* thisTCB) {
	DrawRotaGraph(320, 240, 2.0, 0, dungeonmap[playerpos.nowfloor].backbuf, TRUE, FALSE);
	int i, j;				//i:dapth,j:dir
	int minimap[MINI_MS][3];//�Ǐ��̕ۑ�
	for (i = 0; i < MINI_MS; i++) {
		for (j = 0; j < 3; j++) {
			/*
			*|0123456789A
			-+-----------
			0|****3  ****
			1|****2  ****
			2|****1  ****
			3|****012****
			4|   2***0123
			5|   1***1
			6|3210***2
			7|****210****
			8|****  1****
			9|****  2****
			A|****  3****

			�[�������\(���ꂢ��H)
			*/
			switch (playerpos.dir) {
			case NORTH:
				//x:������/���E,y:�t����/�[��
				minimap[i][j] = soutai_col(j - 1, -i);
				break;
			case EAST:
				//x:������/�[��,y:������/���E
				minimap[i][j] = soutai_col(i,j - 1);
				break;
			case SOUTH:
				//x:�t����/���E,y:������/�[��
				minimap[i][j] = soutai_col(1 - j, i);
				break;
			case WEST:
				//x:�t����/�[��,y:�t����/���E
				minimap[i][j] = soutai_col(-i, 1 - j);
				break;
			}
		}
	}

	for (i = MINI_MS-1; i >= 0; i--) {
		//����:������ǂ��������� ��O���珑���ƔߎS
		//����Ȃ钍��:�^�񒆂̕ǂ��Ō�ɏ�������
		if (minimap[i][WALL_LEFT] == WALL) {
			DrawRotaGraph(320, 240, 2.0, 0, wallbuf[i][WALL_LEFT], TRUE, FALSE);
		}
		if (minimap[i][WALL_RIGHT] == WALL) {
			DrawRotaGraph(320, 240, 2.0, 0, wallbuf[i][WALL_RIGHT], TRUE, FALSE);
		}
		if (minimap[i][WALL_CENTER] == WALL) {
			DrawRotaGraph(320, 240, 2.0, 0, wallbuf[i][WALL_CENTER], TRUE, FALSE);
		}
	}
	if (PTaskDead(thisTCB)) {
		//�V�[���I���ƂƂ��ɏ���(parent = exec03_gamescene)
		TaskKill(thisTCB);
	}
}

void init_mapdraw(TCB* thisTCB) {
	//�摜�Ǐo���͏�����TotalInit�Ɉړ�
	TaskChange(thisTCB, exec_mapdraw);
}

typedef struct {
	int         SceneID;
	int         Time;
} EX3_SCENE_STRUCT;

#define SCENE_INIT          -1    //������
#define SCENE_CHANGE_WAIT   0     //�؂�ւ����̑҂����
#define SCENE_MOVIE			1	  //OP���[�r�[
#define SCENE_TITLECALL		2	  //�^�C�g���R�[��
#define SCENE_TITLE         3     //�^�C�g���V�[��
#define SCENE_DEMO			4	  //�f���V�[��
#define SCENE_GAME          5     //�Q�[����
#define SCENE_GAME_OVER     6     //�Q�[���I�[�o�[
#define SCENE_OPTION        7     //�I�v�V�������
#define SCENE_END           8     //�V�[�����I��������

/*���ڂ�����*/
/*SCENE_CHABGE_WAIT�̓V�[���ɑ΂��ē��ɐ؂�ւ����߂��o�Ă��Ȃ���ԁB*/
/*SCENE_CHANGE_COMM�̒l�����ꂩ��ς�邱�Ƃɂ����*/
/*exec03_SCENE_CTRL�ɂ���ăV�[�����؂�ւ�����*/

void exec03_SCENE_GAME(TCB* thisTCB)
{
	EX3_SCENE_STRUCT* pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	//��莞�Ԃ��Ƃɓ_�ł��J��Ԃ�
	pEX3_SCENEwork->Time++;

	//�Q�[���V�[��

	//�V�[���̐؂�ւ����V�[���Ǘ��^�X�N�ɓ`����
	if (systeminfo.game_over == 1)
	{
		g_SCENE_CHANGE_COMM = SCENE_END;
		TaskKill(thisTCB);
		return;
	}
}

void init03_SCENE_GAME(TCB* thisTCB) {
	EX3_SCENE_STRUCT* pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;
	//�Q�[���V�[��������

	//�v���C���[�̈ʒu�̏�����(�Z�[�u����������͕ύX�\��)
	//�Ƃ肠�������͂��ꂪ�f�t�H���g
	playerpos = { {30,29},WEST };
	event_init();
	//�v���C��[�X�e�[�^�X�̒���
	party[0] = {"�v���C���[", {1,120,36,18,23,20,16,15,21},120,36,{0,0,0,0,0,0,0,0},{ 0x41,0xff,0xff,0xff,0x80,0xa0,0xb0,0xff } };
	//�A�C�e���f�[�^�̏�����
	info.item[0] = 0x50;
	info.item[1] = 0x60;
	int i, j;
	//�h��͎Z�o
	for (i = 4; i < EQUIPSIZE; i++) {
		for (j = 0; j < ZOKUSEI; j++) {
			party[0].def[j] += item_data[party[0].equip[i]].def[j];
		}
	}
	//�K�v�ȃ^�X�N����
	TaskMake(init_hero, 0x2100, thisTCB);
	TaskMake(init_mapdraw, 0x2200, thisTCB);
	TaskMake(compase, 0x2500, thisTCB);
	//�v���C���[���\��
	TCB* childTask = TaskMake(char_message, 0x2300, thisTCB);
	T_CMESSAGE* var = (T_CMESSAGE*)childTask->Work;
	sprintfDx(var->mes, "HP:%d/%d\nMP:%d/%d", party[0].hp,party[0].st.maxhp, party[0].mp, party[0].st.maxmp);
	var->pos = { 448,48 };
	TaskChange(thisTCB, exec03_SCENE_GAME);
}

void exec03_SCENE_GAME_OVER(TCB* thisTCB)
{
	EX3_SCENE_STRUCT* pEX3_SCENEwork;
	pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	//��莞�Ԃ��Ƃɓ_�ł��J��Ԃ�
	pEX3_SCENEwork->Time++;
	if (pEX3_SCENEwork->Time & 0x10)
	{
		DrawFormatString(0, 0, WordCr, "GAME OVER SCENE\n");
	}

	//�Q�[���I�[�o�[�V�[��

	//�V�[���̐؂�ւ����V�[���Ǘ��^�X�N�ɓ`����
	if (ONInputBuff & PAD_INPUT_1)
	{
		g_SCENE_CHANGE_COMM = SCENE_GAME;    //�Q�[���𑱍s����
		TaskKill(thisTCB);
		return;
	}
	if (ONInputBuff & PAD_INPUT_2)
	{
		g_SCENE_CHANGE_COMM = SCENE_TITLECALL;  //�^�C�g���֖߂�
		TaskKill(thisTCB);
		return;
	}
}

void exec03_SCENE_OPTION(TCB* thisTCB)
{
#define RANK_MAX 5
	EX3_SCENE_STRUCT* pEX3_SCENEwork;
	pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	//�����ɃI�v�V������������

	//�V�[���̐؂�ւ����V�[���Ǘ��^�X�N�ɓ`����
	if (ONInputBuff & PAD_INPUT_2)
	{
		g_SCENE_CHANGE_COMM = SCENE_END;
		TaskKill(thisTCB);
		return;
	}
}

void exec03_SCENE_TITLECALL(TCB* thisTCB) {
	//�^�C�g�����[�r�[
	//���͎��󂷂邾��
	g_SCENE_CHANGE_COMM = SCENE_TITLE;
	TaskKill(thisTCB);
	return;
}

void init03_SCENE_TITLECALL(TCB* thisTCB) {
	//�^�C�g���f��������
	//���͕ς�邾��
	TaskChange(thisTCB, exec03_SCENE_TITLECALL);
}

void exec03_SCENE_TITLE(TCB* thisTCB)
{
	EX3_SCENE_STRUCT* pEX3_SCENEwork;
	pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	DrawFormatString(128, 144, WordCr, "BASIC 3D DUNGEON\n�̌���");

	//��莞�Ԃ��Ƃɓ_�ł��J��Ԃ�
	pEX3_SCENEwork->Time++;
	if (pEX3_SCENEwork->Time & 0x10)
	{
		DrawFormatString(128, 288, WordCr, "PRESS A BUTTON");
	}

	//�V�[���̐؂�ւ����V�[���Ǘ��^�X�N�ɓ`����
	if (ONInputBuff & PAD_INPUT_1)
	{
		//�Q�[����I��
		g_SCENE_CHANGE_COMM = SCENE_GAME;
		TaskKill(thisTCB);
		return;
	}
}

void init03_SCENE_CTRL(TCB* thisTCB)
{
	EX3_SCENE_STRUCT* pEX3_SCENEwork;
	pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	//���������̏��
	pEX3_SCENEwork->SceneID = SCENE_INIT;

}

void exec03_SCENE_CTRL(TCB* thisTCB)
{
	EX3_SCENE_STRUCT* pEX3_SCENEwork;
	pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	switch (pEX3_SCENEwork->SceneID)
	{
	case SCENE_INIT: //��������
		pEX3_SCENEwork->SceneID = SCENE_TITLECALL;
		TaskMake(exec03_SCENE_TITLECALL, 0x2000);
		g_SCENE_CHANGE_COMM = SCENE_CHANGE_WAIT;
		break;
	case SCENE_TITLECALL:
		if (g_SCENE_CHANGE_COMM != SCENE_CHANGE_WAIT)
		{
			pEX3_SCENEwork->SceneID = SCENE_TITLE;
			TaskMake(exec03_SCENE_TITLE, 0x2000);
			g_SCENE_CHANGE_COMM = SCENE_CHANGE_WAIT;
		}

		break;
	case SCENE_TITLE: //�^�C�g�����
		if (g_SCENE_CHANGE_COMM != SCENE_CHANGE_WAIT)
		{
			if (g_SCENE_CHANGE_COMM == SCENE_GAME)
			{
				TaskMake(init03_SCENE_GAME, 0x2000);
				pEX3_SCENEwork->SceneID = SCENE_GAME;
			}
			else
				if (g_SCENE_CHANGE_COMM == SCENE_OPTION)
				{
					TaskMake(exec03_SCENE_OPTION, 0x2000);
					pEX3_SCENEwork->SceneID = SCENE_OPTION;
				}
			g_SCENE_CHANGE_COMM = SCENE_CHANGE_WAIT;
		}
		break;
	case SCENE_GAME: //�Q�[������
		if (g_SCENE_CHANGE_COMM != SCENE_CHANGE_WAIT)
		{
			if (g_SCENE_CHANGE_COMM == SCENE_END) {
				pEX3_SCENEwork->SceneID = SCENE_GAME_OVER;
				TaskMake(exec03_SCENE_GAME_OVER, 0x2000);
				g_SCENE_CHANGE_COMM = SCENE_CHANGE_WAIT;
			}
		}
		break;
	case SCENE_GAME_OVER:  //�Q�[���I�[�o�[����
		if (g_SCENE_CHANGE_COMM != SCENE_CHANGE_WAIT)
		{
			if (g_SCENE_CHANGE_COMM == SCENE_GAME)
			{
				pEX3_SCENEwork->SceneID = SCENE_GAME;
				TaskMake(init03_SCENE_GAME, 0x2000);
			}
			else
				if (g_SCENE_CHANGE_COMM == SCENE_TITLECALL)
				{
					pEX3_SCENEwork->SceneID = SCENE_TITLECALL;
					TaskMake(init03_SCENE_TITLECALL, 0x2000);
				}
			g_SCENE_CHANGE_COMM = SCENE_CHANGE_WAIT;
		}
		break;
	case SCENE_OPTION:  //�I�v�V�������
		if (g_SCENE_CHANGE_COMM != SCENE_CHANGE_WAIT)
		{
			pEX3_SCENEwork->SceneID = SCENE_TITLECALL;
			TaskMake(init03_SCENE_TITLECALL, 0x2000);
			g_SCENE_CHANGE_COMM = SCENE_CHANGE_WAIT;
		}
		break;
	case SCENE_DEMO:
		if (g_SCENE_CHANGE_COMM != SCENE_CHANGE_WAIT) {
			pEX3_SCENEwork->SceneID = SCENE_GAME_OVER;
			TaskMake(exec03_SCENE_GAME_OVER, 0x2000);
			g_SCENE_CHANGE_COMM = SCENE_CHANGE_WAIT;
		}
		break;

	}
}

void TotalInit(TCB* thisTCB) {
	//****�摜�Ǐo��****
	//!!!!�댯!!!!
	//�^�X�N�������邽�тɓǂݏo���̂Ŋ댯
	//�ŏI�I�ɑ����������ŌĂяo����
	//�K�v�ɉ����ă��������J������ׂ�

	//�w�i���[�h
	dungeonmap[playerpos.nowfloor].backbuf = LoadGraph("./image/dungeon_temp3.bmp");
	//�ǉ摜���[�h
	int i, j;
	char name[256];
	//��0����1�E2�ɂȂ����̂ŕύX
	char* dir[3] = { "left","center","right" };
	//4*3�~�j�}�b�v�ׁ̈A�摜��12���K�v
	//4*3�ɂȂ邩�����Ȃ���
	for (i = 0; i < MINI_MS; i++) {
		for (j = 0; j < 3; j++) {
			//���O�C���v�b�g
			snprintfDx(name, 256, "./image/wall/wall_%d_%s.bmp", i, dir[j]);
			//���[�h
			wallbuf[i][j] = LoadGraph(name);
		}
	}
	//�����X�^�[�摜���[�h
	for (i = 0; i < MONSTERNUM; i++) {
		//HP0�̃����X�^�[�͑��݂��Ȃ����̂Ƃ���
		if (monster_data[i].st.maxhp != 0) {
			snprintfDx(name, 256, "./image/monster/monster_%d.bmp", i);
			monster_img[i] = LoadGraph(name);
		}
	}
	//�}�b�v���[�h
	//�e�X�g�Ȃ̂�1�K�̂�
	for (i = 0; i < 1; i++) {
		snprintfDx(name,256, "./map/floor_%d.dat", i);
		int filehandle = FileRead_open(name);
		if (filehandle == 0) {
			//�G���[����
			break;
		}
		FileRead_read(dungeonmap[i].map, sizeof(char)*MAPSIZE*MAPSIZE, filehandle);
		FileRead_close(filehandle);
	}
	//�A�C�e���f�[�^������
	init_item();
	for (i = 0; i < ITEMMAX; i++) {
		info.item[i] = 0xff;
	}
	//�t�H���g�ݒ�
	//�������t�H���g�̓f�[�^�z�z��(�����I�ɂ��m���I�ɂ�)�߂�ǂ��̂ł��Ȃ�����
	SetFontSize(FONTSIZE);
	ChangeFont("JF�h�b�gjiskan16s-1990");
	//�V�X�e�������摜�ǂݍ���
	snprintfDx(name,256, "./image/word/datawords.bmp");
	LoadDivGraph(name, 128, 16, 8, 8, 8, cmesbuf);
	//�R���p�X�摜�ǂݍ���
	snprintfDx(name, 256, "./image/etc/compase.bmp");
	compase_buf = LoadGraph(name);
	//�������I��
	init03_SCENE_CTRL(thisTCB);
	TaskChange(thisTCB, exec03_SCENE_CTRL);
}

void TaskHead00(TCB* thisTCB) {
	//�L�[���͏���
	KeyControll(thisTCB);
}

void TaskHead(TCB* thisTCB) {
	TaskMake(TotalInit, 0x1000);
	TaskChange(thisTCB, TaskHead00);
}