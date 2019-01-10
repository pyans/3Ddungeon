#include "Task.h"
#include "3Ddungeon.h"

TCB Task[MAX_TASK_COUNT];

static int   g_SCENE_CHANGE_COMM;				//シーン遷移用
unsigned int PadInputBuff[INPUTBUFFLAME];		//ボタン入力取得　以下も同様
unsigned int ONInputBuff;
unsigned int OFFInputBuff;
unsigned int WordCr = 0xffffff;

void KeyControll(TCB* thisTCB) {
	//キー入力取得
	int i;
	for (i = INPUTBUFFLAME-1; i > 0; i--) {
		//注意!:必ず古いバッファから更新にかけること
		PadInputBuff[i] = PadInputBuff[i - 1];
	}
	PadInputBuff[0] = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	ONInputBuff = (PadInputBuff[0] & ~PadInputBuff[1]);	//Inputbuf: not		-> pushed
	OFFInputBuff = (~PadInputBuff[0] & PadInputBuff[1]);	//Inputbuf: pushed	-> not
}

//システム情報 func.cpp/line:26
SYS_INFO systeminfo = { 1,0,0 };
#define STRMAX 512								//最長メッセージ数

/*****************
******************
*イベント用関数群*
******************
******************/
//メッセージ型 func.cpp/line 34
typedef struct {
	char mes[STRMAX];
}T_MESSAGE;

//システムメッセージ
typedef struct {
	POS pos;
	char mes[STRMAX];
}T_CMESSAGE;

//文字画像バッファ
int cmesbuf[128];
//字間
#define ZIKAN_X 14
#define ZIKAN_Y 16
void char_message(TCB *thisTCB) {
	T_CMESSAGE *var = (T_CMESSAGE*)thisTCB->Work;
	int i;
	int line = 0, col = 0;
	//今はHP表示なので(後できちんと直すこと)
	sprintfDx(var->mes, "HP:%d/%d\nMP:%d/%d", party[0].hp, party[0].st.maxhp, party[0].mp, party[0].st.maxmp);
	for (i = 0; i < strlen(var->mes); i++) {
		char temp = var->mes[i];
		//改行
		if ( temp== '\n') {
			line++;
			col = 0;
		}
		//表示できる文字のみ表示する
		if ((temp > '!') && (temp < 'Z')) {
			DrawRotaGraph(var->pos.pos_x + ZIKAN_X*col, var->pos.pos_y + line*ZIKAN_Y, 2.0, 0,cmesbuf[temp], TRUE, FALSE);
			col++;
		}
	}
	if (PTaskDead(thisTCB)) {
		TaskKill(thisTCB);
	}
}

void message(TCB *thisTCB) {					//メッセージ表示
	T_MESSAGE *var = (T_MESSAGE*)thisTCB->Work;
	//メッセージの表示
	DrawFormatString(32, 376, WordCr,var->mes);
	if (ONInputBuff & PAD_INPUT_1) {			//Aボタンに反応してOFF
		TaskKill(thisTCB);
	}
}

//グラフィック描画関数
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

FLOORMAP dungeonmap[LEVEL];						//ダンジョンマップ

//味方の情報
PLAYER_INFO info;								//様々な情報
CHARA party[PARTY_NINZU] = {};					//メインメンバー
CHARA sub_member[16];								//酒場などに待機させるサブメンバー

int poscheck(POS a, POS b) {
	//ポジション一致
	if (a.pos_x == b.pos_x && a.pos_y == b.pos_y) {
		//一致なら1を
		return 1;
	}
	else {
		//不一致なら0を
		return 0;
	}
}

void encount(TCB* thisTCB) {
	systeminfo.encount += GetRand(12)+4;
#ifdef DEBUG
	printfDx("ENCOUNTER:%d\n",systeminfo.encount);
#endif
	//エンカウント処理
	//一定の確率で達成されるエンカウント判定によってエンカウント値が増加
	//256以上で戦闘発生
	if (systeminfo.encount >= 256) {
		systeminfo.encount = 0;
		systeminfo.canmove = 0;
		TCB *task = TaskMake(init_event_battle, thisTCB->Prio + 0x0100, thisTCB->Parent);
		//編成は7がレア枠
		((TYPE_BATTLE*)task->Work)->group_id = (GetRand(255) / 36);
	}
}

int compase_buf;
void compase(TCB* thisTCB) {
	//コンパス表示タスク
	DrawRotaGraph(368, 128, 1.0, (-(int)playerpos.dir)*PI / 2, compase_buf, TRUE);
}

//主人公
PLAYER_POS playerpos;		//プレイヤーの位置と向き、現在フロア

//猶予フレーム:
#define YUYOFLAME 8

