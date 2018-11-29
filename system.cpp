#include "DxLib.h"

void GameMain();
int DxLib_Update();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SetGraphMode(640, 480, 16, 60);
	ChangeWindowMode(TRUE);
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);
	SetWindowText("Title");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	GameMain();

	DxLib_End();

	return 0;
}

