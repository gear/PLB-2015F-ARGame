/*********************************************************************
	SoundPlayerクラス			「SoundPlayer.h」
	Created 2013/05/02	by SATO Toshiki
	オーディオシステムの初期化、Music/SEの再生。
	Musicは同時に1曲しか再生できない。
 *********************************************************************/
#ifndef __SOUNDPLAYER_H__
#define __SOUNDPLAYER_H__

#include <SDL.h>
#include <SDL_mixer.h>

class SoundPlayer{

public:

	/* SDL_mixerの初期化・解放 */
	static int initializeAudio(int audioFreq = 44100, int channels = 2, int supportFlag = MIX_INIT_MP3, int soundBufferSize = 1024);		// 初期化時に1回呼べばOK
	static void finalizeAudio();		// 解放時に1回呼べばOK
	
	SoundPlayer();
	~SoundPlayer();

	/* Musicを読み込む */
	int loadMusic(const char *filename);

	/* SEを読み込む */
	int loadSE(const char *filename);

	/* 解放 */
	void releaseSE();

	void releaseMusic();

	/* SE再生 */
	int playSE(int loopCount = 0);

	/* Music再生 */
	int playMusic(int loopCount = -1);

	/* SE停止 */
	void stopSE();

	/* Music停止 */
	void stopMusic();

	/* SEフェードアウト */
	void fadeOutSE(int msec);		// msecは完全停止までの時間

	/* Musicフェードアウト */
	void fadeOutMusic(int msec);		// msecは完全停止までの時間

	/* ポーズ */
	void pauseSE();

	/* ポーズ解除(再び再生) */
	void unpauseSE();

private:

	/* SDL_mixerが初期化されたかどうか */
	static int flagMixerInitialized;

	Mix_Music *music;
	Mix_Chunk *se;

	int channelUsed;

};
#endif