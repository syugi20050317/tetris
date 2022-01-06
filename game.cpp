#include <DxLib.h>
#include "user.h"
#include <time.h>
#include <thread>   // std::this_thread::get_id()を使うのに必要
#include <fstream>  // std::wofstreamを使うのに必要
#include "iostream"
int nowtime0 = 0;
int nowtime1 = 0;
void OutputLogToCChokka(std::wstring txt)
{

	//FILE* fp = NULL;
	auto t = time(nullptr);
	auto tmv = tm();
	auto error = localtime_s(&tmv, &t); // ローカル時間(タイムゾーンに合わせた時間)を取得

	WCHAR buf[256] = { 0 };
	wcsftime(buf, 256, L"%Y/%m/%d %H:%M:%S ", &tmv);

	// 現在のスレッドIDを出力
	auto thId = std::this_thread::get_id();

	// ログ出力
	std::wstring logtxt = buf + txt;


	// ファイルを開く(なければ作成)
	// C直下のファイルに書くにはexe実行時に管理者権限にする必要アリ
	std::wofstream ofs(L"D:\\mylog.log", std::ios::app);
	if (!ofs)
	{
		return;
	}
	// 現在時刻とスレッドIDを付けたログをファイルに書き込み
	ofs << thId << L"  " << logtxt.c_str() << std::endl;
	std::wcout << thId << L"  " << logtxt.c_str() << std::endl;
	// ファイル閉じる
	ofs.close();
}
void gamemain() {
	OutputLogToCChokka(L"gamemain\r\n");
	//初期化
	int blockplace[10][20];
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			blockplace[i][j] = 0;
		}
	}
	int nowtime = 0;
	WaitTimer(300);
	while (1) {
		drawingboard();
		makeblock(blockplace);
		drawingbloack(blockplace);
		if (nowtime == 0) {
			nowtime = clock();
		}
		if (clock() - nowtime > 500) {
			dropblock(blockplace);
			nowtime = clock();
		}
		control(blockplace);
		ScreenFlip();
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}
	}
}
void gameover(int nowboard[10][20]) {
	OutputLogToCChokka(L"gameover\r\n");
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 10; j++) {
			if (nowboard[j][i] != 0) {
				while (1)
				{
					if (CheckHitKey(KEY_INPUT_ESCAPE)) {
						gamemain();
						break;
					}
				}
			}
		}
	}
}
void deleteline(int nowboard[10][20]) {
	OutputLogToCChokka(L"deleteline\r\n");
	int count = 0;
	for (int i = 0; i < 20; i++) {
		count = 0;
		for (int j = 0; j < 10; j++) {
			if (nowboard[j][i] != 0) {
				count++;
			}
		}
		if (count == 10) {
			for (int j = 0; j < 10; j++) {
				nowboard[j][i] = 0;
			}
			for (int j = 0; j < 10; j++) {
				for (int k = i; k > 0; k--) {
					nowboard[j][k] = nowboard[j][k - 1];
				}
			}
		}
	}
}
int stop = 0;
void control(int nowboard[10][20]) {
	OutputLogToCChokka(L"control\r\n");
	int restore = 0;
	int restore_board[10][20];
	if (CheckHitKey(KEY_INPUT_A) != 0) {
		if (clock() - nowtime0 > 170) {
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 20; j++) {
					restore_board[i][j] = nowboard[i][j];
					if (nowboard[i][j] > 10) {
						if (nowboard[i - 1][j] != 0) {
							restore = 1;
						}
						nowboard[i - 1][j] = nowboard[i][j];
						nowboard[i][j] = 0;
						nowtime0 = clock();
					}
				}
			}
		}
	}
	if (CheckHitKey(KEY_INPUT_D) != 0) {
		if (clock() - nowtime0 > 170) {
			for (int i = 9; i >= 0; i--) {
				for (int j = 0; j < 20; j++) {
					restore_board[i][j] = nowboard[i][j];
					if (nowboard[i][j] > 10) {
						if (nowboard[i + 1][j] != 0) {
							restore = 1;
						}
						nowboard[i + 1][j] = nowboard[i][j];
						nowboard[i][j] = 0;
						nowtime0 = clock();

					}
				}
			}
		}
	}
	if (restore == 1) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 20; j++) {
				nowboard[i][j] = restore_board[i][j];
			}
		}
		restore = 0;
	}
	int x = 0;
	int y = 0;
	//順番、ｘ、ｙ、中身
	int block[4][3];
	int ifcant[10][20];
	if (CheckHitKey(KEY_INPUT_W) != 0) {
		if (stop == 0) {
			stop = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 20; j++) {
					ifcant[i][j] = nowboard[i][j];
					if (nowboard[i][j] > 20) {
						x = i;
						y = j;
					}
				}
			}
			int count = 0;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 20; j++) {
					if (nowboard[i][j] > 10) {
						for (int k = 0; k < 3; k++) {
							if (k == 0) {
								block[count][k] = i;
							}
							else if (k == 1) {
								block[count][k] = j;
							}
							else {
								block[count][k] = nowboard[i][j];
								nowboard[i][j] = 0;
							}
						}
						count++;
					}
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 3; j++) {
					if (j == 0) {
						block[i][j] = block[i][j] - x;
					}
					else if (j == 1) {
						block[i][j] = block[i][j] - y;
					}
				}
			}
			for (int i = 0; i < 4; i++) {
				int tmp = block[i][0];
				block[i][0] = block[i][1];
				block[i][1] = tmp * -1;
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 3; j++) {
					if (j == 0) {
						block[i][j] = block[i][j] + x;
					}
					else if (j == 1) {
						block[i][j] = block[i][j] + y;
					}
				}
			}
			int cant = 0;
			for (int i = 0; i < 4; i++) {
				if (nowboard[block[i][0]][block[i][1]] != 0) {
					//課題対応：回転前に回転後にかぶっていないか見るかぶってたら元に戻す
					for (int i = 0; i < 10; i++) {
						for (int j = 0; j < 20; j++) {
							nowboard[i][j] = ifcant[i][j];
						}
					}
					cant = 1;
				}
			}
			if (cant==0) {
				for (int i = 0; i < 4; i++) {
					nowboard[block[i][0]][block[i][1]] = block[i][2];
				}
			}
		}
	}
	else if (CheckHitKey(KEY_INPUT_W) == 0) {
		stop = 0;
	}
	if (CheckHitKey(KEY_INPUT_S) != 0) {
		if (clock() - nowtime1 > 125) {
			checkdrop(nowboard);
			for (int i = 0; i < 10; i++) {
				for (int j = 19; j >= 0; j--) {
					if (nowboard[i][j] > 10) {
						nowboard[i][j + 1] = nowboard[i][j];
						nowboard[i][j] = 0;
						nowtime1 = clock();
					}
				}
			}
		}
	}
	int escape = 0;
	if (CheckHitKey(KEY_INPUT_SPACE) != 0) {
		if (clock() - nowtime1 > 200) {
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 20; j++) {
					while (1) {
						escape = checkdrop(nowboard);
						for (int i = 0; i < 10; i++) {
							for (int j = 19; j >= 0; j--) {
								if (nowboard[i][j] > 10) {
									nowboard[i][j + 1] = nowboard[i][j];
									nowboard[i][j] = 0;
									nowtime1 = clock();
								}
							}
						}
						if (escape == 1) {
							escape = 0;
							break;
						}
					}
				}
			}
		}
	}
}
void drawingboard() {
	OutputLogToCChokka(L"drawingboard\r\n");
	DrawBox(0, 0, 500, 1000, GetColor(135, 135, 135), TRUE);
	for (int i = 0; i < 11; i++) {
		DrawLine(50 * i, 0, 50 * i, 1000, GetColor(255, 255, 255), TRUE);
	}
	for (int j = 0; j < 21; j++) {
		DrawLine(0, 50 * j, 500, 50 * j, GetColor(255, 255, 255), TRUE);
	}
}

