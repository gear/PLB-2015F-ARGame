#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(){
	se = NULL;
	music = NULL;
	channelUsed = -1;			// SE�Đ��Ɏg�p���Ă���`�����l���B-1�̎��͍Đ����ĂȂ����
} // �R���X�g���N�^

SoundPlayer::~SoundPlayer(){
	if ( se ){
		Mix_FreeChunk(se);
		se = NULL;
	}
	if ( music ){
		Mix_FreeMusic(music);
		music = NULL;
	}
} // �f�X�g���N�^

/* SDL_mixer�����ɏ��������ꂽ���ǂ��� */
int SoundPlayer::flagMixerInitialized = 0;

/* SDL_mixer������ */
int SoundPlayer::initializeAudio(int audioFreq, int channels, int supportFlag, int soundBufferSize){
	if ( SoundPlayer::flagMixerInitialized == 0 ){
		//int supportedFormat = Mix_Init( supportFlag );
		//if( (supportedFormat & supportFlag) != supportFlag ) {
		//	printf("�K�v�ȃI�[�f�B�I�t�H�[�}�b�g���T�|�[�g����Ă��܂���: %s\n", Mix_GetError());
		//	return -1;
		//}
		if ( Mix_OpenAudio(audioFreq,  MIX_DEFAULT_FORMAT, channels, soundBufferSize) < 0 ){
			printf("�T�E���h�̏������Ɏ��s���܂���: %s\n", Mix_GetError());
			return -1;
		}

		SoundPlayer::flagMixerInitialized = 1;
	}
	return 0;
} // initializeAudio

/* SDL_mixer��� */
void SoundPlayer::finalizeAudio(){
	if ( SoundPlayer::flagMixerInitialized ){
		Mix_CloseAudio();
		Mix_Quit();
		SoundPlayer::flagMixerInitialized = 0;
	}
} // finalizeAudio

/* Music��ǂݍ��� */
int SoundPlayer::loadMusic(const char *filename){
	if ( (music = Mix_LoadMUS(filename)) == NULL ){
		printf( "%s�̓Ǎ��݂Ɏ��s���܂����B: %s\n", filename, Mix_GetError() );
		return -1;
	}else{
		printf("Music: %s ��Ǎ��݂܂����B\n", filename);
	}
	return 0;
} // loadMusic

/* SE��ǂݍ��� */
int SoundPlayer::loadSE(const char *filename){
	if ( (se = Mix_LoadWAV(filename)) == NULL ){
		printf( "%s�̓Ǎ��݂Ɏ��s���܂����B: %s\n", filename, Mix_GetError() );
		return -1;

	}else{
		printf("SE: %s ��Ǎ��݂܂����B\n", filename);
	}
	return 0;
} // loadSE

/* ��� */
void SoundPlayer::releaseSE(){
	Mix_FreeChunk(se);
	se = NULL;
} // releaseSE

void SoundPlayer::releaseMusic(){
	Mix_FreeMusic(music);
	music = NULL;
} // releaseMusic

/* SE�Đ� */
int SoundPlayer::playSE(int loopCount){
	channelUsed = Mix_PlayChannel(-1, se, loopCount);
	return channelUsed;
} // playSE

/* Music�Đ� */
int SoundPlayer::playMusic(int loopCount){
	return Mix_PlayMusic(music, loopCount);
} // playMusic

/* SE��~ */
void SoundPlayer::stopSE(){
	Mix_HaltChannel(channelUsed);
	channelUsed = -1;
} // stopSE

/* SE�t�F�[�h�A�E�g */
void SoundPlayer::fadeOutSE(int msec){
	Mix_FadeOutChannel(channelUsed, msec);
} // fadeOutSE

/* Music��~ */
void SoundPlayer::stopMusic(){
	Mix_HaltMusic();
} // stopMusic

/* Music�t�F�[�h�A�E�g */
void SoundPlayer::fadeOutMusic(int msec){
	Mix_FadeOutMusic(msec);
} // fadeOutMusic

/* �|�[�Y */
void SoundPlayer::pauseSE(){
	Mix_Pause(channelUsed);
} // pauseSE

/* �|�[�Y����(�ĂэĐ�) */
void SoundPlayer::unpauseSE(){
	Mix_Resume(channelUsed);
} // unpauseSE