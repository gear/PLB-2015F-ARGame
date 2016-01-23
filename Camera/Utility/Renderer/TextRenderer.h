/**************************************************************************
		����ASCII������`��N���X				�uTextRenderer.h�v

		�T�v:
			�V���v���ȕ�����`��N���X�B
			����t�H���g����摜�����Ȃ��̂ō����B
			���l�̕`��ɂ��g����B
			���܂��ŁA���{��Ή��̕�����摜�̐����@�\������(iconv�g�p)�B

		Date:
			2009/04/25	by �����r��@�d�ʑ�IS���r��
			2009/05/09	GL�p�ɉ���	by �����r��
			2009/05/10	UTF8�ɑΉ����������摜�����@�\��ǉ��B
			2013/03/07	glDrawPixels()����߂ăe�N�X�`���`��ɂ����B

		������摜�����@�\�̎g����:
			0. SDL_ttf�͗\��TTF_Init()�ŏ��������Ă����B���̃N���X���ł͏������͍s���ĂȂ��B
				 ASCII�����񍂑��\���@�\���g��Ȃ��ꍇ�́ATextRenderer��initialize()���Ȃ��Ă�OK�B
			1. createTextImage()�Ő������镶����(���{���OK)�Attf�t�H���g�̂���ꏊ�ƁA�t�H���g�T�C�Y(point)�A
				 �F�����w�肷��ƁASDL_Surface�ŋA���Ă���B
				 ���̍쐬�����͊��Əd���̂ŁA���[�v�ɓ����ꍇ�͒��ӁB
			2. ����SDL_Surface��OpenGL�ł��̂܂܎g�p�ł���̂ŁA���̗�̂悤�ɂ���ImageRenderer�ɒ��ڎ������邱�Ƃ��\�B
				 �`����A�ʏ�̉摜�Ɠ����悤�ɍs���B
				
				��:
					ImageRenderer sampleTextImage;			// �錾

					sampleTextImage.setImage( textRenderer.createTextImage("|�M߃�߁L|", "c:\\windows\\fonts\\�݂������.ttf", 100, 255, 255, 0) );			// �ǂݍ���

					sampleTextImage.renderImage(100.0f, 100.0f);			// �`��

					sampleTextImage.releaseImage();				// ���

		  3. ��������SDL_Surface�͎����ŉ������K�v������B
				 ImageRenderer�ɓn�����ꍇ�́AImageRenderer��releaseImage()���s����OK�B

		ASCII�����񍂑��\���̎g����:
			0. SDL_ttf�͗\��TTF_Init()�ŏ��������Ă����B���̃N���X���ł͏������͍s���ĂȂ��B
			1. SDL_ttf�����������initialize()�ŏ������B
				 ttf�t�H���g�̂���ꏊ�ƁA�t�H���g�T�C�Y(point)�A�F���w��
			2. renderASCII()�֐��A�������̓Z���^�����O���L����renderASCIICenter()�ŕ`��B
				 �`��͖���ǂݍ��܂Ȃ�(�ŏ��Ƀe�[�u�������̂�)�̂ō����ɕ`��ł���
		  3. finalize()�ŉ��

		����:
			�Eiconv���C�u�������g�p���Ă���̂ŁAiconv���C�u������ʓr��p�ӂ���K�v������B
			�ESDL_ttf�̏������E����͂����ł͍s��Ȃ����߁A�����ŊO�ōs���K�v������
			�EASCII�����񍂑��\���ŕ`��ł���̂�ASCII�R�[�h�ŕ\���ł��镶���̂�(2�o�C�g�ȏ�̕����A���{�ꓙ�̓T�|�[�g���Ȃ�)
			�EC++���g���Ă���̂ŁA�g�ݍ��ݐ�̃\�[�X�R�[�h�̊g���q�� .c �������� .cpp �ɂ��Ă�
			�E�f�t�H���g�ł�SJIS����z��BEUCJP���̕����R�[�h���ꍇ�́A�`����TEXTRENDERER_SRC_CHARACTER_CODE������������B

 **************************************************************************/

#ifndef __TEXTRENDERER_H__
#define __TEXTRENDERER_H__

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include <windows.h>			// �܂����ł͂����Ă���������
#include <GL/gl.h>
#include <GL/glu.h>

#include "ImageRenderer.h"

/* �����e�N�X�`�� */
typedef struct{
	GLuint textureNo;
	int width, height;
	float texCoordX, texCoordY;
} CharacterTexture;

static const int TEXTRENDERER_TABLE_SIZE = 127;

class TextRenderer{

public:

	TextRenderer();

	~TextRenderer();

	/* SDL_TTF�̏����� */
	static int initializeTTF(){
		if ( TTF_Init() < 0 ){
			printf("SDL_ttf�̏������Ɏ��s���܂���: %s\n", TTF_GetError());
			return -1;
		}
		return 0;	
	} // initializeTTF

	/* SDL_ttf�̉�� */
	static void finalizeTTF(){
		TTF_Quit();
	} // finalizeTTF

	/* ������ */
	int initialize(const char *fontPath, int fontSize, Uint8 r, Uint8 g, Uint8 b, float screenWidth, float screenHeight);

	/* ��� */
	void finalize();

	/* Render ASCII Text */
	virtual void renderASCII(const char *srcString, float x, float y,
		float margin = 0, float xScale = 1.0f, float yScale = 1.0f);		// margin�͕����ƕ����̊Ԃ̊Ԋu

	/* Render ASCII Text(Centering Enabled) */
	virtual void renderASCIICenter(const char *srcString, float y, float margin = 0, float xScale = 1.0f, float yScale = 1.0f);

	/* Create Text Image */
	virtual ImageRenderer *createTextImage(const char *string, int fontSize, Uint8 r, Uint8 g, Uint8 b);

	/* �K�v�ɉ����ăt�H���g�����(�쐬�����e�[�u�����g���������\���݂̂ŁA����createTextImage()����K�v�Ƃ��Ȃ��ꍇ�Ƀt�H���g�݂̂����) */
	void closeFont();

protected:

	TTF_Font *font;
	CharacterTexture asciiCharacterTextureTable[127];				// �T�|�[�g����̂�ascii�R�[�h�̕�������
	SDL_Color color;

	int fontSize;
	float screenWidth, screenHeight;

	/* �����`��p�����e�[�u������� */
	//void createASCIITable();
	void createASCIITextureTable();

	/* ��������� */
	SDL_Surface *loadCharacter(const char asciiCharacter, int *fontImageWidth, int *fontImageHeight, float *texCoordX, float *texCoordY);
	SDL_Surface *loadText(const char *axciiText, SDL_Color color, float *texCoordX, float *texCoordY);

	/* �ǂݍ��݂ƕϊ� */
	SDL_Surface *convertImage(SDL_Surface *srcSurface, float *texCoordW, float *texCoordH);

	/* �e�N�X�`���쐬 */
	GLuint createTexture(SDL_Surface *srcImage, int *width, int *height);

	/* calculate minimum power of two value grater than specific value(for calculating OpenGL supported texture width/height size) */
	int calcPowerOfTwo(int value){
		int result = 1;
		while ( result < value ){
			result <<= 1;
		}
		return result;
	} // calcPowerOfTwo
};

#endif