int drawingbloack(int nowboard[10][20]) {
	OutputLogToCChokka(L"drawingbloack\r\n");
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			switch (nowboard[i][j])
			{
			case 0:
				DrawBox(i * 50 + 1, j * 50 + 1, i * 50 + 49, j * 50 + 49, GetColor(135, 135, 135), TRUE);
				break;
			case 1:
			case 11:
			case 21:
				//水色
				DrawBox(i * 50 + 1, j * 50 + 1, i * 50 + 49, j * 50 + 49, GetColor(30, 220, 240), TRUE);
				break;
			case 2:
			case 12:
			case 22:
				//黄色
				DrawBox(i * 50 + 1, j * 50 + 1, i * 50 + 49, j * 50 + 49, GetColor(240, 230, 30), TRUE);
				break;
			case 3:
			case 13:
			case 23:
				//緑色
				DrawBox(i * 50 + 1, j * 50 + 1, i * 50 + 49, j * 50 + 49, GetColor(70, 230, 40), TRUE);
				break;
			case 4:
			case 14:
			case 24:
				//赤
				DrawBox(i * 50 + 1, j * 50 + 1, i * 50 + 49, j * 50 + 49, GetColor(250, 50, 20), TRUE);
				break;
			case 5:
			case 15:
			case 25:
				//青
				DrawBox(i * 50 + 1, j * 50 + 1, i * 50 + 49, j * 50 + 49, GetColor(50, 20, 250), TRUE);
				break;
			case 6:
			case 16:
			case 26:
				//オレンジ
				DrawBox(i * 50 + 1, j * 50 + 1, i * 50 + 49, j * 50 + 49, GetColor(240, 250, 40), TRUE);
				break;
			case 7:
			case 17:
			case 27:
				//紫
				DrawBox(i * 50 + 1, j * 50 + 1, i * 50 + 49, j * 50 + 49, GetColor(200, 40, 240), TRUE);
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

int dropblock(int nowboard[10][20]) {
	OutputLogToCChokka(L"dropblock\r\n");
	checkdrop(nowboard);
	for (int i = 0; i < 10; i++) {
		for (int j = 19; j >= 0; j--) {
			if (nowboard[i][j] > 10) {
				nowboard[i][j + 1] = nowboard[i][j];
				nowboard[i][j] = 0;
			}
		}
	}
	return 0;
}
int checkdrop(int nowboard[10][20]) {
	OutputLogToCChokka(L"checkdrop\r\n");
	int flag = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 18; j >= 0; j--) {
			if (nowboard[i][19] > 10) {
				flag = 1;
			}
			if (nowboard[i][j] > 10) {
				if (nowboard[i][j + 1] != 0 && nowboard[i][j + 1] != nowboard[i][j]) {
					if (nowboard[i][j + 1] != nowboard[i][j] + 10) {
						if (nowboard[i][j + 1] + 10 != nowboard[i][j]) {
							flag = 1;
						}
					}
				}
			}
		}
	}
	if (flag == 1) {
		for (int i = 0; i < 10; i++) {
			for (int j = 19; j >= 0; j--) {
				if (nowboard[i][j] > 10 && nowboard[i][j] < 20) {
					nowboard[i][j] = nowboard[i][j] - 10;
				}
				else if (nowboard[i][j] > 20) {
					nowboard[i][j] = nowboard[i][j] - 20;
				}
			}
		}
		deleteline(nowboard);
		gameover(nowboard);
	}
	return 1;
}
void makeblock(int nowboard[10][20]) {
	OutputLogToCChokka(L"makeblock\r\n");
	int breaks = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			if (nowboard[i][j] > 10) {
				breaks = 1;
			}
		}
	}
	if (breaks == 0) {
		int rand = GetRand(6) + 1;
		switch (rand)
		{
		case 0:
			break;
		case 1:
			//水色I型
			for (int i = 1; i <= 4; i++) {
				nowboard[2 + i][0] = 11;
				nowboard[4][0] = 21;
			}
			break;
		case 2:
			//黄色O型
			for (int i = 1; i <= 2; i++) {
				for (int j = 0; j <= 1; j++) {
					nowboard[3 + i][0 + j] = 12;
				}
			}
			nowboard[4][1] = 22;
			break;
		case 3:
			//緑色S型
			for (int i = 0; i <= 1; i++) {
				nowboard[3 + i][0] = 13;
			}
			for (int i = 0; i <= 1; i++) {
				nowboard[4 + i][1] = 13;
			}
			nowboard[4][1] = 23;
			break;
		case 4:
			//赤Zがた
			for (int i = 0; i <= 1; i++) {
				nowboard[4 + i][0] = 14;
			}
			for (int i = 0; i <= 1; i++) {
				nowboard[3 + i][1] = 14;
			}
			nowboard[4][1] = 24;
			break;
		case 5:
			//青J型
			nowboard[3][0] = 15;
			for (int i = 0; i <= 2; i++) {
				nowboard[3 + i][1] = 15;
			}
			nowboard[4][1] = 25;
			break;
		case 6:
			//オレンジL型
			nowboard[5][0] = 16;
			for (int i = 0; i <= 2; i++) {
				nowboard[3 + i][1] = 16;
			}
			nowboard[4][1] = 26;
			break;
		case 7:
			//紫T型
			nowboard[4][0] = 17;
			for (int i = 0; i <= 2; i++) {
				nowboard[3 + i][1] = 17;
			}
			nowboard[4][1] = 27;
			break;
		default:
			break;
		}
	}
}