#include "other.h"

ITEM item_data[ITEMNUM];
void init_item() {
	item_data[0x40] = {
		"�u���[�h�\�[�h",	//���O
		EQUIP_WEP,			//��������
		8,					//���퐫�\
		1,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{},					//�h���
		{0,0x09,-1,-1,-1}		//�Z
	};
	item_data[0x41] = {
		"�����O�\�[�h",	//���O
		EQUIP_WEP,			//��������
		11,					//���퐫�\
		2,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{},					//�h���
		{ 0,0x09,0x0A,0x0B,-1 }		//�Z
	};
	item_data[0x42] = {
		"�e��",	//���O
		EQUIP_WEP,			//��������
		9,					//���퐫�\
		1,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{},					//�h���
		{ 0,0x09,0x0A,-1,-1 }		//�Z
	};
	item_data[0x4f] = {
		"���x��",	//���O
		EQUIP_WEP,			//��������
		77,					//���퐫�\
		1,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{},					//�h���
		{ 0,0x0C,0x0A,-1,-1 }		//�Z
	};

	item_data[0x50] = {
		"�n���h�A�b�N�X",
		EQUIP_HANDS,
		10,
		4,					//�d��
		EQUIP_NOGURAD,
		{},
		{ 4,0x0B,-1,-1,-1 }
	};
	item_data[0x60] = {
		"�|��",
		EQUIP_WEP,
		8,
		2,
		EQUIP_NOGURAD,
		{},
		{2,-1,-1,-1,-1 }
	};
	item_data[0x70] = {
		"���|",				//���O
		EQUIP_HANDS,		//��������
		9,					//���퐫�\
		1,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{},					//�h���
		{ 3,-1,-1,-1,-1 }		//�Z
	};
	item_data[0x80] = {
		"��Z",	//���O
		EQUIP_BODY,			//��������
		6,					//���퐫�\
		3,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{6,12,6,6,12,12,6,6},					//�h���
		{ -1,-1,-1,-1,-1 }		//�Z
	};
	item_data[0x81] = {
		"�{�[���v���X�g",	//���O
		EQUIP_BODY,			//��������
		13,					//���퐫�\
		2,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{ 13,4,13,13,4,13,13,13},					//�h���
		{ -1,-1,-1,-1,-1 }		//�Z
	};
	item_data[0x8f] = {
		"���ق̊Z",	//���O
		EQUIP_BODY,			//��������
		35,					//���퐫�\
		2,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{ 35,35,35,35,45,45,25,25 },					//�h���
		{ -1,-1,-1,-1,-1 }		//�Z
	};
	item_data[0x90] = {
		"�X�q",	//���O
		EQUIP_HEAD,			//��������
		3,					//���퐫�\
		0,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{3,3,3,3,3,3,3,3},					//�h���
		{ -1,-1,-1,-1,-1 }		//�Z
	};
	item_data[0xA0] = {
		"����",	//���O
		EQUIP_ARMS,			//��������
		2,					//���퐫�\
		0,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{2,2,2,2,2,10,2,2},					//�h���
		{ -1,-1,-1,-1,-1 }		//�Z
	};
	item_data[0xB0] = {
		"�t�����e�B�A�̃u�[�c",	//���O
		EQUIP_REGS,			//��������
		4,					//���퐫�\
		1,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{4,4,4,4,4,4,4,4},					//�h���
		{ -1,-1,-1,-1,-1 }		//�Z
	};
	item_data[0xC0] = {
		"�t�F�C�X�X�[�c",	//���O
		EQUIP_FULL,			//��������
		25,					//���퐫�\
		10,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{25,42,25,25,18,18,9,18},					//�h���
		{ -1,-1,-1,-1,-1 }		//�Z
	};
	//��
	item_data[0xFF] = {
		"",	//���O
		EQUIP_SOME,			//��������
		0,					//���퐫�\
		0,					//�d��
		EQUIP_NOGURAD,		//�ϐ�
		{},					//�h���
		{ -1,-1,-1,-1,-1 }		//�Z
	};
}