//指定された座標の壁判定(主人公との相対座標)
int soutai_col(int xplus, int yplus) {
	//指定された座標の壁判定(主人公との相対座標)
	int x = playerpos.pos.pos_x + xplus;		//オーバーフロー注意
	int y = playerpos.pos.pos_y + yplus;		//同上
	if (x < 0 || x >= MAPSIZE)return -1;
	if (y < 0 || y >= MAPSIZE)return -1;
	int a = dungeonmap[playerpos.nowfloor].map[x][y];		//キャスト注意
	return a;
}

void move(TCB *thisTCB) {
	//移動
	//8フレーム連続入力したか？(うざいのでカット)
	int Input8buf = 0x00000000;
	int tempdir = (int)playerpos.dir;
	//for (i = 0; i < YUYOFLAME; i++) {
	//	Input8buf = Input8buf & PadInputBuff[i];
	//}

	//向き変更
	if ((ONInputBuff | Input8buf) & PAD_INPUT_LEFT) {
		tempdir = (int)(playerpos.dir) - 1;			//左を向く
	}
	if ((ONInputBuff | Input8buf) & PAD_INPUT_RIGHT) {
		tempdir = (int)(playerpos.dir) + 1;			//右を向く
	}
	if ((ONInputBuff | Input8buf) & PAD_INPUT_DOWN) {
		tempdir = (int)(playerpos.dir) + 2;			//振り返れば奴がいる
	}
	//向きを代入
	if (tempdir < 0)
	{
		playerpos.dir = WEST;					//-1なら西向き
	}
	else {
		playerpos.dir = (DIR)(tempdir % 4);		//DIR型に変換
	}

	if ((ONInputBuff | Input8buf) & PAD_INPUT_UP) {
		//一歩前進
		switch (playerpos.dir)
		{
		case NORTH:
			if (soutai_col(0, -1) == SPACE) {
				playerpos.pos.pos_y--;			//北へ一歩
			}
			break;
		case EAST:
			if (soutai_col(1, 0) == SPACE) {
				playerpos.pos.pos_x++;			//東へ一歩
			}
			break;
		case SOUTH:
			if (soutai_col(0, 1) == SPACE) {
				playerpos.pos.pos_y++;			//南へ一歩
			}
			break;
		case WEST:
			if (soutai_col(-1, 0) == SPACE) {
				playerpos.pos.pos_x--;			//西へ一歩
			}
			break;
		default:
			break;
		}
		//デバッグ情報
#ifdef DEBUG
		printfDx("POS:x=%d,y=%d\ndir=%d\n", playerpos.pos.pos_x, playerpos.pos.pos_y, (int)(playerpos.dir));
#endif
		//イベント発生チェック
		TaskMake(event_boot, thisTCB->Prio + 0x0100,thisTCB->Parent);
	}
}

void exec_hero(TCB* thisTCB) {
	//実行
	//移動、メニュー開閉など
	if (systeminfo.canmove) {
		move(thisTCB);
		//メニュー処理(デバッグ中)
		if (ONInputBuff & PAD_INPUT_4) {
			systeminfo.canmove = 0;
			TaskMake(menu_equip,0x2500,thisTCB);
		}
	}
	if (!(thisTCB->Parent->Flag & _USE)) {
		//シーン終了とともに消滅(parent = exec03_gamescene)
		TaskKill(thisTCB);
	}
}

void init_hero(TCB* thisTCB) {
	//初期化:今は何もない
	//execに変更
	TaskChange(thisTCB, exec_hero);
}

//これいる？←これいる(鋼の意思)
#define WALL_RIGHT 2
#define WALL_CENTER 1
#define WALL_LEFT 0

//マップ描画
#define MINI_MS 4				//ミニマップサイズ
int wallbuf[MINI_MS][3];								//壁グラ[深さ][方向]

void exec_mapdraw(TCB* thisTCB) {
	DrawRotaGraph(320, 240, 2.0, 0, dungeonmap[playerpos.nowfloor].backbuf, TRUE, FALSE);
	int i, j;				//i:dapth,j:dir
	int minimap[MINI_MS][3];//壁情報の保存
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

			深さ早見表(これいる？)
			*/
			switch (playerpos.dir) {
			case NORTH:
				//x:順方向/左右,y:逆方向/深さ
				minimap[i][j] = soutai_col(j - 1, -i);
				break;
			case EAST:
				//x:順方向/深さ,y:順方向/左右
				minimap[i][j] = soutai_col(i,j - 1);
				break;
			case SOUTH:
				//x:逆方向/左右,y:順方向/深さ
				minimap[i][j] = soutai_col(1 - j, i);
				break;
			case WEST:
				//x:逆方向/深さ,y:逆方向/左右
				minimap[i][j] = soutai_col(-i, 1 - j);
				break;
			}
		}
	}

	for (i = MINI_MS-1; i >= 0; i--) {
		//注意:奥から壁を書くこと 手前から書くと悲惨
		//さらなる注意:真ん中の壁を最後に書くこと
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
		//シーン終了とともに消滅(parent = exec03_gamescene)
		TaskKill(thisTCB);
	}
}

