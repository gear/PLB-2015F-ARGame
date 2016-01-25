/*********************************************
		Rendererクラス			「Renderer.cpp」
		2015/02/22 by 佐藤俊樹@対話型メディア技研
 **********************************************/

#include "Renderer.h"

Renderer *Renderer::renInstance = NULL;

/* Singletonインスタンス生成 */
Renderer *Renderer::getInstance(){
	if ( renInstance ){
		return renInstance;
	}else{
		renInstance = new Renderer();
		return renInstance;
	}
} // capGetInstance

/* Singletonインスタンス破棄 */
void Renderer::releaseInstance(){
	if ( renInstance ){
		delete renInstance;
		renInstance = NULL;
	}
} // releaseInstance

/* 開始 */
void Renderer::begin(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
} // begin

/* 反映 */
void Renderer::end(){
	SDL_GL_SwapBuffers();
} //end

/****************************************************
							初期化 / 終了
 ****************************************************/

/* 初期化(事前のSDLの初期化が必要) */
int Renderer::initialize(int windowWidth, int windowHeight, int cameraImageWidth, int cameraImageHeight, int depth, int isFullScreenMode){

	if ( !isInitialized ){

		this->windowWidth = windowWidth;
		this->windowHeight = windowHeight;
		this->cameraImageWidth = cameraImageWidth;
		this->cameraImageHeight = cameraImageHeight;
		this->depth = depth;
		this->isFullScreenMode = isFullScreenMode;

		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

		SDL_WM_SetCaption("ARFlashlightImageProcessingSystem", "ARFlashlightImageProcessingSystem");

		/* Create Window */
		if ( isFullScreenMode ){
			if( SDL_SetVideoMode( windowWidth, windowHeight, depth, SDL_OPENGL | SDL_FULLSCREEN ) == 0 ) {			// フルスクリーン
				printf( "Faliled: SDL_SetVideoMode(): %s\n", SDL_GetError( ) );
				return -1;
			}
		
		}else{
			if( SDL_SetVideoMode( windowWidth, windowHeight, depth, SDL_OPENGL ) == 0 ) {
				printf( "Faliled: SDL_SetVideoMode(): %s\n", SDL_GetError( ) );
				return -1;
			}else{
				printf("Window Created\n");
			}		
		}

		/* カメラの構築 */
		setCamera();

		//glRasterPos2f(-windowWidth / 200.0f, -windowHeight / 200.0f);

		/* 2のべき乗サイズ */
		textureBufferWidth = calcPowerOfTwo(cameraImageWidth);
		textureBufferHeight = calcPowerOfTwo(cameraImageHeight);

		/* IplImageバッファ */
		renderingBuffer1ch = cvCreateImage(cvSize(textureBufferWidth, textureBufferHeight), IPL_DEPTH_8U, 1);
		renderingBuffer1ch16U = cvCreateImage(cvSize(textureBufferWidth, textureBufferHeight), IPL_DEPTH_16U, 1);
		renderingBuffer3ch = cvCreateImage(cvSize(textureBufferWidth, textureBufferHeight), IPL_DEPTH_8U, 3);
		renderingBuffer4ch = cvCreateImage(cvSize(textureBufferWidth, textureBufferHeight), IPL_DEPTH_8U, 4);

		/* Matバッファ */
		renderingBufferMat1ch = Mat(textureBufferHeight, textureBufferWidth, CV_8UC1);
		renderingBufferMat1ch16U = Mat(textureBufferHeight, textureBufferWidth, CV_16UC1);
		renderingBufferMat3ch = Mat(textureBufferHeight, textureBufferWidth, CV_8UC3);
		renderingBufferMat4ch = Mat(textureBufferHeight, textureBufferWidth, CV_8UC4);

		printf("Renderer Initialized\n");

		isInitialized = 1;

	}else{
		finalize();
		return initialize(windowWidth, windowHeight, cameraImageWidth, cameraImageHeight, isFullScreenMode);
	}

	return 0;
} // initialize

