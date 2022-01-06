#include <DxLib.h>
#include "user.h"

int mode = 0;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ��ʃ��[�h�̃Z�b�g
	SetGraphMode(SCREEN_WIDTH,
		SCREEN_HEIGHT, 16);

	//�E�B���h�E���[�h�ŋN��
	ChangeWindowMode(true);

	//�c�w���C�u��������������
	if (DxLib_Init() == -1) {
		return -1;
	}

	//�`����o�C���j�A��
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//	SetDrawMode(DX_DRAWMODE_NEAREST);

	int FrameCount = 0;
	mode = 1;

	// ���C�����[�v
	while (1) {
		// ��ʂ�����������
		ClearDrawScreen();

		switch (mode) {
		case 1:
			gamemain();
			break;
		}

		//ESC�L�[�Ńv���O�����I��
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

		//�E�B���h�E���b�Z�[�W����
		if (ProcessMessage() == -1) {
			break;
		}
	}

	//�I��
	DxLib_End();

	return 0;
}