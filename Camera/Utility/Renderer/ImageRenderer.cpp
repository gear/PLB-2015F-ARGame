#include "ImageRenderer.h"

ImageRenderer::ImageRenderer(){
	imageData = NULL;
	srcSDLSurface = NULL;
	textureNo = 0;
	flagTextTexture = 0;
} // �R���X�g���N�^

ImageRenderer::~ImageRenderer(){
	releaseImage();
} // �f�X�g���N�^

/* �ǂݍ��� */
int ImageRenderer::loadImage(const char *filename, int windowWidth, int windowHeight){
	releaseImage();
	releaseTexture();
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	if ( loadAndConvertImage(filename, 1) < 0 ){
		printf("�摜�̓ǂݍ��݂Ɏ��s���܂����B\n");
		return -1;
	}
	return 0;
} // loadImage

/* �ǂݍ��� */
int ImageRenderer::setImage(SDL_Surface *srcSurface, int windowWidth, int windowHeight){


	releaseImage();
	releaseTexture();
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	/* �摜����ۑ� */
	width = srcSurface->w;
	height = srcSurface->h;
	imageData = (unsigned char *)srcSurface->pixels;
	srcSDLSurface = srcSurface;

	return 0;
} // setImage

/* �ǂݍ��� */
int ImageRenderer::loadTexture(const char *filename, int windowWidth, int windowHeight){

	/* �摜�̓ǂݍ��� */
	releaseTexture();

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	if ( loadAndConvertImage(filename, 1) < 0 ){
		printf("�摜�̓ǂݍ��݂Ɏ��s���܂����B\n");
		return -1;
	}

	/* �V����GL�e�N�X�`������ */
	glGenTextures(1, &textureNo);
	glBindTexture(GL_TEXTURE_2D, textureNo);

	/* �e�N�X�`���p�����[�^�̏����ݒ� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	/* �e�N�X�`���̌J��Ԃ��̏����w��(�J��Ԃ�) */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* GL�e�N�X�`���ɉ摜�����蓖�Ă� */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
		GL_UNSIGNED_BYTE, imageData);

	releaseImage();

	return 0;

} // loadTexture

/* �ǂݍ��� */
int ImageRenderer::setTexture(GLuint textureNo, int width, int height, int texCoordX, int texCoordY, int flagTextTexture){
	this->width = width;
	this->height = height;
	this->textureNo = textureNo;
	this->texCoordX = texCoordX;
	this->texCoordY = texCoordY;
	this->flagTextTexture = flagTextTexture;
	return 0;
} // setTexture

/* �摜�̉�� */
void ImageRenderer::releaseImage(){
	if ( srcSDLSurface ){
		SDL_FreeSurface(srcSDLSurface);
		srcSDLSurface = NULL;
		imageData = NULL;
	}
} // releaseImage

/* �e�N�X�`���̉�� */
void ImageRenderer::releaseTexture(){
	if ( textureNo ){
		glDeleteTextures(1, &textureNo);
		textureNo = 0;
	}
} // releaseTexture

/* �`�� */
void ImageRenderer::renderImage(float x, float y, BLEND_MODE blendMode, float xScale, float yScale){

	if ( srcSDLSurface == NULL ){
		return;
	}

	/* ����Blend���L�����ǂ������ׁA�����̏ꍇ�͗L���ɂ��� */
	int isBlendEnabled;
	isBlendEnabled = glIsEnabled(GL_BLEND);
	if ( !isBlendEnabled ){
		glEnable(GL_BLEND);
	}

	/* DepthTest���L�����ǂ������ׁA�L���̏ꍇ�͖����ɂ��� */
	int isDepthTestEnabled;
	isDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
	if ( isDepthTestEnabled ){
		glDisable(GL_DEPTH_TEST);
	}

	/* �u�����h�ݒ� */
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

	/* ���_�␳�ʂ̌v�Z */
	float xMove, yMove;
	moveOrigin(x, y, &xMove, &yMove);

	glRasterPos2f(x + xMove, y + yMove);
	glBitmap(0, 0, 0, 0, -xMove, -yMove, NULL);

	glPixelZoom(xScale, yScale);
	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glPixelZoom(1.0f, 1.0f);

	/* Blend�̐ݒ�����ɖ߂� */
	if ( !isBlendEnabled ){
		glDisable(GL_BLEND);
	}

	/* DepthTest�̐ݒ�����ɖ߂� */
	if ( isDepthTestEnabled ){
		glEnable(GL_DEPTH_TEST);
	}

} // renderImage

/* �e�N�X�`���̕`�� */
void ImageRenderer::renderTextureImage(float x, float y, float z, BLEND_MODE blendMode, float xRotation, float yRotation, float zRotation, float xScale, float yScale){

	if ( !textureNo ){
		return;
	}

	/* ����Blend���L�����ǂ������ׁA�����̏ꍇ�͗L���ɂ��� */
	int isBlendEnabled;
	isBlendEnabled = glIsEnabled(GL_BLEND);
	if ( !isBlendEnabled ){
		glEnable(GL_BLEND);
	}

	/* �u�����h�ݒ� */
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

	/* Blend�̐ݒ�����ɖ߂� */
	if ( !isBlendEnabled ){
		glDisable(GL_BLEND);
	}

} // renderTextureImage

/* �e�N�X�`���̎擾 */
GLuint ImageRenderer::getTexture(){
	return textureNo;
} // getTexture

/* �ǂݍ��݂ƕϊ� */
SDL_Surface *ImageRenderer::convertImage(SDL_Surface *srcSurface, float *texCoordW, float *texCoordH){

	int width, height;
	width = calcPowerOfTwo(srcSurface->w);
	height = calcPowerOfTwo(srcSurface->h);

	*texCoordW = (float)srcSurface->w / width;
	*texCoordH = (float)srcSurface->h / height;

	/* ���GL�e�N�X�`���p�T�[�t�F�X�̍쐬 */
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

	/* �A���t�@�u�����f�B���O�����̕ۑ� */
	Uint32 savedFlags;				// �摜�����ۑ��p�ϐ�
	Uint8  savedAlpha;				// �A���t�@�����ۑ��p�t���O
	savedFlags = srcSurface->flags & ( SDL_SRCALPHA | SDL_RLEACCELOK );
	savedAlpha = srcSurface->format->alpha;
	if ( (savedFlags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(srcSurface, 0, 0);
	}

	/* SDL�T�[�t�F�X��GL�e�N�X�`���p�T�[�t�F�X�փu���� */
	SDL_Rect region = {0, 0, srcSurface->w, srcSurface->h};
	SDL_BlitSurface(srcSurface, &region, glSurface, &region);
	SDL_FreeSurface(srcSurface);			// ��������Ȃ�

	return glSurface;
} // convertImage

/* �ǂݍ��݂ƕϊ� */
int ImageRenderer::loadAndConvertImage(const char *filename, int enableVerticalFlip){

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	SDL_Surface *srcSurface = IMG_Load(filename);
	if ( srcSurface == NULL ){
		printf("%s���ǂݍ��߂܂���: %s\n", filename, IMG_GetError());
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

	/* ���GL�e�N�X�`���p�T�[�t�F�X�̍쐬 */
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

	/* �A���t�@�u�����f�B���O�����̕ۑ� */
	Uint32 savedFlags;				// �摜�����ۑ��p�ϐ�
	Uint8  savedAlpha;				// �A���t�@�����ۑ��p�t���O
	savedFlags = tmpSurface->flags & ( SDL_SRCALPHA | SDL_RLEACCELOK );
	savedAlpha = tmpSurface->format->alpha;
	if ( (savedFlags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(tmpSurface, 0, 0);
	}

	/* SDL�T�[�t�F�X��GL�e�N�X�`���p�T�[�t�F�X�փu���� */
	SDL_Rect region = {0, 0, tmpSurface->w, tmpSurface->h};
	SDL_BlitSurface(tmpSurface, &region, glSurface, &region);
	SDL_FreeSurface(tmpSurface);			// ��������Ȃ�

	/* �摜����ۑ� */
	width = glSurface->w;
	height = glSurface->h;
	imageData = (unsigned char *)glSurface->pixels;
	srcSDLSurface = glSurface;

	return 0;

} // loadAndConvertImage

/* �摜���㉺���]������ */
SDL_Surface *ImageRenderer::flipSurface(SDL_Surface *srcSurface){

	SDL_Surface *flippedImage = SDL_CreateRGBSurface(
		srcSurface->flags,
		srcSurface->w, srcSurface->h, 
		srcSurface->format->BitsPerPixel, 
		srcSurface->format->Rmask, 
		srcSurface->format->Gmask,
		srcSurface->format->Bmask,
		srcSurface->format->Amask);

	/* �t���O�̈ڐA */
	flippedImage->flags = srcSurface->flags;

	/* �_���p���b�g�̈ڐA */
	if ( srcSurface->format->BitsPerPixel == 8 ){
		SDL_SetPalette(flippedImage, SDL_LOGPAL, srcSurface->format->palette->colors, 0, srcSurface->format->palette->ncolors);
	}

	unsigned char *srcImagePtr = (unsigned char*)srcSurface->pixels;
	unsigned char *dstImagePtr = (unsigned char*)flippedImage->pixels;

	/* �㉺�t���܂ɂ��ē]�� */
	for ( int j = 0; j < srcSurface->h; j++ ){
 		for ( int i = 0; i < srcSurface->pitch; i++ ){
			dstImagePtr[(srcSurface->h - j - 1) * srcSurface->pitch + i] = srcImagePtr[j * srcSurface->pitch + i];
		}
	}

	return flippedImage;
} // flipSurface

/* ���_�ړ��ʂ��v�Z���� */
void ImageRenderer::moveOrigin(float x, float y, float *xMove, float *yMove, float xZoom, float yZoom){

	/* ���_�␳�ʂ̌v�Z */
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