/* カメラの再構築 */
void Renderer::setCamera(){

	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor((GLclampf)0.0f, (GLclampf)0.0f, (GLclampf)0.0f, (GLclampf)0.0f);
	 	
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	 	
	/* Initialize 2D Camera */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(GLdouble)windowWidth / 2.0, (GLdouble)windowWidth / 2.0,
		-(GLdouble)windowHeight / 2.0, (GLdouble)windowHeight / 2.0,
		-100.0, 100.0);

	/* モデルビュー行列を初期化 */
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
} // setCamera

/* 終了 */
void Renderer::finalize(){

	cvReleaseImage(&renderingBuffer4ch);
	cvReleaseImage(&renderingBuffer3ch);
	cvReleaseImage(&renderingBuffer1ch16U);
	cvReleaseImage(&renderingBuffer1ch);

	renderingBufferMat4ch.release();
	renderingBufferMat3ch.release();
	renderingBufferMat1ch16U.release();
	renderingBufferMat1ch.release();

	if ( isInitialized ){
		isInitialized = 0;
	}
} // finalize

/****************************************************
								Rendering funcs
	****************************************************/

void Renderer::renderRGB(IplImage *srcImageRGB, float x, float y, float xScale, float yScale, float zAngle){

	unsigned int textureNo = loadTexture(srcImageRGB, GL_RGB);
	float xTexCoord, yTexCoord;
	xTexCoord = srcImageRGB->width / (float)textureBufferWidth;
	yTexCoord = srcImageRGB->height / (float)textureBufferHeight;
	
	glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D , textureNo);

		glScalef(xScale, yScale, 1.0f);
		glTranslatef(x, y, 1.0f);
		glRotatef(zAngle, 0.0f, 0.0f, 1.0f);

		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0 , yTexCoord);
			glVertex2f(-srcImageRGB->width / 2.0f, srcImageRGB->height / 2.0f);

			glTexCoord2f(0 , 0);
			glVertex2f(-srcImageRGB->width / 2.0f, -srcImageRGB->height / 2.0f);

			glTexCoord2f(xTexCoord , 0);
			glVertex2f(srcImageRGB->width / 2.0f, -srcImageRGB->height / 2.0f);

			glTexCoord2f(xTexCoord , yTexCoord);
			glVertex2f(srcImageRGB->width / 2.0f, srcImageRGB->height / 2.0f);
		glEnd();

		glBindTexture(GL_TEXTURE_2D , 0);
		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();

	glDeleteTextures(1, &textureNo);

} // renderRGB

void Renderer::renderMatRGB(Mat srcImageRGB, float x, float y, float xScale, float yScale, float zAngle){

	unsigned int textureNo = loadTextureMat(srcImageRGB, GL_RGB);
	float xTexCoord, yTexCoord;
	xTexCoord = srcImageRGB.cols / (float)textureBufferWidth;
	yTexCoord = srcImageRGB.rows / (float)textureBufferHeight;
	
	glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D , textureNo);

		glScalef(xScale, yScale, 1.0f);
		glTranslatef(x, y, 1.0f);
		glRotatef(zAngle, 0.0f, 0.0f, 1.0f);

		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0 , yTexCoord);
			glVertex2f(-srcImageRGB.cols / 2.0f, srcImageRGB.rows / 2.0f);

			glTexCoord2f(0 , 0);
			glVertex2f(-srcImageRGB.cols / 2.0f, -srcImageRGB.rows / 2.0f);

			glTexCoord2f(xTexCoord , 0);
			glVertex2f(srcImageRGB.cols / 2.0f, -srcImageRGB.rows / 2.0f);

			glTexCoord2f(xTexCoord , yTexCoord);
			glVertex2f(srcImageRGB.cols / 2.0f, srcImageRGB.rows / 2.0f);
		glEnd();

		glBindTexture(GL_TEXTURE_2D , 0);
		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();

	glDeleteTextures(1, &textureNo);

} // renderMatRGB

