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
	// �Q�[���J�n�ォ��̌o�߃t���[����
	int cnt_frame = 0;
	// �t���[���ŏ���GetNowCount()�̒l
	int start_ms;
	// �t���[�����ɗ��ׂ����ԁi�~���b�j
	int frame_delta_ms = 1000 / FPS;

	// ���ۂ�FPS
	int act_fps = 0;
	// �O��̃t���[�����\����start_ms
	int start_ms_prev = 0;
	
	//�^�X�N�̏�����
	InitTask();

	while (DxLib_Update() == 1) {
		// �J�E���^�擾
		start_ms = GetNowCount();

		// �҂ׂ����Ԃ��Z�o
		int frame_wait_ms = frame_delta_ms - (GetNowCount() - start_ms);

		if (frame_wait_ms >= 0) {
			WaitTimer(frame_wait_ms);
		}
		else {
			// 0�ȉ���������t���[�������i���邢�̓I�[�o�[�t���[���j
			// do nothing
		}

		// �t���[������\���i��Ƀf�o�b�O�p�j
		// ����܂荂���x����Ȃ����ǋC�ɂ��Ȃ�
		act_fps += 2;
		if (start_ms_prev == 0 || start_ms - start_ms_prev >= 500) {
			char frame_mes[256];
			sprintf_s(frame_mes, "FPS: %d, Frame Idle Time: %d ms", act_fps, frame_wait_ms);
			SetWindowText(frame_mes);

			start_ms_prev = start_ms;
			act_fps = 0;
		}

		// �t���[���J�E���^��i�߂�
		cnt_frame++;

		//�^�X�N���C�����[�v
		TaskExec();
	}

}