#pragma once

#include "CODE.h"			//�ǂݍ���
#include "Task.h"

//#define DEBUG				//�f�o�b�O

#define PI 3.14156			//�G�t�F�N�g����
#define LEVEL 16			//�K��
#define MAPSIZE 32			//�}�b�v�T�C�Y
#define INPUTBUFFLAME 16	//�A�����͂̋L�����ǂꂾ�����邩
#define FONTSIZE 20			//�t�H���g�T�C�Y
#define CODEMAX 128			//�C�x���g�R�[�h�̒���
#define MESNUMMAX 256		//���b�Z�[�W�̍ő吔
#define PARTY_NINZU 4		//�p�[�e�B�l��
#define ITEMMAX 64			//�����A�C�e���ő吔
#define EQUIPSIZE 8			//�����ꏊ��
#define NAMEMAX 16			//���O������(�}���`�o�C�g�����̉\����)
#define WAZAMAX 256			//���U�ő吔(�K��)
#define ZOKUSEI 8			//�a���ˎ˔M��d��

//FLOORMAP:�}�b�v�Ɣw�i�o�b�t�@(�����邩��)
#define WALL 1
#define SPACE 0
typedef struct TYPE_01 {
	int backbuf;
	char map[MAPSIZE][MAPSIZE];
}FLOORMAP;

//POS:�l�X�ȃI�u�W�F�N�g�̍��W(Z�͂܂������Ȃ�)
typedef struct TYPE_02 {
	int pos_x;
	int pos_y;
}POS;

//�����̗񋓑�
typedef enum ENUM_00 {
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
}DIR;

//PLAYER_POS:�v���C���[�ʒu���̊i�[
typedef struct TYPE_03 {
	POS pos;
	DIR dir;
	int nowfloor;								//������K
}PLAYER_POS;

//�C�x���g�f�[�^
#define EVENT_IN_A_FLOOR 32	//1�K������̃C�x���g���ݐ�
//���݃t���O�n
#define EVENT_EXSIT 0x01
#define EVENT_NULL	0x00

//�R�[�h�ʒu�p
#define EVENT_ISEXSIT 0		//�C�x���g�����݂��邩�ǂ���

//�S�̏��
typedef struct TYPE_05 {
	int money;
	int maxfloor;
	int item[ITEMMAX];
}PLAYER_INFO;

//�X�e�[�^�X�f�[�^(�T���v��)
typedef struct TYPE_06 {
	int lv;
	int maxhp;
	int maxmp;
	int str;
	int agi;
	int mus;
	int pow;
	int wil;
	int chr;
}STATUS;

//�L�����f�[�^(�T���v��)
typedef struct TYPE_07 {
	char name[NAMEMAX];
	STATUS st;
	int hp;
	int mp;
	int def[ZOKUSEI];
	int equip[EQUIPSIZE];
}CHARA;

//���j���[��퓬�̑I��
#define CHOOSEMAX 8
#define WAZALENGTH 64
typedef struct TYPE_EX01 {
	int num;
	int choice;
	int id[CHOOSEMAX];
	int *ret_p;
	char text[CHOOSEMAX][WAZALENGTH];
}SENTAKU;

//�A�C�e���f�[�^(�T���v��)
#define SOUBIWAZA 5			//�����Z��
#define ITEMNUM 256			//�A�C�e���f�[�^��
#define ITEMNAME 32			//�A�C�e�����̒���
//������(�Ƒ�������)
#define EQUIP_WEP 0x01
#define EQUIP_SHI 0x02
#define EQUIP_HANDS (EQUIP_WEP | EQUIP_SHI)
#define EQUIP_ITEM 0x04
#define EQUIP_CLOTH 0x08
#define EQUIP_BODY 0x10
#define EQUIP_HEAD 0x20
#define EQUIP_ARMS 0x40
#define EQUIP_REGS 0x80
#define EQUIP_FULL 0xF0
#define EQUIP_SOME 0x00
//�����ϐ�
#define EQUIP_NOGURAD 0x00
typedef struct TYPE_08 {
	char name[ITEMNAME];	//���O
	unsigned char equip_flags;		//�ǂ��ɑ����ł��邩
	int seinou;				//���\
	int zyuryou;			//�d��
	int taisei;				//�����ϐ�
	int def[ZOKUSEI];		//�������Ƃ̖h���
	int waza[SOUBIWAZA];	//�ŗL�Z
}ITEM;

