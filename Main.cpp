#include "DxLib.h"
#include "Task.h"
#define FPS 60

int DxLib_Update() {
	if (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
		return 1;
	return 0;
}

void GameMain() {

	//initialization here
	// ゲーム開始後からの経過フレーム数
	int cnt_frame = 0;
	// フレーム最初のGetNowCount()の値
	int start_ms;
	// フレーム毎に立つべき時間（ミリ秒）
	int frame_delta_ms = 1000 / FPS;

	// 実際のFPS
	int act_fps = 0;
	// 前回のフレーム情報表示時start_ms
	int start_ms_prev = 0;
	
	//タスクの初期化
	InitTask();

	while (DxLib_Update() == 1) {
		// カウンタ取得
		start_ms = GetNowCount();

		// 待つべき時間を算出
		int frame_wait_ms = frame_delta_ms - (GetNowCount() - start_ms);

		if (frame_wait_ms >= 0) {
			WaitTimer(frame_wait_ms);
		}
		else {
			// 0以下だったらフレーム落ち（あるいはオーバーフローか）
			// do nothing
		}

		// フレーム情報を表示（主にデバッグ用）
		// あんまり高精度じゃないけど気にしない
		act_fps += 2;
		if (start_ms_prev == 0 || start_ms - start_ms_prev >= 500) {
			char frame_mes[256];
			sprintf_s(frame_mes, "FPS: %d, Frame Idle Time: %d ms", act_fps, frame_wait_ms);
			SetWindowText(frame_mes);

			start_ms_prev = start_ms;
			act_fps = 0;
		}

		// フレームカウンタを進める
		cnt_frame++;

		//タスクメインループ
		TaskExec();
	}

}