void Renderer::renderRGBA(IplImage *srcImageRGBA, float x, float y, float xScale, float yScale, float zAngle){

	float xTexCoord, yTexCoord;
	unsigned int textureNo = loadTexture(srcImageRGBA, GL_RGBA);
	xTexCoord = srcImageRGBA->width / (float)textureBufferWidth;
	yTexCoord = srcImageRGBA->height / (float)textureBufferHeight;

	glBlendFunc(GL_ONE, GL_ZERO);

	glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D , textureNo);

		glScalef(xScale, yScale, 1.0f);
		glTranslatef(x, y, 1.0f);
		glRotatef(zAngle, 0.0f, 0.0f, 1.0f);

		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0 , yTexCoord);
			glVertex2f(-srcImageRGBA->width / 2.0f, srcImageRGBA->height / 2.0f);

			glTexCoord2f(0 , 0);
			glVertex2f(-srcImageRGBA->width / 2.0f, -srcImageRGBA->height / 2.0f);

			glTexCoord2f(xTexCoord , 0);
			glVertex2f(srcImageRGBA->width / 2.0f, -srcImageRGBA->height / 2.0f);

			glTexCoord2f(xTexCoord , yTexCoord);
			glVertex2f(srcImageRGBA->width / 2.0f, srcImageRGBA->height / 2.0f);
		glEnd();
	
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D , 0);
	glDeleteTextures(1, &textureNo);
	glDisable(GL_TEXTURE_2D);

} // renderRGBA

void Renderer::renderMatRGBA(Mat srcImageRGBA, float x, float y, float xScale, float yScale, float zAngle){

	float xTexCoord, yTexCoord;
	unsigned int textureNo = loadTextureMat(srcImageRGBA, GL_RGBA);
	xTexCoord = srcImageRGBA.cols / (float)textureBufferWidth;
	yTexCoord = srcImageRGBA.rows / (float)textureBufferHeight;

	glBlendFunc(GL_ONE, GL_ZERO);

	glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D , textureNo);

		glScalef(xScale, yScale, 1.0f);
		glTranslatef(x, y, 1.0f);
		glRotatef(zAngle, 0.0f, 0.0f, 1.0f);

		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0 , yTexCoord);
			glVertex2f(-srcImageRGBA.cols / 2.0f, srcImageRGBA.rows / 2.0f);

			glTexCoord2f(0 , 0);
			glVertex2f(-srcImageRGBA.cols / 2.0f, -srcImageRGBA.rows / 2.0f);

			glTexCoord2f(xTexCoord , 0);
			glVertex2f(srcImageRGBA.cols / 2.0f, -srcImageRGBA.rows / 2.0f);

			glTexCoord2f(xTexCoord , yTexCoord);
			glVertex2f(srcImageRGBA.cols / 2.0f, srcImageRGBA.rows / 2.0f);
		glEnd();
	
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D , 0);
	glDeleteTextures(1, &textureNo);
	glDisable(GL_TEXTURE_2D);

} // renderMatRGBA

void Renderer::renderGRAY(IplImage *srcImageGRAY, float x, float y, float xScale, float yScale, float zAngle){

	unsigned int textureNo = loadTexture(srcImageGRAY, GL_LUMINANCE);
	float xTexCoord, yTexCoord;
	xTexCoord = srcImageGRAY->width / (float)textureBufferWidth;
	yTexCoord = srcImageGRAY->height / (float)textureBufferHeight;

	glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D , textureNo);

		glScalef(xScale, yScale, 1.0f);
		glTranslatef(x, y, 1.0f);
		glRotatef(zAngle, 0.0f, 0.0f, 1.0f);

		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0 , yTexCoord);
			glVertex2f(-srcImageGRAY->width / 2.0f, srcImageGRAY->height / 2.0f);

			glTexCoord2f(0 , 0);
			glVertex2f(-srcImageGRAY->width / 2.0f, -srcImageGRAY->height / 2.0f);

			glTexCoord2f(xTexCoord , 0);
			glVertex2f(srcImageGRAY->width / 2.0f, -srcImageGRAY->height / 2.0f);

			glTexCoord2f(xTexCoord , yTexCoord);
			glVertex2f(srcImageGRAY->width / 2.0f, srcImageGRAY->height / 2.0f);
		glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D , 0);
	glDeleteTextures(1, &textureNo);
	glDisable(GL_TEXTURE_2D);

} // renderGRAY

