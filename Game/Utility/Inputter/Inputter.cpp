/**********************************************
		Inputterクラス			「Inputter.h」
 **********************************************/

#include "Inputter.h"

Inputter *Inputter::instance = NULL;

/******************************************
						Singletonメソッド
 ******************************************/
/* Singletonインスタンス生成 */
Inputter *Inputter::getInstance(){
	if ( instance ){
		return instance;
	}else{
		instance = new Inputter();
		return instance;
	}
} // getInstance

/* Singletonインスタンス破棄 */
void Inputter::releaseInstance(){
	if ( instance ){
		delete instance;
		instance = NULL;
	}
} // releaseInstance

/**********************************************
						キー状態の初期化と更新
 **********************************************/
/* 入力情報を更新する。ループの中に入れる。 */
void Inputter::update(void){
	pollEvent();
} // update	

/* キーの状態を初期化する */
void Inputter::initializeKeyState(void){
	memset(keyState, 0, sizeof(INPUTMANAGER_KEY_STATE) * INPUT_MANAGER_MAX_KEYS);
	mouseState.buttonR = mouseState.buttonM = mouseState.buttonL = KEY_STATE_UP;
	printf("Key Sate Initialized\n");
} // initializeKeyState

/**********************************************
								マウス設定
 **********************************************/
/* カーソルの表示/非表示 */
void Inputter::showCursor(){
	SDL_ShowCursor(SDL_ENABLE);
	SDL_WM_GrabInput(SDL_GRAB_OFF);
} // showCursor

void Inputter::hideCursor(){
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_GrabInput(SDL_GRAB_ON);
} // hideCursor

/**********************************************
				コンストラクタ / デストラクタ
 **********************************************/

/* コンストラクタ */
Inputter::Inputter(){} // コンストラクタ

/* デストラクタ */
Inputter::~Inputter(void){} // デストラクタ

/**********************************************
								ポーリング
 **********************************************/

