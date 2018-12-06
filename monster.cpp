#include "3Ddungeon.h"

IMAGE_SIZE SIZE32 = { 32,32 };
IMAGE_SIZE SIZE64 = { 64,64 };
IMAGE_SIZE SIZE96 = { 96,96 };
IMAGE_SIZE SIZE128 = { 128,128 };
IMAGE_SIZE SIZE160 = { 160,160 };
int monster_img[MONSTERNUM];
MONSTER_DATA monster_data[MONSTERNUM] = {
	{
		//ID No.0
		"�X�P���g��",	//���O
		//�X�e�[�^�X
		{
			/*Lv*/1,
			/*HP*/90,
			/*MP*/30,
			/*STR*/6,
			/*AGI*/2,
			/*MUS*/8,
			/*POW*/4,
			/*WIL*/2,
			/*CHR*/3
		},
		SIZE32,			//�T�C�Y
		//����
		{0x41,0xff,0xff,0xff,0x81,0xff,0xff,0xff},
		0x00,			//�s���p�^�[��
		5,				//EXP(�㏸���x��)
		10,				//�S�[���h
		{0x00,0x00},	//�h���b�v
		4				//�X�L�����x��
	},

	{
		//ID No.1
		"�E",	//���O
		//�X�e�[�^�X
		{
			/*Lv*/2,
			/*HP*/160,
			/*MP*/18,
			/*STR*/3,
			/*AGI*/16,
			/*MUS*/4,
			/*POW*/5,
			/*WIL*/5,
			/*CHR*/8
		},
		SIZE32,			//�T�C�Y
		//����
		{ 0x42,0xff,0xff,0xff,0x80,0xff,0xff,0xff },
		0x00,			//�s���p�^�[��
		5,				//EXP(�㏸���x��)
		10,				//�S�[���h
		{ 0x00,0x00 },	//�h���b�v
		3				//�X�L�����x��
	}
};