void init_mapdraw(TCB* thisTCB) {
	//画像読出しは初期化TotalInitに移動
	TaskChange(thisTCB, exec_mapdraw);
}

typedef struct {
	int         SceneID;
	int         Time;
} EX3_SCENE_STRUCT;

#define SCENE_INIT          -1    //初期化
#define SCENE_CHANGE_WAIT   0     //切り替え時の待ち状態
#define SCENE_MOVIE			1	  //OPムービー
#define SCENE_TITLECALL		2	  //タイトルコール
#define SCENE_TITLE         3     //タイトルシーン
#define SCENE_DEMO			4	  //デモシーン
#define SCENE_GAME          5     //ゲーム中
#define SCENE_GAME_OVER     6     //ゲームオーバー
#define SCENE_OPTION        7     //オプション画面
#define SCENE_END           8     //シーンが終了した時

/*おぼえがき*/
/*SCENE_CHABGE_WAITはシーンに対して特に切り替え命令が出ていない状態。*/
/*SCENE_CHANGE_COMMの値がこれから変わることによって*/
/*exec03_SCENE_CTRLによってシーンが切り替えられる*/

void exec03_SCENE_GAME(TCB* thisTCB)
{
	EX3_SCENE_STRUCT* pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	//一定時間ごとに点滅を繰り返す
	pEX3_SCENEwork->Time++;

	//ゲームシーン

	//シーンの切り替えをシーン管理タスクに伝える
	if (systeminfo.game_over == 1)
	{
		g_SCENE_CHANGE_COMM = SCENE_END;
		TaskKill(thisTCB);
		return;
	}
}

void init03_SCENE_GAME(TCB* thisTCB) {
	EX3_SCENE_STRUCT* pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;
	//ゲームシーン初期化

	//プレイヤーの位置の初期化(セーブ処理実装後は変更予定)
	//とりあえず今はこれがデフォルト
	playerpos = { {30,29},WEST };
	event_init();
	//プレイやーステータスの調整
	party[0] = {"プレイヤー", {1,120,36,18,23,20,16,15,21},120,36,{0,0,0,0,0,0,0,0},{ 0x41,0xff,0xff,0xff,0x80,0xa0,0xb0,0xff } };
	//アイテムデータの初期化
	info.item[0] = 0x50;
	info.item[1] = 0x60;
	int i, j;
	//防御力算出
	for (i = 4; i < EQUIPSIZE; i++) {
		for (j = 0; j < ZOKUSEI; j++) {
			party[0].def[j] += item_data[party[0].equip[i]].def[j];
		}
	}
	//必要なタスク制作
	TaskMake(init_hero, 0x2100, thisTCB);
	TaskMake(init_mapdraw, 0x2200, thisTCB);
	TaskMake(compase, 0x2500, thisTCB);
	//プレイヤー名表示
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

	//一定時間ごとに点滅を繰り返す
	pEX3_SCENEwork->Time++;
	if (pEX3_SCENEwork->Time & 0x10)
	{
		DrawFormatString(0, 0, WordCr, "GAME OVER SCENE\n");
	}

	//ゲームオーバーシーン

	//シーンの切り替えをシーン管理タスクに伝える
	if (ONInputBuff & PAD_INPUT_1)
	{
		g_SCENE_CHANGE_COMM = SCENE_GAME;    //ゲームを続行する
		TaskKill(thisTCB);
		return;
	}
	if (ONInputBuff & PAD_INPUT_2)
	{
		g_SCENE_CHANGE_COMM = SCENE_TITLECALL;  //タイトルへ戻る
		TaskKill(thisTCB);
		return;
	}
}

void exec03_SCENE_OPTION(TCB* thisTCB)
{
#define RANK_MAX 5
	EX3_SCENE_STRUCT* pEX3_SCENEwork;
	pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	//ここにオプションを書こう

	//シーンの切り替えをシーン管理タスクに伝える
	if (ONInputBuff & PAD_INPUT_2)
	{
		g_SCENE_CHANGE_COMM = SCENE_END;
		TaskKill(thisTCB);
		return;
	}
}

void exec03_SCENE_TITLECALL(TCB* thisTCB) {
	//タイトルムービー
	//今は自壊するだけ
	g_SCENE_CHANGE_COMM = SCENE_TITLE;
	TaskKill(thisTCB);
	return;
}

void init03_SCENE_TITLECALL(TCB* thisTCB) {
	//タイトルデモ初期化
	//今は変わるだけ
	TaskChange(thisTCB, exec03_SCENE_TITLECALL);
}

