#include "ImageRenderer.h"

ImageRenderer::ImageRenderer(){
	imageData = NULL;
	srcSDLSurface = NULL;
	textureNo = 0;
	flagTextTexture = 0;
} // コンストラクタ

ImageRenderer::~ImageRenderer(){
	releaseImage();
} // デストラクタ

/* 読み込み */
int ImageRenderer::loadImage(const char *filename, int windowWidth, int windowHeight){
	releaseImage();
	releaseTexture();
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	if ( loadAndConvertImage(filename, 1) < 0 ){
		printf("画像の読み込みに失敗しました。\n");
		return -1;
	}
	return 0;
} // loadImage

/* 読み込み */
int ImageRenderer::setImage(SDL_Surface *srcSurface, int windowWidth, int windowHeight){


	releaseImage();
	releaseTexture();
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	/* 画像情報を保存 */
	width = srcSurface->w;
	height = srcSurface->h;
	imageData = (unsigned char *)srcSurface->pixels;
	srcSDLSurface = srcSurface;

	return 0;
} // setImage

/* 読み込み */
int ImageRenderer::loadTexture(const char *filename, int windowWidth, int windowHeight){

	/* 画像の読み込み */
	releaseTexture();

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	if ( loadAndConvertImage(filename, 1) < 0 ){
		printf("画像の読み込みに失敗しました。\n");
		return -1;
	}

	/* 新しいGLテクスチャ生成 */
	glGenTextures(1, &textureNo);
	glBindTexture(GL_TEXTURE_2D, textureNo);

	/* テクスチャパラメータの初期設定 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	/* テクスチャの繰り返しの初期指定(繰り返し) */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* GLテクスチャに画像を割り当てる */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
		GL_UNSIGNED_BYTE, imageData);

	releaseImage();

	return 0;

} // loadTexture

/* 読み込み */
int ImageRenderer::setTexture(GLuint textureNo, int width, int height, int texCoordX, int texCoordY, int flagTextTexture){
	this->width = width;
	this->height = height;
	this->textureNo = textureNo;
	this->texCoordX = texCoordX;
	this->texCoordY = texCoordY;
	this->flagTextTexture = flagTextTexture;
	return 0;
} // setTexture

/* 画像の解放 */
void ImageRenderer::releaseImage(){
	if ( srcSDLSurface ){
		SDL_FreeSurface(srcSDLSurface);
		srcSDLSurface = NULL;
		imageData = NULL;
	}
} // releaseImage

/* テクスチャの解放 */
void ImageRenderer::releaseTexture(){
	if ( textureNo ){
		glDeleteTextures(1, &textureNo);
		textureNo = 0;
	}
} // releaseTexture

/* 描画 */
void ImageRenderer::renderImage(float x, float y, BLEND_MODE blendMode, float xScale, float yScale){

	if ( srcSDLSurface == NULL ){
		return;
	}

	/* 既にBlendが有効かどうか調べ、無効の場合は有効にする */
	int isBlendEnabled;
	isBlendEnabled = glIsEnabled(GL_BLEND);
	if ( !isBlendEnabled ){
		glEnable(GL_BLEND);
	}

	/* DepthTestが有効かどうか調べ、有効の場合は無効にする */
	int isDepthTestEnabled;
	isDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
	if ( isDepthTestEnabled ){
		glDisable(GL_DEPTH_TEST);
	}

	/* ブレンド設定 */
	switch ( blendMode ){
		case BLEND_NO_BLEND:
			glBlendFunc(GL_ONE, GL_ZERO);
			break;
		case BLEND_ALPHA:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BLEND_ADD:
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		case BLEND_MULT:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		case BLEND_SCREEN:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
			break;
		case BLEND_INV:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
			break;
		case BLEND_ADD_AND_ALPHA:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA | GL_ONE);
			break;
		case BLEND_XOR:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
			break;
	} // switch

	/* 原点補正量の計算 */
	float xMove, yMove;
	moveOrigin(x, y, &xMove, &yMove);

	glRasterPos2f(x + xMove, y + yMove);
	glBitmap(0, 0, 0, 0, -xMove, -yMove, NULL);

	glPixelZoom(xScale, yScale);
	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glPixelZoom(1.0f, 1.0f);

	/* Blendの設定を元に戻す */
	if ( !isBlendEnabled ){
		glDisable(GL_BLEND);
	}

	/* DepthTestの設定を元に戻す */
	if ( isDepthTestEnabled ){
		glEnable(GL_DEPTH_TEST);
	}

} // renderImage

