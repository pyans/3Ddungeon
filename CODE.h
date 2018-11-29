#pragma once

//制御系コード
#define NOSIGNAL 0x00
#define EVENTCODE 0x01
#define START 0x02
#define IF_ZERO 0x08
#define IF_NOZERO 0x09
#define IF_NEGA 0x0A
#define GOTO 0x0e
#define END 0x0E
#define ENDEVENT 0x0F

//汎用イベント呼び出し
#define MES 0x10 
#define EXEC 0x20
#define EFFECT 0x30
#define CHOOSE 0xb0
#define ACSESS 0xC0

//イベントとして作る可能性
#define BATTLE 0x40
#define GETITEM 0x50
#define TREASURE 0x60
#define GETMONEY 0x70
#define LOSTMONEY 0x80		//負数で処理する可能性
#define WARP 0x90
#define HEAL 0xA0