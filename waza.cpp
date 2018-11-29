#include "3Ddungeon.h"

const WAZA_DATA waza_data[WAZAMAX] = {
	{
		//id_0
		"��",			//���O
		36,				//�З�
		3,				//�����x
		240,			//����
		0,				//�R�X�g
		IZON_STR,		//�ˑ��\�͒l
		ATK_SLASH,		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_ATK		//�Z�^�C�v
	},

	{
		//id_1
		"���_",			//���O
		50,				//�З�
		4,				//�����x
		216,			//����
		0,				//�R�X�g
		IZON_STR,		//�ˑ��\�͒l
		ATK_HIT,		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_ATK		//�Z�^�C�v
	},

	{
		//id_2
		"��",			//���O
		30,				//�З�
		3,				//�����x
		248,			//����
		0,				//�R�X�g
		IZON_STR,		//�ˑ��\�͒l
		ATK_SPEAR,		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_ATK		//�Z�^�C�v
	},

	{
		//id_3
		"�|",			//���O
		32,				//�З�
		2,				//�����x
		232,			//����
		0,				//�R�X�g
		IZON_WIL,		//�ˑ��\�͒l
		ATK_SHOOT,		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_ATK		//�Z�^�C�v
	},

	{
		//id_4
		"��",			//���O
		50,				//�З�
		4,				//�����x
		208,			//����
		0,				//�R�X�g
		IZON_STR,		//�ˑ��\�͒l
		(ATK_SLASH | ATK_HIT),		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_ATK		//�Z�^�C�v
	},

	{
		//id_5
	},

	{
		//id_6
	},

	{
		//id_7
		"�g�����",			//���O
		0,			//�З�
		0,				//�����x
		255,			//����
		0,				//�R�X�g
		IZON_STR,		//�ˑ��\�͒l
		ATK_ENERGY,		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_DEF		//�Z�^�C�v
	},

	{
		//id_8
		"����",			//���O
		100,			//�З�
		0,				//�����x
		255,			//����
		0,				//�R�X�g
		IZON_STR,		//�ˑ��\�͒l
		ATK_ENERGY,		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_HEAL		//�Z�^�C�v
	},

	{
		//id_9
		"�j�i�؂�",			//���O
		100,			//�З�
		2,				//�����x
		255,			//����
		2,				//�R�X�g
		IZON_STR,		//�ˑ��\�͒l
		ATK_SLASH,		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_ATK		//�Z�^�C�v
	},

	{
		//id_A
		"�U���؂�",			//���O
		160,			//�З�
		5,				//�����x
		224,			//����
		2,				//�R�X�g
		IZON_STR,		//�ˑ��\�͒l
		ATK_SLASH,		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_ATK		//�Z�^�C�v
	},

	{
		//id_B
		"������",		//���O
		200,			//�З�
		5,				//�����x
		224,			//����
		3,				//�R�X�g
		IZON_STR,		//�ˑ��\�͒l
		(ATK_SLASH | ATK_HIT),		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_ATK		//�Z�^�C�v
	},

	{
		//id_C
		"�����卪�؂�",		//���O
		2400,			//�З�
		15,				//�����x
		255,			//����
		13,				//�R�X�g
		IZON_STR,		//�ˑ��\�͒l
		ATK_SLASH ,		//�^�C�v
		ATKSP_NOFLUGS,	//����t���O
		SKILL_ATK		//�Z�^�C�v
	},
};