/* テクスチャの描画 */
void ImageRenderer::renderTextureImage(float x, float y, float z, BLEND_MODE blendMode, float xRotation, float yRotation, float zRotation, float xScale, float yScale){

	if ( !textureNo ){
		return;
	}

	/* 既にBlendが有効かどうか調べ、無効の場合は有効にする */
	int isBlendEnabled;
	isBlendEnabled = glIsEnabled(GL_BLEND);
	if ( !isBlendEnabled ){
		glEnable(GL_BLEND);
	}

	/* ブレンド設定 */
	switch ( blendMode ){
		case BLEND_NO_BLEND:
			glBlendFunc(GL_ONE, GL_ZERO);
			break;
		case BLEND_ALPHA:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BLEND_ADD:
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		case BLEND_MULT:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		case BLEND_SCREEN:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
			break;
		case BLEND_INV:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
			break;
		case BLEND_ADD_AND_ALPHA:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA | GL_ONE);
			break;
		case BLEND_XOR:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
			break;
	} // switch

	glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D , textureNo);

		glScalef(xScale, yScale, 1.0f);
		glTranslatef(x, y, z);
		glRotatef(xRotation, 1.0f, 0.0f, 0.0f);
		glRotatef(yRotation, 0.0f, 1.0f, 0.0f);
		glRotatef(zRotation, 0.0f, 0.0f, 1.0f);

		if ( !flagTextTexture ){

			glBegin(GL_QUADS);
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(texCoordX , texCoordY);
				glVertex2f(width / 2.0f, height / 2.0f);

				glTexCoord2f(0 , texCoordY);
				glVertex2f(-width / 2.0f, height / 2.0f);

				glTexCoord2f(0 , 0);
				glVertex2f(-width / 2.0f, -height / 2.0f);

				glTexCoord2f(texCoordX , 0);
				glVertex2f(width / 2.0f, -height / 2.0f);
			glEnd();

		}else{

			glBegin(GL_QUADS);
				glColor3f(1.0f, 1.0f, 1.0f);

				glTexCoord2f(0 , texCoordY);
				glVertex2f(-width / 2.0f, -height / 2.0f);

				glTexCoord2f(texCoordX , texCoordY);
				glVertex2f(width / 2.0f, -height / 2.0f);

				glTexCoord2f(texCoordX , 0);
				glVertex2f(width / 2.0f, height / 2.0f);

				glTexCoord2f(0 , 0);
				glVertex2f(-width / 2.0f, height / 2.0f);
			glEnd();		
		}

		glBindTexture(GL_TEXTURE_2D , 0);
		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();

	/* Blendの設定を元に戻す */
	if ( !isBlendEnabled ){
		glDisable(GL_BLEND);
	}

} // renderTextureImage

/* テクスチャの取得 */
GLuint ImageRenderer::getTexture(){
	return textureNo;
} // getTexture

/* 読み込みと変換 */
SDL_Surface *ImageRenderer::convertImage(SDL_Surface *srcSurface, float *texCoordW, float *texCoordH){

	int width, height;
	width = calcPowerOfTwo(srcSurface->w);
	height = calcPowerOfTwo(srcSurface->h);

	*texCoordW = (float)srcSurface->w / width;
	*texCoordH = (float)srcSurface->h / height;

	/* 空のGLテクスチャ用サーフェスの作成 */
	SDL_Surface *glSurface = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		width, height,
		32,
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN						// OpenGL RGBA masks
					0x000000FF, 
					0x0000FF00,
					0x00FF0000,
					0xFF000000
		#else
					0xFF000000,
					0x00FF0000,
					0x0000FF00,
					0x000000FF
		#endif
	);

	/* アルファブレンディング属性の保存 */
	Uint32 savedFlags;				// 画像属性保存用変数
	Uint8  savedAlpha;				// アルファ属性保存用フラグ
	savedFlags = srcSurface->flags & ( SDL_SRCALPHA | SDL_RLEACCELOK );
	savedAlpha = srcSurface->format->alpha;
	if ( (savedFlags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(srcSurface, 0, 0);
	}

	/* SDLサーフェスをGLテクスチャ用サーフェスへブリる */
	SDL_Rect region = {0, 0, srcSurface->w, srcSurface->h};
	SDL_BlitSurface(srcSurface, &region, glSurface, &region);
	SDL_FreeSurface(srcSurface);			// もういらない

	return glSurface;
} // convertImage

