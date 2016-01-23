/**********************************************
		Inputter�N���X			�uInputter.h�v
 **********************************************/

#include "Inputter.h"

Inputter *Inputter::instance = NULL;

/******************************************
						Singleton���\�b�h
 ******************************************/
/* Singleton�C���X�^���X���� */
Inputter *Inputter::getInstance(){
	if ( instance ){
		return instance;
	}else{
		instance = new Inputter();
		return instance;
	}
} // getInstance

/* Singleton�C���X�^���X�j�� */
void Inputter::releaseInstance(){
	if ( instance ){
		delete instance;
		instance = NULL;
	}
} // releaseInstance

/**********************************************
						�L�[��Ԃ̏������ƍX�V
 **********************************************/
/* ���͏����X�V����B���[�v�̒��ɓ����B */
void Inputter::update(void){
	pollEvent();
} // update	

/* �L�[�̏�Ԃ����������� */
void Inputter::initializeKeyState(void){
	memset(keyState, 0, sizeof(INPUTMANAGER_KEY_STATE) * INPUT_MANAGER_MAX_KEYS);
	mouseState.buttonR = mouseState.buttonM = mouseState.buttonL = KEY_STATE_UP;
	printf("Key Sate Initialized\n");
} // initializeKeyState

/**********************************************
								�}�E�X�ݒ�
 **********************************************/
/* �J�[�\���̕\��/��\�� */
void Inputter::showCursor(){
	SDL_ShowCursor(SDL_ENABLE);
	SDL_WM_GrabInput(SDL_GRAB_OFF);
} // showCursor

void Inputter::hideCursor(){
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_GrabInput(SDL_GRAB_ON);
} // hideCursor

/**********************************************
				�R���X�g���N�^ / �f�X�g���N�^
 **********************************************/

/* �R���X�g���N�^ */
Inputter::Inputter(){} // �R���X�g���N�^

/* �f�X�g���N�^ */
Inputter::~Inputter(void){} // �f�X�g���N�^

/**********************************************
								�|�[�����O
 **********************************************/

/* �C�x���g�|�[�����O */
void Inputter::pollEvent(void){

	mouseState.buttonRelX = mouseState.buttonRelY = 0;		// ���Έړ��ʂ̏�����

	while ( SDL_PollEvent(&currentEvent) ){

		switch (currentEvent.type){

			case SDL_QUIT:
				break;

			/*************************************
									�}�E�X�̓���
			 *************************************/
			/* �}�E�X���ړ����� */
			case SDL_MOUSEMOTION:
				mouseState.positionX = currentEvent.motion.x;
				mouseState.positionY = currentEvent.motion.y;
				mouseState.buttonRelX = currentEvent.motion.xrel;
				mouseState.buttonRelY = currentEvent.motion.yrel;
				break;
			      
			/* �}�E�X�{�^��Down */
			case SDL_MOUSEBUTTONDOWN:

				/* �����ꂽ�{�^���̎�ނ𔻕ʂ��� */
				switch (currentEvent.button.button){
					case SDL_BUTTON_LEFT:		// ���N���b�N
						mouseState.buttonL = KEY_STATE_DOWN;
						break;
					case SDL_BUTTON_RIGHT:	// �E�N���b�N
						mouseState.buttonR = KEY_STATE_DOWN;
						break;
					case SDL_BUTTON_MIDDLE:	// �����N���b�N
						mouseState.buttonM = KEY_STATE_DOWN;
						break;

				} // switch
				break;

			/* �}�E�X�{�^��Up */
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
									�L�[�{�[�h�̓���
				*************************************/
			/* �L�[�{�[�h�̃L�[Down */
			case SDL_KEYDOWN:

				/* �����ꂽ�L�[�̎�ނ𔻕ʂ��� */
				switch (currentEvent.key.keysym.sym){

					/* ESC */
					case SDLK_ESCAPE:
						keyState[SDLK_ESCAPE].state = KEY_STATE_DOWN;
						break;

					/* ���L�[ */
					case SDLK_UP:				// ��
						keyState[SDLK_UP].state = KEY_STATE_DOWN;
						break;
					case SDLK_DOWN:			// ��
						keyState[SDLK_DOWN].state = KEY_STATE_DOWN;
						break;
					case SDLK_LEFT:			// ��
						keyState[SDLK_LEFT].state = KEY_STATE_DOWN;
						break;
					case SDLK_RIGHT:		// ��
						keyState[SDLK_RIGHT].state = KEY_STATE_DOWN;
						break;

					/* �A���t�@�x�b�g */
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

					/* ���� */
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

					/* ���̑� */
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

			/* �L�[�{�[�h�̃L�[Up */
			case SDL_KEYUP:
								
				/* �����ꂽ�L�[�̎�ނ𔻕ʂ��� */
				switch (currentEvent.key.keysym.sym){

					/* ESC */
					case SDLK_ESCAPE:
						keyState[SDLK_ESCAPE].state = KEY_STATE_UP;
						break;

					/* ���L�[ */
					case SDLK_UP:				// ��
						keyState[SDLK_UP].state = KEY_STATE_UP;
						break;
					case SDLK_DOWN:		// ��
						keyState[SDLK_DOWN].state = KEY_STATE_UP;
						break;
					case SDLK_LEFT:			// ��
						keyState[SDLK_LEFT].state = KEY_STATE_UP;
						break;
					case SDLK_RIGHT:		// ��
						keyState[SDLK_RIGHT].state = KEY_STATE_UP;
						break;

					/* �A���t�@�x�b�g */
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

					/* ���� */
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

					/* ���̑� */
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