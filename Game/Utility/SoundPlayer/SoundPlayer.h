/*********************************************************************
	SoundPlayer�N���X			�uSoundPlayer.h�v
	Created 2013/05/02	by SATO Toshiki
	�I�[�f�B�I�V�X�e���̏������AMusic/SE�̍Đ��B
	Music�͓�����1�Ȃ����Đ��ł��Ȃ��B
 *********************************************************************/
#ifndef __SOUNDPLAYER_H__
#define __SOUNDPLAYER_H__

#include <SDL.h>
#include <SDL_mixer.h>

class SoundPlayer{

public:

	/* SDL_mixer�̏������E��� */
	static int initializeAudio(int audioFreq = 44100, int channels = 2, int supportFlag = MIX_INIT_MP3, int soundBufferSize = 1024);		// ����������1��Ăׂ�OK
	static void finalizeAudio();		// �������1��Ăׂ�OK
	
	SoundPlayer();
	~SoundPlayer();

	/* Music��ǂݍ��� */
	int loadMusic(const char *filename);

	/* SE��ǂݍ��� */
	int loadSE(const char *filename);

	/* ��� */
	void releaseSE();

	void releaseMusic();

	/* SE�Đ� */
	int playSE(int loopCount = 0);

	/* Music�Đ� */
	int playMusic(int loopCount = -1);

	/* SE��~ */
	void stopSE();

	/* Music��~ */
	void stopMusic();

	/* SE�t�F�[�h�A�E�g */
	void fadeOutSE(int msec);		// msec�͊��S��~�܂ł̎���

	/* Music�t�F�[�h�A�E�g */
	void fadeOutMusic(int msec);		// msec�͊��S��~�܂ł̎���

	/* �|�[�Y */
	void pauseSE();

	/* �|�[�Y����(�ĂэĐ�) */
	void unpauseSE();

private:

	/* SDL_mixer�����������ꂽ���ǂ��� */
	static int flagMixerInitialized;

	Mix_Music *music;
	Mix_Chunk *se;

	int channelUsed;

};
#endif