/* 読み込みと変換 */
int ImageRenderer::loadAndConvertImage(const char *filename, int enableVerticalFlip){

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	SDL_Surface *srcSurface = IMG_Load(filename);
	if ( srcSurface == NULL ){
		printf("%sが読み込めません: %s\n", filename, IMG_GetError());
		return -1;
	}

	width = calcPowerOfTwo(srcSurface->w);
	height = calcPowerOfTwo(srcSurface->h);

	texCoordX = (float)srcSurface->w / width;
	texCoordY = (float)srcSurface->h / height;

	SDL_Surface *tmpSurface = srcSurface;
	if ( enableVerticalFlip ){
		tmpSurface = flipSurface(srcSurface);
		SDL_FreeSurface(srcSurface);
	}

	/* 空のGLテクスチャ用サーフェスの作成 */
	SDL_Surface *glSurface = SDL_CreateRGBSurface(
		SDL_SWSURFACE, 
		width, height,
		32,
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN						// OpenGL RGBA masks
					0x000000FF, 
					0x0000FF00,
					0x00FF0000,
					0xFF000000
		#else
					0xFF000000,
					0x00FF0000,
					0x0000FF00,
					0x000000FF
		#endif
	);

	/* アルファブレンディング属性の保存 */
	Uint32 savedFlags;				// 画像属性保存用変数
	Uint8  savedAlpha;				// アルファ属性保存用フラグ
	savedFlags = tmpSurface->flags & ( SDL_SRCALPHA | SDL_RLEACCELOK );
	savedAlpha = tmpSurface->format->alpha;
	if ( (savedFlags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(tmpSurface, 0, 0);
	}

	/* SDLサーフェスをGLテクスチャ用サーフェスへブリる */
	SDL_Rect region = {0, 0, tmpSurface->w, tmpSurface->h};
	SDL_BlitSurface(tmpSurface, &region, glSurface, &region);
	SDL_FreeSurface(tmpSurface);			// もういらない

	/* 画像情報を保存 */
	width = glSurface->w;
	height = glSurface->h;
	imageData = (unsigned char *)glSurface->pixels;
	srcSDLSurface = glSurface;

	return 0;

} // loadAndConvertImage

/* 画像を上下反転させる */
SDL_Surface *ImageRenderer::flipSurface(SDL_Surface *srcSurface){

	SDL_Surface *flippedImage = SDL_CreateRGBSurface(
		srcSurface->flags,
		srcSurface->w, srcSurface->h, 
		srcSurface->format->BitsPerPixel, 
		srcSurface->format->Rmask, 
		srcSurface->format->Gmask,
		srcSurface->format->Bmask,
		srcSurface->format->Amask);

	/* フラグの移植 */
	flippedImage->flags = srcSurface->flags;

	/* 論理パレットの移植 */
	if ( srcSurface->format->BitsPerPixel == 8 ){
		SDL_SetPalette(flippedImage, SDL_LOGPAL, srcSurface->format->palette->colors, 0, srcSurface->format->palette->ncolors);
	}

	unsigned char *srcImagePtr = (unsigned char*)srcSurface->pixels;
	unsigned char *dstImagePtr = (unsigned char*)flippedImage->pixels;

	/* 上下逆さまにして転送 */
	for ( int j = 0; j < srcSurface->h; j++ ){
 		for ( int i = 0; i < srcSurface->pitch; i++ ){
			dstImagePtr[(srcSurface->h - j - 1) * srcSurface->pitch + i] = srcImagePtr[j * srcSurface->pitch + i];
		}
	}

	return flippedImage;
} // flipSurface

/* 原点移動量を計算する */
void ImageRenderer::moveOrigin(float x, float y, float *xMove, float *yMove, float xZoom, float yZoom){

	/* 原点補正量の計算 */
	float diff;

	if ( x < 0 ){
		diff = windowWidth / -200.0f - x;
		if ( diff > 0 ){
			*xMove = diff * xZoom;
		}else{
			*xMove = 0;
		}
	}else{
		diff = windowWidth / 200.0f - x;
		if ( diff < 0 ){
			*xMove = diff * xZoom;
		}else{
			*xMove = 0;
		}
	}
	if ( y < 0 ){
		diff = windowHeight / -200.0f - y;
		if ( diff > 0 ){
			*yMove = diff * yZoom;
		}else{
			*yMove = 0;
		}
	}else{
		diff = windowHeight / 200.0f - y;
		if ( diff < 0 ){
			*yMove = diff * yZoom;
		}else{
			*yMove = 0;
		}
	}
} // moveOrigin