/* イベントポーリング */
void Inputter::pollEvent(void){

	mouseState.buttonRelX = mouseState.buttonRelY = 0;		// 相対移動量の初期化

	while ( SDL_PollEvent(&currentEvent) ){

		switch (currentEvent.type){

			case SDL_QUIT:
				break;

			/*************************************
									マウスの入力
			 *************************************/
			/* マウスが移動した */
			case SDL_MOUSEMOTION:
				mouseState.positionX = currentEvent.motion.x;
				mouseState.positionY = currentEvent.motion.y;
				mouseState.buttonRelX = currentEvent.motion.xrel;
				mouseState.buttonRelY = currentEvent.motion.yrel;
				break;
			      
			/* マウスボタンDown */
			case SDL_MOUSEBUTTONDOWN:

				/* 押されたボタンの種類を判別する */
				switch (currentEvent.button.button){
					case SDL_BUTTON_LEFT:		// 左クリック
						mouseState.buttonL = KEY_STATE_DOWN;
						break;
					case SDL_BUTTON_RIGHT:	// 右クリック
						mouseState.buttonR = KEY_STATE_DOWN;
						break;
					case SDL_BUTTON_MIDDLE:	// 中央クリック
						mouseState.buttonM = KEY_STATE_DOWN;
						break;

				} // switch
				break;

			/* マウスボタンUp */
			case SDL_MOUSEBUTTONUP:
				switch (currentEvent.button.button){
					case SDL_BUTTON_LEFT:
						mouseState.buttonL = KEY_STATE_UP;
						break;
					case SDL_BUTTON_RIGHT:
						mouseState.buttonR = KEY_STATE_UP;
						break;
					case SDL_BUTTON_MIDDLE:
						mouseState.buttonM = KEY_STATE_UP;
						break;
				} // switch
				break;

			/*************************************
									キーボードの入力
				*************************************/
			/* キーボードのキーDown */
			case SDL_KEYDOWN:

				/* 押されたキーの種類を判別する */
				switch (currentEvent.key.keysym.sym){

					/* ESC */
					case SDLK_ESCAPE:
						keyState[SDLK_ESCAPE].state = KEY_STATE_DOWN;
						break;

					/* 矢印キー */
					case SDLK_UP:				// ↑
						keyState[SDLK_UP].state = KEY_STATE_DOWN;
						break;
					case SDLK_DOWN:			// ↓
						keyState[SDLK_DOWN].state = KEY_STATE_DOWN;
						break;
					case SDLK_LEFT:			// ←
						keyState[SDLK_LEFT].state = KEY_STATE_DOWN;
						break;
					case SDLK_RIGHT:		// →
						keyState[SDLK_RIGHT].state = KEY_STATE_DOWN;
						break;

					/* アルファベット */
					case SDLK_a:		// a
						keyState[SDLK_a].state = KEY_STATE_DOWN;
						break;
					case SDLK_b:		// b
						keyState[SDLK_b].state = KEY_STATE_DOWN;
						break;
					case SDLK_c:		// c
						keyState[SDLK_c].state = KEY_STATE_DOWN;
						break;
					case SDLK_d:		// d
						keyState[SDLK_d].state = KEY_STATE_DOWN;
						break;
					case SDLK_e:	// e
						keyState[SDLK_e].state = KEY_STATE_DOWN;
						break;
					case SDLK_f:		// f
						keyState[SDLK_f].state = KEY_STATE_DOWN;
						break;	
					case SDLK_g:		// g
						keyState[SDLK_g].state = KEY_STATE_DOWN;
						break;
					case SDLK_h:	// h
						keyState[SDLK_h].state = KEY_STATE_DOWN;
						break;
					case SDLK_i:		// i
						keyState[SDLK_i].state = KEY_STATE_DOWN;
						break;
					case SDLK_j:		// j
						keyState[SDLK_j].state = KEY_STATE_DOWN;
						break;
					case SDLK_k:		// k
						keyState[SDLK_k].state = KEY_STATE_DOWN;
						break;
					case SDLK_l:		// l
						keyState[SDLK_l].state = KEY_STATE_DOWN;
						break;
					case SDLK_m:	// m
						keyState[SDLK_m].state = KEY_STATE_DOWN;
						break;
					case SDLK_n:	// n
						keyState[SDLK_n].state = KEY_STATE_DOWN;
						break;
					case SDLK_o:	// o
						keyState[SDLK_o].state = KEY_STATE_DOWN;
						break;
					case SDLK_p:		// p
						keyState[SDLK_p].state = KEY_STATE_DOWN;
						break;
					case SDLK_q:		// q
						keyState[SDLK_q].state = KEY_STATE_DOWN;
						break;
					case SDLK_r:		// r
						keyState[SDLK_r].state = KEY_STATE_DOWN;
						break;
					case SDLK_s:		// s
						keyState[SDLK_s].state = KEY_STATE_DOWN;
						break;
					case SDLK_t:		// t
						keyState[SDLK_t].state = KEY_STATE_DOWN;
						break;
					case SDLK_u:	// u
						keyState[SDLK_u].state = KEY_STATE_DOWN;
						break;
					case SDLK_v:		// v
						keyState[SDLK_v].state = KEY_STATE_DOWN;
						break;
					case SDLK_w:		// w
						keyState[SDLK_w].state = KEY_STATE_DOWN;
						break;
					case SDLK_x:		// x
						keyState[SDLK_x].state = KEY_STATE_DOWN;
						break;
					case SDLK_y:		// y
						keyState[SDLK_y].state = KEY_STATE_DOWN;
						break;
					case SDLK_z:		// z
						keyState[SDLK_z].state = KEY_STATE_DOWN;
						break;

					/* 数字 */
					case SDLK_1:
						keyState[SDLK_1].state = KEY_STATE_DOWN;
						break;
					case SDLK_2:
						keyState[SDLK_2].state = KEY_STATE_DOWN;
						break;
					case SDLK_3:
						keyState[SDLK_3].state = KEY_STATE_DOWN;
						break;
					case SDLK_4:
						keyState[SDLK_4].state = KEY_STATE_DOWN;
						break;
					case SDLK_5:
						keyState[SDLK_5].state = KEY_STATE_DOWN;
						break;
					case SDLK_6:
						keyState[SDLK_6].state = KEY_STATE_DOWN;
						break;
					case SDLK_7:
						keyState[SDLK_7].state = KEY_STATE_DOWN;
						break;
					case SDLK_8:
						keyState[SDLK_8].state = KEY_STATE_DOWN;
						break;
					case SDLK_9:
						keyState[SDLK_9].state = KEY_STATE_DOWN;
						break;
					case SDLK_0:
						keyState[SDLK_0].state = KEY_STATE_DOWN;
						break;

					/* その他 */
					case SDLK_SPACE:		// Space
						keyState[SDLK_SPACE].state = KEY_STATE_DOWN;
						break;
					case SDLK_BACKSPACE:		// BackSpace
						keyState[SDLK_BACKSPACE].state = KEY_STATE_DOWN;
						break;
					case SDLK_RETURN:		// Enter
						keyState[SDLK_RETURN].state = KEY_STATE_DOWN;
						break;

				} // switch
				break;

			/* キーボードのキーUp */
			case SDL_KEYUP:
								
				/* 押されたキーの種類を判別する */
				switch (currentEvent.key.keysym.sym){

					/* ESC */
					case SDLK_ESCAPE:
						keyState[SDLK_ESCAPE].state = KEY_STATE_UP;
						break;

					/* 矢印キー */
					case SDLK_UP:				// ↑
						keyState[SDLK_UP].state = KEY_STATE_UP;
						break;
					case SDLK_DOWN:		// ↓
						keyState[SDLK_DOWN].state = KEY_STATE_UP;
						break;
					case SDLK_LEFT:			// ←
						keyState[SDLK_LEFT].state = KEY_STATE_UP;
						break;
					case SDLK_RIGHT:		// →
						keyState[SDLK_RIGHT].state = KEY_STATE_UP;
						break;

					/* アルファベット */
					case SDLK_a:		// a
						keyState[SDLK_a].state = KEY_STATE_UP;
						break;
					case SDLK_b:		// b
						keyState[SDLK_b].state = KEY_STATE_UP;
						break;
					case SDLK_c:	// c
						keyState[SDLK_c].state = KEY_STATE_UP;
						break;
					case SDLK_d:		// d
						keyState[SDLK_d].state = KEY_STATE_UP;
						break;
					case SDLK_e:	// e
						keyState[SDLK_e].state = KEY_STATE_UP;
						break;
					case SDLK_f:		// f
						keyState[SDLK_f].state = KEY_STATE_UP;
						break;	
					case SDLK_g:		// g
						keyState[SDLK_g].state = KEY_STATE_UP;
						break;
					case SDLK_h:	// h
						keyState[SDLK_h].state = KEY_STATE_UP;
						break;
					case SDLK_i:		// i
						keyState[SDLK_i].state = KEY_STATE_UP;
						break;
					case SDLK_j:		// j
						keyState[SDLK_j].state = KEY_STATE_UP;
						break;
					case SDLK_k:		// k
						keyState[SDLK_k].state = KEY_STATE_UP;
						break;
					case SDLK_l:		// l
						keyState[SDLK_l].state = KEY_STATE_UP;
						break;
					case SDLK_m:	// m
						keyState[SDLK_m].state = KEY_STATE_UP;
						break;
					case SDLK_n:	// n
						keyState[SDLK_n].state = KEY_STATE_UP;
						break;
					case SDLK_o:	// o
						keyState[SDLK_o].state = KEY_STATE_UP;
						break;
					case SDLK_p:		// p
						keyState[SDLK_p].state = KEY_STATE_UP;
						break;
					case SDLK_q:		// q
						keyState[SDLK_q].state = KEY_STATE_UP;
						break;
					case SDLK_r:		// r
						keyState[SDLK_r].state = KEY_STATE_UP;
						break;
					case SDLK_s:		// s
						keyState[SDLK_s].state = KEY_STATE_UP;
						break;
					case SDLK_t:		// t
						keyState[SDLK_t].state = KEY_STATE_UP;
						break;
					case SDLK_u:	// u
						keyState[SDLK_u].state = KEY_STATE_UP;
						break;
					case SDLK_v:		// v
						keyState[SDLK_v].state = KEY_STATE_UP;
						break;
					case SDLK_w:	// w
						keyState[SDLK_w].state = KEY_STATE_UP;
						break;
					case SDLK_x:		// x
						keyState[SDLK_x].state = KEY_STATE_UP;
						break;
					case SDLK_y:		// y
						keyState[SDLK_y].state = KEY_STATE_UP;
						break;
					case SDLK_z:		// z
						keyState[SDLK_z].state = KEY_STATE_UP;
						break;

					/* 数字 */
					case SDLK_1:
						keyState[SDLK_1].state = KEY_STATE_UP;
						break;
					case SDLK_2:
						keyState[SDLK_2].state = KEY_STATE_UP;
						break;
					case SDLK_3:
						keyState[SDLK_3].state = KEY_STATE_UP;
						break;
					case SDLK_4:
						keyState[SDLK_4].state = KEY_STATE_UP;
						break;
					case SDLK_5:
						keyState[SDLK_5].state = KEY_STATE_UP;
						break;
					case SDLK_6:
						keyState[SDLK_6].state = KEY_STATE_UP;
						break;
					case SDLK_7:
						keyState[SDLK_7].state = KEY_STATE_UP;
						break;
					case SDLK_8:
						keyState[SDLK_8].state = KEY_STATE_UP;
						break;
					case SDLK_9:
						keyState[SDLK_9].state = KEY_STATE_UP;
						break;
					case SDLK_0:
						keyState[SDLK_0].state = KEY_STATE_UP;
						break;

					/* その他 */
					case SDLK_SPACE:					// Space
						keyState[SDLK_SPACE].state = KEY_STATE_UP;
						break;
					case SDLK_BACKSPACE:			// BackSpace
						keyState[SDLK_BACKSPACE].state = KEY_STATE_UP;
						break;
					case SDLK_RETURN:					// Enter
						keyState[SDLK_RETURN].state = KEY_STATE_UP;
						break;
				} // switch

				break;
		} //switch

	}//while

} // pollEvent