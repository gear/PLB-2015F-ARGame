/*******************************************************
	Inputterクラス		「Inputter.h」

	概要:
		SDLでキーボード入力、マウス入力を管理するクラス。
		入力を受け付けるウインドウはSDLのウインドウのみなので注意。

	Date:
		Created: 2009/01/16	by 佐藤俊樹@UECISMS
		Updated: 2010/07/10	by 佐藤俊樹@UECISMS
		Updated: 2015/02/22 by 佐藤俊樹@UECISMS: コード整形など、ごく細かい変更
 *******************************************************/
#ifndef __INPUTTER_H__
#define __INPUTTER_H__

#include <SDL.h>

/* キーの最大値 */
static const int INPUT_MANAGER_MAX_KEYS = SDLK_LAST;

/* キーの状態( 押された時1(=KEY_STATE_DOWN), 離された時0(=KEY_STATE_UP) ) */
typedef enum{
	KEY_STATE_UP = 0,
	KEY_STATE_DOWN = 1,
} KEY_STATE;

/* キー情報を格納する構造体 */
typedef struct{
  //char symbol;								// キーの文字(使わない)
	KEY_STATE state;						// キーの状態
} INPUTMANAGER_KEY_STATE;

/* キー情報を格納する構造体 */
typedef struct{
	Uint16 positionX, positionY;																				// マウスの絶対座標
	Sint16 buttonRelX, buttonRelY;																			// マウスの相対移動量
	KEY_STATE buttonR, buttonM, buttonL;																// マウスのボタンの状態
} INPUTMANAGER_MOUSE_STATE;

class Inputter{
 
public:

	/******************************************
							Singletonメソッド
	 ******************************************/

	/* Singletonインスタンス生成 */
	static Inputter *getInstance();

	/* Singletonインスタンス破棄 */
	static void releaseInstance();

	/**********************************************
							キー状態の初期化と更新
	 **********************************************/

	/* 入力情報を更新する。ループの中に入れる。 */
	void update(void);

	/* キーの状態を初期化する */
	void initializeKeyState(void);

	/**********************************************
									マウス設定
	 **********************************************/

	/* カーソルの表示/非表示 */
	void showCursor();
	void hideCursor();

	/**********************************************
									入力の状態
	 **********************************************/

	INPUTMANAGER_KEY_STATE keyState[INPUT_MANAGER_MAX_KEYS];					// キーの状態
	INPUTMANAGER_MOUSE_STATE mouseState;															// マウスの状態

private:

	static Inputter *instance;

	/**********************************************
					コンストラクタ / デストラクタ
	 **********************************************/

	/* コンストラクタ */
	Inputter();

	/* デストラクタ */
	virtual ~Inputter(void);

	/**********************************************
									ポーリング
	 **********************************************/

	/* イベントポーリング */
	SDL_Event currentEvent;
	void pollEvent(void);

};
#endif