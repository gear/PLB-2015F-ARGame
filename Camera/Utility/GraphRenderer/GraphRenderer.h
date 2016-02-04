/**************************************************************************
		�O���t�`��N���X				�uGraphRenderer.h�v
		Created 2013.03.03 by �����r��
		�O���t���ȒP�ɕ`���B
 **************************************************************************/

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

class GraphRenderer{

public:

	GraphRenderer();

	virtual ~GraphRenderer();

	/* ������ */
	virtual int initialize(float windowWidth, float windowHeight, int graphYPosition, int enableCurrentPositionRendering, int enableXAxisRendering = 1, float graphMagnificationRatio = 1.0f);

	/* ��� */
	virtual void finalize();

	/* �`�悷�� */
	virtual void render();

	/* �����ڐݒ� */
	void setRenderingOption(float lineWidth, float r, float g, float b);

	/* �N���A */
	void clear();

	/* �l�̒ǉ� */
	void addData(float value, int position);

	/* �I�t�Z�b�g�l�̃Z�b�g */
	void setOffsetValue(float value);

	/* �f�[�^�z��̃|�C���^�𓾂� */
	float *getDataArrayPtr();

	/* �f�[�^�T�C�Y��Ԃ� */
	int getDataArraySize();

protected:

	int isInitialize;
	float windowWidth, windowHeight;		// windowWidth��dataArray�̃T�C�Y(�f�[�^��)�Ƃ��Ă�����
	float *dataArray;
	float offsetData;
	int currentPosition;
	int graphYPosition;
	float graphMagnificationRatio;

	int enableXAxisRendering;
	int enableCurrentPositionRendering;

	float lineWidth;
	float r, g, b;
	
	/* x���̕`�� */
	void renderXAxis();

	/* ���݈ʒu�̕`�� */
	void renderCurrentPosition();

};
