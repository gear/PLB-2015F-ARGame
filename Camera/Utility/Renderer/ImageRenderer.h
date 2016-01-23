/**************************************************************
		2�����摜�������N���XImageRenderer�̃e�N�X�`���Ή���
		�uImageRenderer.h�v
			Date:
				2009/04/28 �쐬	by �����r��(dendenkamushi@gmail.com)
				2009/04/30 �e�N�X�`�������@�\��ǉ� by �����r��
				2009/05/10 �u�����h�@�\��ǉ�

		�摜�`��@�\�̎g����:

			�E�錾

				ImageRenderer image1;
				ImageRenderer image2;		// �摜1�ɂ�ImageRenderer1��

			�E�摜�ǂݍ��݂���1
				�t�@�C�����璼�ړǂݍ��ޏꍇ�AloadImage()�ōs���B
				WINDOW_WIDTH��WINDOW_HEIGHT�ɂ̓E�C���h�E�T�C�Y���w�肷��B

					if ( testImage.loadImage("./data/image/koike.tga", WINDOW_WIDTH, WINDOW_HEIGHT) < 0 ){
						// ���s
					}

			�E�摜�ǂݍ��݂���2
				setImage()�ŁASDL_Surface��n����B

				�Ⴆ�΁A�e�L�X�g�����_������e�L�X�g�摜�𒼐ړǂݍ��ނɂ́A���̂悤�Ȋ����ōs���B

					testTextImage.setImage( textRenderer.createTextImage("����������", "c:\\windows\\fonts\\�݂������.ttf", 100, 255, 255, 0), WINDOW_WIDTH, WINDOW_HEIGHT );

				�Ȃ��A�n����SDL_Surface�̓N���X�����ŉ������̂ŁA�O�ŉ�����Ă͂����Ȃ��B

			�E�摜�`��

				renderImage()�Ŏ��̂悤�ɍs���B

					testTextImage.renderImage(-100.0f, 100.0f, BLEND_NO_BLEND);

				��1�����A��2�����ňʒu���w��B���̏ꍇ�A�摜�̌��_�͍����ŁA��ʂ͒��S��(0, 0)�ł���B
				��3�����ɂ̓u�����h���@���w�肷��B�u�����h���@�̏ڍׂ́ABLEND_MODE���Q�ƁB
				��4�����A��5�����ɂ̓X�P�[�����w�肷��B
				��3�����ȍ~�͏ȗ�����ƃf�t�H���g�l���g����B

				�Ȃ��A�`�掞��Depth Test��OFF�ɂȂ�̂ŁA�`�揇�ɒ��ӂ���K�v������B

			�E���

				releaseImage()�ōs���B

		�e�N�X�`���`��@�\�̎g����:

			�摜���e�N�X�`���ɓ\��t���ĕ`�悷��B
			�菇�́A�ǂݍ��݂�loadTexture()�A�`���renderTextureImage()�A�����releaseTexture()���g�p����B
			�ǂݍ��񂾃e�N�X�`���̃e�N�X�`���ԍ����擾�������ꍇ�́AgetTexture()���g���B

 **************************************************************/

#ifndef __IMAGERENDERER_H__
#define __IMAGERENDERER_H__

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include <SDL.h>
#include <SDL_image.h>

#include <GL/GL.h>

/* �u�����h���@ */
typedef enum{
	BLEND_NO_BLEND = 0,
	BLEND_ALPHA,						// �A���t�@����
	BLEND_ADD,							// ���Z����
	BLEND_ADD_AND_ALPHA,		// ���Z�A���t�@����
	BLEND_MULT,							// ��Z����
	BLEND_INV,							// ���]����
	BLEND_SCREEN,						// �X�N���[������
	BLEND_XOR,							// �r���I�_���a����
} BLEND_MODE;

class ImageRenderer{

public:

	ImageRenderer();
	~ImageRenderer();

	/*********************************************
									2D�摜�֌W
	 *********************************************/

	/* �ǂݍ��� */
	int loadImage(const char *filename, int windowWidth = 640, int windowHeight = 480);

	/* �ǂݍ��� */
	int setImage(SDL_Surface *srcSurface, int windowWidth = 640, int windowHeight = 480);

	/* �摜�̉�� */
	void releaseImage();

	/* �`�� */
	void renderImage(float x, float y, BLEND_MODE blendMode = BLEND_ALPHA, float xScale = 1.0f, float yScale = 1.0f);

	/*********************************************
								�e�N�X�`���֌W
	 *********************************************/

	/* �e�N�X�`���ǂݍ��� */
	int loadTexture(const char *filename, int windowWidth = 640, int windowHeight = 480);
	int setTexture(GLuint textureNo, int width, int height, int texCoordX, int texCoordY, int flagTextTexture);

	/* �e�N�X�`���̉�� */
	void releaseTexture();

	/* �e�N�X�`���̎擾 */
	GLuint getTexture();

	/* �e�N�X�`���̕`�� */
	void renderTextureImage(float x, float y, float z, BLEND_MODE blendMode = BLEND_ALPHA,
		float xRotation = 0.0f, float yRotation = 0.0f, float zRotation = 0.0f, 
		float xScale = 1.0f, float yScale = 1.0f);

protected:

	SDL_Surface *srcSDLSurface;
	unsigned char *imageData;
	int width, height;
	float texCoordX, texCoordY;
	int windowWidth, windowHeight;
	int numChannles;
	int flagTextTexture;

	GLuint textureNo;

	/* �ǂݍ��݂ƕϊ� */
	SDL_Surface *convertImage(SDL_Surface *srcSurface, float *texCoordW, float *texCoordH);
	int loadAndConvertImage(const char *filename, int enableVerticalFlip = 1);

	/* �摜���㉺���]������ */
	SDL_Surface *flipSurface(SDL_Surface *srcSurface);

	/* ���_�ړ��ʂ��v�Z���� */
	void moveOrigin(float x, float y, float *xMove, float *yMove, float xZoom = 1.0f, float yZoom = 1.0f);

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