/**************************************************************************
		高速ASCII文字列描画クラス				「TextRenderer.h」

		概要:
			シンプルな文字列描画クラス。
			毎回フォントから画像を作らないので高速。
			数値の描画にも使える。
			おまけで、日本語対応の文字列画像の生成機能もある(iconv使用)。

		Date:
			2009/04/25	by 佐藤俊樹@電通大IS小池研
			2009/05/09	GL用に改造	by 佐藤俊樹
			2009/05/10	UTF8に対応した文字画像生成機能を追加。
			2013/03/07	glDrawPixels()をやめてテクスチャ描画にした。

		文字列画像生成機能の使い方:
			0. SDL_ttfは予めTTF_Init()で初期化しておく。このクラス内では初期化は行ってない。
				 ASCII文字列高速表示機能を使わない場合は、TextRendererはinitialize()しなくてもOK。
			1. createTextImage()で生成する文字列(日本語もOK)、ttfフォントのある場所と、フォントサイズ(point)、
				 色等を指定すると、SDL_Surfaceで帰ってくる。
				 この作成処理は割と重いので、ループに入れる場合は注意。
			2. このSDL_SurfaceはOpenGLでそのまま使用できるので、次の例のようにしてImageRendererに直接持たせることが可能。
				 描画も、通常の画像と同じように行う。
				
				例:
					ImageRenderer sampleTextImage;			// 宣言

					sampleTextImage.setImage( textRenderer.createTextImage("|｀ﾟωﾟ´|", "c:\\windows\\fonts\\みかちゃん.ttf", 100, 255, 255, 0) );			// 読み込み

					sampleTextImage.renderImage(100.0f, 100.0f);			// 描画

					sampleTextImage.releaseImage();				// 解放

		  3. 生成したSDL_Surfaceは自分で解放する必要がある。
				 ImageRendererに渡した場合は、ImageRendererのreleaseImage()を行えばOK。

		ASCII文字列高速表示の使い方:
			0. SDL_ttfは予めTTF_Init()で初期化しておく。このクラス内では初期化は行ってない。
			1. SDL_ttfを初期化後にinitialize()で初期化。
				 ttfフォントのある場所と、フォントサイズ(point)、色を指定
			2. renderASCII()関数、もしくはセンタリングが有効なrenderASCIICenter()で描画。
				 描画は毎回読み込まない(最初にテーブルを作るので)ので高速に描画できる
		  3. finalize()で解放

		注意:
			・iconvライブラリを使用しているので、iconvライブラリを別途を用意する必要がある。
			・SDL_ttfの初期化・解放はここでは行わないため、自分で外で行う必要がある
			・ASCII文字列高速表示で描画できるのはASCIIコードで表現できる文字のみ(2バイト以上の文字、日本語等はサポートしない)
			・C++を使っているので、組み込み先のソースコードの拡張子が .c だったら .cpp にしてね
			・デフォルトではSJIS環境を想定。EUCJP等の文字コード環境場合は、冒頭のTEXTRENDERER_SRC_CHARACTER_CODEを書き換える。

 **************************************************************************/

#ifndef __TEXTRENDERER_H__
#define __TEXTRENDERER_H__

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include <windows.h>			// まっくではけしてくださいあ
#include <GL/gl.h>
#include <GL/glu.h>

#include "ImageRenderer.h"

/* 文字テクスチャ */
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

	/* SDL_TTFの初期化 */
	static int initializeTTF(){
		if ( TTF_Init() < 0 ){
			printf("SDL_ttfの初期化に失敗しました: %s\n", TTF_GetError());
			return -1;
		}
		return 0;	
	} // initializeTTF

	/* SDL_ttfの解放 */
	static void finalizeTTF(){
		TTF_Quit();
	} // finalizeTTF

	/* 初期化 */
	int initialize(const char *fontPath, int fontSize, Uint8 r, Uint8 g, Uint8 b, float screenWidth, float screenHeight);

	/* 解放 */
	void finalize();

	/* Render ASCII Text */
	virtual void renderASCII(const char *srcString, float x, float y,
		float margin = 0, float xScale = 1.0f, float yScale = 1.0f);		// marginは文字と文字の間の間隔

	/* Render ASCII Text(Centering Enabled) */
	virtual void renderASCIICenter(const char *srcString, float y, float margin = 0, float xScale = 1.0f, float yScale = 1.0f);

	/* Create Text Image */
	virtual ImageRenderer *createTextImage(const char *string, int fontSize, Uint8 r, Uint8 g, Uint8 b);

	/* 必要に応じてフォントを閉じる(作成したテーブルを使った文字表示のみで、今後createTextImage()等を必要としない場合にフォントのみを閉じる) */
	void closeFont();

protected:

	TTF_Font *font;
	CharacterTexture asciiCharacterTextureTable[127];				// サポートするのはasciiコードの文字だけ
	SDL_Color color;

	int fontSize;
	float screenWidth, screenHeight;

	/* 高速描画用文字テーブルを作る */
	//void createASCIITable();
	void createASCIITextureTable();

	/* 文字を作る */
	SDL_Surface *loadCharacter(const char asciiCharacter, int *fontImageWidth, int *fontImageHeight, float *texCoordX, float *texCoordY);
	SDL_Surface *loadText(const char *axciiText, SDL_Color color, float *texCoordX, float *texCoordY);

	/* 読み込みと変換 */
	SDL_Surface *convertImage(SDL_Surface *srcSurface, float *texCoordW, float *texCoordH);

	/* テクスチャ作成 */
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