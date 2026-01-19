// シューティング基本
#include "DxLib.h"

// ショットの最大数
#define MAX_SHOT	4

int PlayerX, PlayerY, PlayerSize;	// プレイヤーの位置
int MapX, MapY, MapW, MapH; // マップの位置
int AreaX, AreaY, AreaW, AreaH; // エリアの位置

int ShotValid[MAX_SHOT];	// ショットが存在するか、フラグ
int ShotX[MAX_SHOT], ShotY[MAX_SHOT];	// ショットの位置

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int Key = 0;
	int OldKey;	// 前のキー入力状態
	int i, j;

	SetGraphMode(640, 480, 16);
	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// プレイヤーの初期値
	PlayerX = 320;
	PlayerY = 400;
	PlayerSize = 10;

	// マップの初期値
	MapX = 0;
	MapY = 0;
	MapW = 640;
	MapH = 300;

	// エリアの初期値
	AreaX = 200;
	AreaY = 250;
	AreaW = 200;
	AreaH = 200;


	// ショットの存在を初期化する
	for (i = 0; i < MAX_SHOT; i++)
		ShotValid[i] = 0;

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// キー入力取得
		OldKey = Key;
		Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		if (Key & PAD_INPUT_RIGHT) PlayerX += 3;	// 右を押していたら右に進む
		if (Key & PAD_INPUT_LEFT) PlayerX -= 3;	// 左を押していたら左に進む
		if (Key & PAD_INPUT_DOWN) PlayerY += 3;	// 右を押していたら右に進む
		if (Key & PAD_INPUT_UP) PlayerY -= 3;	// 左を押していたら左に進む

		// ショットの移動処理
		for (j = 0; j < MAX_SHOT; j++)
		{
			// ショットデータが無効だったらスキップ
			if (ShotValid[j] == 0) continue;

			// 位置を上にずらす
			ShotY[j] -= 8;

			// 画面外に出ていたらショットデータを無効にする
			if (ShotY[j] < -32) ShotValid[j] = 0;
		}

		// ショットボタンを押していたらショットを出す
		// 一つ前のループでショットボタンを押していたらショットは出さない
		if ((Key & ~OldKey) & PAD_INPUT_A)
		{
			// 使われていないショットデータを探す
			for (j = 0; j < MAX_SHOT; j++)
			{
				if (ShotValid[j] == 0) break;
			}

			// もし使われていないショットデータがあったらショットを出す
			if (j != MAX_SHOT)
			{
				// ショットの位置を設定
				ShotX[j] = PlayerX + 16;
				ShotY[j] = PlayerY;

				// ショットデータを使用中にセット
				ShotValid[j] = 1;
			}
		}

		// 画面を初期化する
		ClearDrawScreen();

		// マップを描画
		DrawBox(MapX, MapY, MapX + MapW, MapY + MapH, GetColor(255, 255, 255), FALSE);

		// エリアを描画
		DrawBox(AreaX, AreaY, AreaX + AreaW, AreaY + AreaH, GetColor(255, 255, 255), TRUE);

		// プレイヤーを描画する
		DrawCircle(PlayerX, PlayerY, PlayerSize, GetColor(255, 0, 0), TRUE);
		// DrawBox(PlayerX, PlayerY, PlayerX + 48, PlayerY + 48, GetColor(255, 0, 0), TRUE);

		// ショットを描画する
		for (j = 0; j < MAX_SHOT; j++)
		{
			// ショットデータが有効な時のみ描画
			if (ShotValid[j] == 1)
				DrawBox(ShotX[j], ShotY[j], ShotX[j] + 16, ShotY[j] + 16,
					GetColor(255, 255, 255), TRUE);

		}
		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
