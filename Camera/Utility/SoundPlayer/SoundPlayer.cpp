#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(){
	se = NULL;
	music = NULL;
	channelUsed = -1;			// SE再生に使用しているチャンネル。-1の時は再生してない状態
} // コンストラクタ

SoundPlayer::~SoundPlayer(){
	if ( se ){
		Mix_FreeChunk(se);
		se = NULL;
	}
	if ( music ){
		Mix_FreeMusic(music);
		music = NULL;
	}
} // デストラクタ

/* SDL_mixerが既に初期化されたかどうか */
int SoundPlayer::flagMixerInitialized = 0;

/* SDL_mixer初期化 */
int SoundPlayer::initializeAudio(int audioFreq, int channels, int supportFlag, int soundBufferSize){
	if ( SoundPlayer::flagMixerInitialized == 0 ){
		//int supportedFormat = Mix_Init( supportFlag );
		//if( (supportedFormat & supportFlag) != supportFlag ) {
		//	printf("必要なオーディオフォーマットがサポートされていません: %s\n", Mix_GetError());
		//	return -1;
		//}
		if ( Mix_OpenAudio(audioFreq,  MIX_DEFAULT_FORMAT, channels, soundBufferSize) < 0 ){
			printf("サウンドの初期化に失敗しました: %s\n", Mix_GetError());
			return -1;
		}

		SoundPlayer::flagMixerInitialized = 1;
	}
	return 0;
} // initializeAudio

/* SDL_mixer解放 */
void SoundPlayer::finalizeAudio(){
	if ( SoundPlayer::flagMixerInitialized ){
		Mix_CloseAudio();
		Mix_Quit();
		SoundPlayer::flagMixerInitialized = 0;
	}
} // finalizeAudio

/* Musicを読み込む */
int SoundPlayer::loadMusic(const char *filename){
	if ( (music = Mix_LoadMUS(filename)) == NULL ){
		printf( "%sの読込みに失敗しました。: %s\n", filename, Mix_GetError() );
		return -1;
	}else{
		printf("Music: %s を読込みました。\n", filename);
	}
	return 0;
} // loadMusic

/* SEを読み込む */
int SoundPlayer::loadSE(const char *filename){
	if ( (se = Mix_LoadWAV(filename)) == NULL ){
		printf( "%sの読込みに失敗しました。: %s\n", filename, Mix_GetError() );
		return -1;

	}else{
		printf("SE: %s を読込みました。\n", filename);
	}
	return 0;
} // loadSE

/* 解放 */
void SoundPlayer::releaseSE(){
	Mix_FreeChunk(se);
	se = NULL;
} // releaseSE

void SoundPlayer::releaseMusic(){
	Mix_FreeMusic(music);
	music = NULL;
} // releaseMusic

/* SE再生 */
int SoundPlayer::playSE(int loopCount){
	channelUsed = Mix_PlayChannel(-1, se, loopCount);
	return channelUsed;
} // playSE

/* Music再生 */
int SoundPlayer::playMusic(int loopCount){
	return Mix_PlayMusic(music, loopCount);
} // playMusic

/* SE停止 */
void SoundPlayer::stopSE(){
	Mix_HaltChannel(channelUsed);
	channelUsed = -1;
} // stopSE

/* SEフェードアウト */
void SoundPlayer::fadeOutSE(int msec){
	Mix_FadeOutChannel(channelUsed, msec);
} // fadeOutSE

/* Music停止 */
void SoundPlayer::stopMusic(){
	Mix_HaltMusic();
} // stopMusic

/* Musicフェードアウト */
void SoundPlayer::fadeOutMusic(int msec){
	Mix_FadeOutMusic(msec);
} // fadeOutMusic

/* ポーズ */
void SoundPlayer::pauseSE(){
	Mix_Pause(channelUsed);
} // pauseSE

/* ポーズ解除(再び再生) */
void SoundPlayer::unpauseSE(){
	Mix_Resume(channelUsed);
} // unpauseSE