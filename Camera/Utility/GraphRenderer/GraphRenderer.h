/**************************************************************************
		グラフ描画クラス				「GraphRenderer.h」
		Created 2013.03.03 by 佐藤俊樹
		グラフを簡単に描く。
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

	/* 初期化 */
	virtual int initialize(float windowWidth, float windowHeight, int graphYPosition, int enableCurrentPositionRendering, int enableXAxisRendering = 1, float graphMagnificationRatio = 1.0f);

	/* 解放 */
	virtual void finalize();

	/* 描画する */
	virtual void render();

	/* 見た目設定 */
	void setRenderingOption(float lineWidth, float r, float g, float b);

	/* クリア */
	void clear();

	/* 値の追加 */
	void addData(float value, int position);

	/* オフセット値のセット */
	void setOffsetValue(float value);

	/* データ配列のポインタを得る */
	float *getDataArrayPtr();

	/* データサイズを返す */
	int getDataArraySize();

protected:

	int isInitialize;
	float windowWidth, windowHeight;		// windowWidthはdataArrayのサイズ(データ個数)としても扱う
	float *dataArray;
	float offsetData;
	int currentPosition;
	int graphYPosition;
	float graphMagnificationRatio;

	int enableXAxisRendering;
	int enableCurrentPositionRendering;

	float lineWidth;
	float r, g, b;
	
	/* x軸の描画 */
	void renderXAxis();

	/* 現在位置の描画 */
	void renderCurrentPosition();

};