void Renderer::renderMatGRAY(Mat srcImageGRAY, float x, float y, float xScale, float yScale, float zAngle){

	unsigned int textureNo = loadTextureMat(srcImageGRAY, GL_LUMINANCE);
	float xTexCoord, yTexCoord;
	xTexCoord = srcImageGRAY.cols / (float)textureBufferWidth;
	yTexCoord = srcImageGRAY.rows / (float)textureBufferHeight;

	glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D , textureNo);

		glScalef(xScale, yScale, 1.0f);
		glTranslatef(x, y, 1.0f);
		glRotatef(zAngle, 0.0f, 0.0f, 1.0f);

		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0 , yTexCoord);
			glVertex2f(-srcImageGRAY.cols / 2.0f, srcImageGRAY.rows / 2.0f);

			glTexCoord2f(0 , 0);
			glVertex2f(-srcImageGRAY.cols / 2.0f, -srcImageGRAY.rows / 2.0f);

			glTexCoord2f(xTexCoord , 0);
			glVertex2f(srcImageGRAY.cols / 2.0f, -srcImageGRAY.rows / 2.0f);

			glTexCoord2f(xTexCoord , yTexCoord);
			glVertex2f(srcImageGRAY.cols / 2.0f, srcImageGRAY.rows / 2.0f);
		glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D , 0);
	glDeleteTextures(1, &textureNo);
	glDisable(GL_TEXTURE_2D);

} // renderMatGRAY

/* 矩形 */
void Renderer::renderRectangle(float x1, float y1, float x2, float y2, float r, float g, float b, float lineSize){
	glLineWidth(lineSize);
	glPolygonMode( GL_FRONT, GL_LINE );
	glBegin(GL_QUADS);
		glColor3f(r, g, b);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
	glEnd();
	glPolygonMode( GL_FRONT, GL_FILL );
} // renderRectangle
void Renderer::renderRectangleDisplayCoord(float x1, float y1, float x2, float y2, float r, float g, float b, float lineSize){
	glLineWidth(lineSize);
	glPolygonMode( GL_FRONT, GL_LINE );
	glBegin(GL_QUADS);
		glColor3f(r, g, b);
		glVertex2f(x1 - windowWidth / 2.0f, -y1 + windowHeight / 2.0f);
		glVertex2f(x1 - windowWidth / 2.0f, -y2 + windowHeight / 2.0f);
		glVertex2f(x2 - windowWidth / 2.0f, -y2 + windowHeight / 2.0f);
		glVertex2f(x2 - windowWidth / 2.0f, -y1 + windowHeight / 2.0f);
	glEnd();
	glPolygonMode( GL_FRONT, GL_FILL );
} // renderRectangleDisplayCoord

/* 点 */
void Renderer::renderPoint(float x, float y, float r, float g, float b, float pointSize){
	glPointSize(pointSize);
	glBegin(GL_POINTS);
		glColor3f(r, g, b);
		glVertex2f(x, y);
	glEnd();
} // renderPoint
void Renderer::renderPointDisplayCoord(float x, float y, float r, float g, float b, float pointSize){
	glPointSize(pointSize);
	glBegin(GL_POINTS);
		glColor3f(r, g, b);
		glVertex2f(x - windowWidth / 2.0f, -y + windowHeight / 2.0f);
	glEnd();
} // renderPointDisplayCoord

/* 線 */
void Renderer::renderLine(float x1, float y1, float x2, float y2, float r, float g, float b, float lineSize){
	glLineWidth(lineSize);
	glBegin(GL_LINES);
		glColor3f(r, g, b);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
} // renderLine
void Renderer::renderLineDisplayCoord(float x1, float y1, float x2, float y2, float r, float g, float b, float lineSize){
	glLineWidth(lineSize);
	glBegin(GL_LINES);
		glColor3f(r, g, b);
		glVertex2f(x1 - windowWidth / 2.0f, -y1 + windowHeight / 2.0f);
		glVertex2f(x2 - windowWidth / 2.0f, -y2 + windowHeight / 2.0f);
	glEnd();
} // renderLineDisplayCoord

/* Get Width and Height */
int Renderer::getScreenWidth(){
	return windowWidth;
} // getScreenWidth
int Renderer::getScreenHeight(){
	return windowHeight;
} // getScreenHeight

/* Constructor / Destructor */
Renderer::Renderer(){
	isInitialized = 0;
}
Renderer::~Renderer(){
}