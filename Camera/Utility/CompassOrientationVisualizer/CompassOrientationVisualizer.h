/**************************************************************************
		方位描画クラス				「CompassOrientationVisualizer.h」
		Created 2015.03.28 by 佐藤俊樹
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

	/* コンストラクタ/デストラクタ */
	CompassOrientationVisualizer(){}
	virtual ~CompassOrientationVisualizer(){}

	/* 初期化 */
	virtual int initialize(){
		arrowImage.loadTexture("./data/texture/compass.png", WINDOW_WIDTH, WINDOW_HEIGHT);
		return 0;
	} // initialize

	/* 解放 */
	virtual void finalize(){
		arrowImage.releaseTexture();
	} // finalize

	/* 描画 */
	virtual void render(float x, float y, float xyDegAngle){
		
		arrowImage.renderTextureImage(x, y, 0.0f, BLEND_ALPHA, 0.0f, 0.0f, xyDegAngle);		// GLは反時計回り回転

	} // render

protected:

	ImageRenderer arrowImage;

};