//�����X�^�[���̃T�C�Y
typedef struct TYPE_0D {
	int x;
	int y;
}IMAGE_SIZE;

//�����X�^�[�f�[�^
#define MONSETR_NAME 32
typedef struct TYPE_0E {
	char name[MONSETR_NAME];
	STATUS st;
	IMAGE_SIZE size;
	int equip[EQUIPSIZE];
	int koudou;
	int exp;				//�Ȃ����㏸���x��
	int gold;				//�S�[���h
	int drop[2];			//�h���b�v�A�C�e��:�m�[�}���ƃ��A
	int skill;				//�X�L�����x���Ŕ��肷��ꍇ
}MONSTER_DATA;

//�o���p�^�[���f�[�^
#define MAXMONSTER 8		//�o�������X�^�[��
#define MONSTERNUM 256		//�����X�^�[��
#define HENSEI_P 8			//�G�Ґ��p�^�[����
typedef struct TYPE_0B {
	//0�͑��݂��Ȃ����Ƃ�\��
	int id[MAXMONSTER];		//�����X�^�[id
}M_GROUP;

//�Z�f�[�^
#define WAZA_NAME 32
//�ˑ��\�͒l
#define IZON_STR 0
#define IZON_AGI 1
#define IZON_MUS 2
#define IZON_POW 3
#define IZON_WIL 4
#define IZON_CHR 5
//�U������
#define ATK_SLASH 0x01
#define ATK_HIT 0x02
#define ATK_SPEAR 0x04
#define ATK_SHOOT 0x08
#define ATK_HEAT 0x10
#define ATK_ICE 0x20
#define ATK_THUNDER 0x40
#define ATK_POIZUN 0x80
#define ATK_ENERGY 0x00
//����t���O
#define ATKSP_NOFLUGS 0x0000
//�Z�̃^�C�v
#define SKILL_ATK 0x01
#define SKILL_HEAL 0x10
#define SPELL_ATK 0x20
#define SKILL_DEF 0x40
#define ITEM_HEAL 0x80
//����
#define COMM_DEF 0x07
typedef struct TYPE_10 {
	char name[WAZA_NAME];	//�Z��
	int iryoku;				//�З�
	int seityou;			//�����x
	int meityu;				//����
	short cost;				//�R�X�g
	short izon;				//�ˑ��\�͒l
	short zokusei;			//�U������
	int special_flug;		//���U�Ȃǂ̓���t���O
	short type;				//�Z�̃^�C�v(�U���A�񕜁A��Ԉُ퓙)
}WAZA_DATA;

//�V�X�e�����
typedef struct TYPE_09 {
	int canmove;
	int cursor;
	int encount;
	int game_over;
}SYS_INFO;

//�ȉ�:	��	��	��	��
extern unsigned int PadInputBuff[INPUTBUFFLAME];
extern unsigned int ONInputBuff;
extern unsigned int OFFInputBuff;
extern unsigned int WordCr;
//�񋓌^�ϊ��p
#define STR(var) #var

//�֐��錾
extern int soutai_col(int xplus, int yplus);			//�ړ������̑��΍��W�v�Z
extern int event_init();								//�C�x���g������
extern void init_item();								//�A�C�e��������
extern void message(TCB *thisTCB);
extern void encount(TCB* thisTCB);
extern int poscheck(POS a, POS b);