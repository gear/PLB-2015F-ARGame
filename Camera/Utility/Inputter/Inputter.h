/*******************************************************
	Inputter�N���X		�uInputter.h�v

	�T�v:
		SDL�ŃL�[�{�[�h���́A�}�E�X���͂��Ǘ�����N���X�B
		���͂��󂯕t����E�C���h�E��SDL�̃E�C���h�E�݂̂Ȃ̂Œ��ӁB

	Date:
		Created: 2009/01/16	by �����r��@UECISMS
		Updated: 2010/07/10	by �����r��@UECISMS
		Updated: 2015/02/22 by �����r��@UECISMS: �R�[�h���`�ȂǁA�����ׂ����ύX
 *******************************************************/
#ifndef __INPUTTER_H__
#define __INPUTTER_H__

#include <SDL.h>

/* �L�[�̍ő�l */
static const int INPUT_MANAGER_MAX_KEYS = SDLK_LAST;

/* �L�[�̏��( �����ꂽ��1(=KEY_STATE_DOWN), �����ꂽ��0(=KEY_STATE_UP) ) */
typedef enum{
	KEY_STATE_UP = 0,
	KEY_STATE_DOWN = 1,
} KEY_STATE;

/* �L�[�����i�[����\���� */
typedef struct{
  //char symbol;								// �L�[�̕���(�g��Ȃ�)
	KEY_STATE state;						// �L�[�̏��
} INPUTMANAGER_KEY_STATE;

/* �L�[�����i�[����\���� */
typedef struct{
	Uint16 positionX, positionY;																				// �}�E�X�̐�΍��W
	Sint16 buttonRelX, buttonRelY;																			// �}�E�X�̑��Έړ���
	KEY_STATE buttonR, buttonM, buttonL;																// �}�E�X�̃{�^���̏��
} INPUTMANAGER_MOUSE_STATE;

class Inputter{
 
public:

	/******************************************
							Singleton���\�b�h
	 ******************************************/

	/* Singleton�C���X�^���X���� */
	static Inputter *getInstance();

	/* Singleton�C���X�^���X�j�� */
	static void releaseInstance();

	/**********************************************
							�L�[��Ԃ̏������ƍX�V
	 **********************************************/

	/* ���͏����X�V����B���[�v�̒��ɓ����B */
	void update(void);

	/* �L�[�̏�Ԃ����������� */
	void initializeKeyState(void);

	/**********************************************
									�}�E�X�ݒ�
	 **********************************************/

	/* �J�[�\���̕\��/��\�� */
	void showCursor();
	void hideCursor();

	/**********************************************
									���͂̏��
	 **********************************************/

	INPUTMANAGER_KEY_STATE keyState[INPUT_MANAGER_MAX_KEYS];					// �L�[�̏��
	INPUTMANAGER_MOUSE_STATE mouseState;															// �}�E�X�̏��

private:

	static Inputter *instance;

	/**********************************************
					�R���X�g���N�^ / �f�X�g���N�^
	 **********************************************/

	/* �R���X�g���N�^ */
	Inputter();

	/* �f�X�g���N�^ */
	virtual ~Inputter(void);

	/**********************************************
									�|�[�����O
	 **********************************************/

	/* �C�x���g�|�[�����O */
	SDL_Event currentEvent;
	void pollEvent(void);

};
#endif