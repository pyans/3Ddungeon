#pragma once

//����n�R�[�h
#define NOSIGNAL 0x00
#define EVENTCODE 0x01
#define START 0x02
#define IF_ZERO 0x08
#define IF_NOZERO 0x09
#define IF_NEGA 0x0A
#define GOTO 0x0e
#define END 0x0E
#define ENDEVENT 0x0F

//�ėp�C�x���g�Ăяo��
#define MES 0x10 
#define EXEC 0x20
#define EFFECT 0x30
#define CHOOSE 0xb0
#define ACSESS 0xC0

//�C�x���g�Ƃ��č��\��
#define BATTLE 0x40
#define GETITEM 0x50
#define TREASURE 0x60
#define GETMONEY 0x70
#define LOSTMONEY 0x80		//�����ŏ�������\��
#define WARP 0x90
#define HEAL 0xA0