void exec03_SCENE_TITLE(TCB* thisTCB)
{
	EX3_SCENE_STRUCT* pEX3_SCENEwork;
	pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	DrawFormatString(128, 144, WordCr, "BASIC 3D DUNGEON\n体験版");

	//一定時間ごとに点滅を繰り返す
	pEX3_SCENEwork->Time++;
	if (pEX3_SCENEwork->Time & 0x10)
	{
		DrawFormatString(128, 288, WordCr, "PRESS A BUTTON");
	}

	//シーンの切り替えをシーン管理タスクに伝える
	if (ONInputBuff & PAD_INPUT_1)
	{
		//ゲームを選択
		g_SCENE_CHANGE_COMM = SCENE_GAME;
		TaskKill(thisTCB);
		return;
	}
}

void init03_SCENE_CTRL(TCB* thisTCB)
{
	EX3_SCENE_STRUCT* pEX3_SCENEwork;
	pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	//初期化中の状態
	pEX3_SCENEwork->SceneID = SCENE_INIT;

}

void exec03_SCENE_CTRL(TCB* thisTCB)
{
	EX3_SCENE_STRUCT* pEX3_SCENEwork;
	pEX3_SCENEwork = (EX3_SCENE_STRUCT*)thisTCB->Work;

	switch (pEX3_SCENEwork->SceneID)
	{
	case SCENE_INIT: //初期化中
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
	case SCENE_TITLE: //タイトル画面
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
	case SCENE_GAME: //ゲーム処理
		if (g_SCENE_CHANGE_COMM != SCENE_CHANGE_WAIT)
		{
			if (g_SCENE_CHANGE_COMM == SCENE_END) {
				pEX3_SCENEwork->SceneID = SCENE_GAME_OVER;
				TaskMake(exec03_SCENE_GAME_OVER, 0x2000);
				g_SCENE_CHANGE_COMM = SCENE_CHANGE_WAIT;
			}
		}
		break;
	case SCENE_GAME_OVER:  //ゲームオーバー処理
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
	case SCENE_OPTION:  //オプション画面
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
	//****画像読出し****
	//!!!!危険!!!!
	//タスクが消えるたびに読み出すので危険
	//最終的に総合初期化で呼び出すか
	//必要に応じてメモリを開放するべし

	//背景ロード
	dungeonmap[playerpos.nowfloor].backbuf = LoadGraph("./image/dungeon_temp3.bmp");
	//壁画像ロード
	int i, j;
	char name[256];
	//左0中央1右2になったので変更
	char* dir[3] = { "left","center","right" };
	//4*3ミニマップの為、画像は12枚必要
	//4*3になるかも←なった
	for (i = 0; i < MINI_MS; i++) {
		for (j = 0; j < 3; j++) {
			//名前インプット
			snprintfDx(name, 256, "./image/wall/wall_%d_%s.bmp", i, dir[j]);
			//ロード
			wallbuf[i][j] = LoadGraph(name);
		}
	}
	//モンスター画像ロード
	for (i = 0; i < MONSTERNUM; i++) {
		//HP0のモンスターは存在しないものとする
		if (monster_data[i].st.maxhp != 0) {
			snprintfDx(name, 256, "./image/monster/monster_%d.bmp", i);
			monster_img[i] = LoadGraph(name);
		}
	}
	//マップロード
	//テストなので1階のみ
	for (i = 0; i < 1; i++) {
		snprintfDx(name,256, "./map/floor_%d.dat", i);
		int filehandle = FileRead_open(name);
		if (filehandle == 0) {
			//エラー処理
			break;
		}
		FileRead_read(dungeonmap[i].map, sizeof(char)*MAPSIZE*MAPSIZE, filehandle);
		FileRead_close(filehandle);
	}
	//アイテムデータ初期化
	init_item();
	for (i = 0; i < ITEMMAX; i++) {
		info.item[i] = 0xff;
	}
	//フォント設定
	//ただしフォントはデータ配布が(権利的にも知識的にも)めんどいのでやらないかも
	SetFontSize(FONTSIZE);
	ChangeFont("JFドットjiskan16s-1990");
	//システム文字画像読み込み
	snprintfDx(name,256, "./image/word/datawords.bmp");
	LoadDivGraph(name, 128, 16, 8, 8, 8, cmesbuf);
	//コンパス画像読み込み
	snprintfDx(name, 256, "./image/etc/compase.bmp");
	compase_buf = LoadGraph(name);
	//初期化終了
	init03_SCENE_CTRL(thisTCB);
	TaskChange(thisTCB, exec03_SCENE_CTRL);
}

void TaskHead00(TCB* thisTCB) {
	//キー入力所得
	KeyControll(thisTCB);
}

void TaskHead(TCB* thisTCB) {
	TaskMake(TotalInit, 0x1000);
	TaskChange(thisTCB, TaskHead00);
}