#include <DxLib.h>
#include "user.h"

int mode = 0;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// 画面モードのセット
	SetGraphMode(SCREEN_WIDTH,
		SCREEN_HEIGHT, 16);

	//ウィンドウモードで起動
	ChangeWindowMode(true);

	//ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1) {
		return -1;
	}

	//描画をバイリニアで
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//	SetDrawMode(DX_DRAWMODE_NEAREST);

	int FrameCount = 0;
	mode = 1;

	// メインループ
	while (1) {
		// 画面を初期化する
		ClearDrawScreen();

		switch (mode) {
		case 1:
			gamemain();
			break;
		}

		//ESCキーでプログラム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

		//ウィンドウメッセージ処理
		if (ProcessMessage() == -1) {
			break;
		}
	}

	//終了
	DxLib_End();

	return 0;
}