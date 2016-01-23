/**************************************************************
		2次元画像を扱うクラスImageRendererのテクスチャ対応版
		「ImageRenderer.h」
			Date:
				2009/04/28 作成	by 佐藤俊樹(dendenkamushi@gmail.com)
				2009/04/30 テクスチャ生成機能を追加 by 佐藤俊樹
				2009/05/10 ブレンド機能を追加

		画像描画機能の使い方:

			・宣言

				ImageRenderer image1;
				ImageRenderer image2;		// 画像1個につきImageRenderer1個

			・画像読み込みその1
				ファイルから直接読み込む場合、loadImage()で行う。
				WINDOW_WIDTHとWINDOW_HEIGHTにはウインドウサイズを指定する。

					if ( testImage.loadImage("./data/image/koike.tga", WINDOW_WIDTH, WINDOW_HEIGHT) < 0 ){
						// 失敗
					}

			・画像読み込みその2
				setImage()で、SDL_Surfaceを渡せる。

				例えば、テキストレンダラからテキスト画像を直接読み込むには、次のような感じで行う。

					testTextImage.setImage( textRenderer.createTextImage("あいうえお", "c:\\windows\\fonts\\みかちゃん.ttf", 100, 255, 255, 0), WINDOW_WIDTH, WINDOW_HEIGHT );

				なお、渡したSDL_Surfaceはクラス内部で解放するので、外で解放してはいけない。

			・画像描画

				renderImage()で次のように行う。

					testTextImage.renderImage(-100.0f, 100.0f, BLEND_NO_BLEND);

				第1引数、第2引数で位置を指定。この場合、画像の原点は左下で、画面は中心が(0, 0)である。
				第3引数にはブレンド方法を指定する。ブレンド方法の詳細は、BLEND_MODEを参照。
				第4引数、第5引数にはスケールを指定する。
				第3引数以降は省略するとデフォルト値が使われる。

				なお、描画時はDepth TestはOFFになるので、描画順に注意する必要がある。

			・解放

				releaseImage()で行う。

		テクスチャ描画機能の使い方:

			画像をテクスチャに貼り付けて描画する。
			手順は、読み込みにloadTexture()、描画にrenderTextureImage()、解放にreleaseTexture()を使用する。
			読み込んだテクスチャのテクスチャ番号を取得したい場合は、getTexture()を使う。

 **************************************************************/

#ifndef __IMAGERENDERER_H__
#define __IMAGERENDERER_H__

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include <SDL.h>
#include <SDL_image.h>

#include <GL/GL.h>

/* ブレンド方法 */
typedef enum{
	BLEND_NO_BLEND = 0,
	BLEND_ALPHA,						// アルファ合成
	BLEND_ADD,							// 加算合成
	BLEND_ADD_AND_ALPHA,		// 加算アルファ合成
	BLEND_MULT,							// 乗算合成
	BLEND_INV,							// 反転合成
	BLEND_SCREEN,						// スクリーン合成
	BLEND_XOR,							// 排他的論理和合成
} BLEND_MODE;

class ImageRenderer{

public:

	ImageRenderer();
	~ImageRenderer();

	/*********************************************
									2D画像関係
	 *********************************************/

	/* 読み込み */
	int loadImage(const char *filename, int windowWidth = 640, int windowHeight = 480);

	/* 読み込み */
	int setImage(SDL_Surface *srcSurface, int windowWidth = 640, int windowHeight = 480);

	/* 画像の解放 */
	void releaseImage();

	/* 描画 */
	void renderImage(float x, float y, BLEND_MODE blendMode = BLEND_ALPHA, float xScale = 1.0f, float yScale = 1.0f);

	/*********************************************
								テクスチャ関係
	 *********************************************/

	/* テクスチャ読み込み */
	int loadTexture(const char *filename, int windowWidth = 640, int windowHeight = 480);
	int setTexture(GLuint textureNo, int width, int height, int texCoordX, int texCoordY, int flagTextTexture);

	/* テクスチャの解放 */
	void releaseTexture();

	/* テクスチャの取得 */
	GLuint getTexture();

	/* テクスチャの描画 */
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

	/* 読み込みと変換 */
	SDL_Surface *convertImage(SDL_Surface *srcSurface, float *texCoordW, float *texCoordH);
	int loadAndConvertImage(const char *filename, int enableVerticalFlip = 1);

	/* 画像を上下反転させる */
	SDL_Surface *flipSurface(SDL_Surface *srcSurface);

	/* 原点移動量を計算する */
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