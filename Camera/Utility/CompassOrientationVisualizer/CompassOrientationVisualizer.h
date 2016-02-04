/**************************************************************************
		���ʕ`��N���X				�uCompassOrientationVisualizer.h�v
		Created 2015.03.28 by �����r��
 **************************************************************************/
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../Renderer/ImageRenderer.h"
#include "../Renderer/TextRenderer.h"

#include "../../Definitions.h"

class CompassOrientationVisualizer{

public:

	/* �R���X�g���N�^/�f�X�g���N�^ */
	CompassOrientationVisualizer(){}
	virtual ~CompassOrientationVisualizer(){}

	/* ������ */
	virtual int initialize(){
		arrowImage.loadTexture("./data/texture/compass.png", WINDOW_WIDTH, WINDOW_HEIGHT);
		return 0;
	} // initialize

	/* ��� */
	virtual void finalize(){
		arrowImage.releaseTexture();
	} // finalize

	/* �`�� */
	virtual void render(float x, float y, float xyDegAngle){
		
		arrowImage.renderTextureImage(x, y, 0.0f, BLEND_ALPHA, 0.0f, 0.0f, xyDegAngle);		// GL�͔����v����]

	} // render

protected:

	ImageRenderer arrowImage;

};
