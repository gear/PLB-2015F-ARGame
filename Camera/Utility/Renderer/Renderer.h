/****************************************************************
	Renderer Class	「Renderer.h」

	Overview:
		Create Window, Render Camera Image using OpenGL and SDL

	Date:
		Create	2009/01/16 by SATO Toshiki
		Updated	2013/04/03 by SATO Toshiki (IplImage is rendered on 4 vertices mesh instead of using glDrawPixels())
		Updated 2015/02/22 by SATO Toshiki (Mat Support)
 ****************************************************************/
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/* SDL */
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

/* OpenCV */
#include <opencv2/opencv.hpp>
using namespace cv;

class Renderer{

public:

	static Renderer *getInstance();
	static void releaseInstance();

	/* SDLの初期化 */
	static int initializeSDL(){
		if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
			printf("SDLの初期化に失敗しました: %s\n", SDL_GetError());
			return -1;
		}
		return 0;
	} // initializeSDL

	static void finalizeSDL(){
		SDL_Quit();
	} // finalizeSDL

	/* カメラの構築 */
	void setCamera();

	/****************************************************
							Begin / End
	 ****************************************************/
	/* Begin Rendering */
	virtual void begin();

	/* End Rendering */
	void end();

	/****************************************************
								IplImage Rendering
	 ****************************************************/

	/* 3ch RGB Image */
	void renderRGB(IplImage *srcImageRGB, float x, float y, float xScale = 1.0f, float yScale = 1.0f, float zAngle = 0.0f);
	void renderMatRGB(Mat srcImageRGB, float x, float y, float xScale = 1.0f, float yScale = 1.0f, float zAngle = 0.0f);

	/* 4ch RGBA Image */
	void renderRGBA(IplImage *srcImageRGBA, float x, float y, float xScale = 1.0f, float yScale = 1.0f, float zAngle = 0.0f);
	void renderMatRGBA(Mat srcImageRGBA, float x, float y, float xScale = 1.0f, float yScale = 1.0f, float zAngle = 0.0f);

	/* 1ch Grayscale Image */
	void renderGRAY(IplImage *srcImageGRAY, float x, float y, float xScale = 1.0f, float yScale = 1.0f, float zAngle = 0.0f);
	void renderMatGRAY(Mat srcImageGRAY, float x, float y, float xScale = 1.0f, float yScale = 1.0f, float zAngle = 0.0f);
	//void renderGRAY16U(IplImage *srcImageGRAY16U, float x, float y, float xScale = 1.0f, float yScale = 1.0f, float zAngle = 0.0f);

	/****************************************************
								Initialize / Finalize
	 ****************************************************/

	/* Initialize */
	int initialize(int windowWidth, int windowHeight, int cameraImageWidth, int cameraImageHeight, int depth = 24, int isFullScreenMode = 0);

	/* Finalize */
	void finalize();

	/* get Screen Width and Height */
	int getScreenWidth();
	int getScreenHeight();

	/****************************************************
								図形描画
	 ****************************************************/

	/* 矩形 */
	void renderRectangle(float x1, float y1, float x2, float y2, float r, float g, float b, float lineSize);
	void renderRectangleDisplayCoord(float x1, float y1, float x2, float y2, float r, float g, float b, float lineSize);

	/* 点 */
	void renderPoint(float x, float y, float r, float g, float b, float pointSize);
	void renderPointDisplayCoord(float x, float y, float r, float g, float b, float pointSize);

	/* 線 */
	void renderLine(float x1, float y1, float x2, float y2, float r, float g, float b, float lineSize);
	void renderLineDisplayCoord(float x1, float y1, float x2, float y2, float r, float g, float b, float lineSize);

private:

	int windowWidth, windowHeight, depth;
	int cameraImageWidth, cameraImageHeight;
	int textureBufferWidth, textureBufferHeight;
	int isFullScreenMode;
	int isInitialized;
	static Renderer *renInstance;

	IplImage *renderingBuffer4ch, *renderingBuffer3ch, *renderingBuffer1ch, *renderingBuffer1ch16U;
	Mat renderingBufferMat4ch, renderingBufferMat3ch, renderingBufferMat1ch, renderingBufferMat1ch16U;

	Renderer();
	virtual ~Renderer();

	/* Create OpenGL Texture */
	unsigned int loadTexture(IplImage *srcImage, GLenum format){

		unsigned int textureNo = 0;

		switch (srcImage->nChannels){

			case 1:

				/* 8U */
				if (srcImage->depth == 8){

					cvSetImageROI(renderingBuffer1ch, cvRect(0, 0, srcImage->width, srcImage->height));
					cvCopy(srcImage, renderingBuffer1ch);

					glGenTextures(1, &textureNo);
					glBindTexture(GL_TEXTURE_2D, textureNo);

					/* Set Texture Parameters */
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

					/* Bind Texture */
					glTexImage2D(GL_TEXTURE_2D, 0, format, textureBufferWidth, textureBufferHeight, 0, format, GL_UNSIGNED_BYTE, renderingBuffer1ch->imageData);				
				}
				break;

			case 3:
				cvSetImageROI(renderingBuffer3ch, cvRect(0, 0, srcImage->width, srcImage->height));
				cvCvtColor(srcImage, renderingBuffer3ch, CV_BGR2RGB);

				glGenTextures(1, &textureNo);
				glBindTexture(GL_TEXTURE_2D, textureNo);

				/* Set Texture Parameters */
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				/* Bind Texture */
				glTexImage2D(GL_TEXTURE_2D, 0, format, textureBufferWidth, textureBufferHeight, 0, format, GL_UNSIGNED_BYTE, renderingBuffer3ch->imageData);
				break;

			case 4:
				cvSetImageROI(renderingBuffer4ch, cvRect(0, 0, srcImage->width, srcImage->height));
				cvCvtColor(srcImage, renderingBuffer4ch, CV_BGRA2RGBA);
				cvResetImageROI(renderingBuffer4ch);
	
				glGenTextures(1, &textureNo);
				glBindTexture(GL_TEXTURE_2D, textureNo);

				/* Set Texture Parameters */
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				/* Bind Texture */
				glTexImage2D(GL_TEXTURE_2D, 0, format, textureBufferWidth, textureBufferHeight, 0, format, GL_UNSIGNED_BYTE, renderingBuffer4ch->imageData);
				break;

			default:
				return 0;
		} // switch

		return textureNo;
	} // loadTexture

	/* Create OpenGL Texture */
	unsigned int loadTextureMat(Mat srcImage, GLenum format){

		unsigned int textureNo = 0;

		Rect roi;
		Mat roiImage;
		switch (srcImage.channels()){

			case 1:

				/* 8U */
				if (srcImage.depth() == CV_8U){		// depth()の戻り値はビット深度値ではないので注意
					roi.x = roi.y = 0;
					roi.width = srcImage.cols;
					roi.height = srcImage.rows;
					roiImage = renderingBufferMat1ch(roi);
					srcImage.copyTo(roiImage);

					glGenTextures(1, &textureNo);
					glBindTexture(GL_TEXTURE_2D, textureNo);

					/* Set Texture Parameters */
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

					/* Bind Texture */
					glTexImage2D(GL_TEXTURE_2D, 0, format, textureBufferWidth, textureBufferHeight, 0, format, GL_UNSIGNED_BYTE, roiImage.data);				
				}
				break;

			case 3:
				roi.x = roi.y = 0;
				roi.width = srcImage.cols;
				roi.height = srcImage.rows;
				roiImage = renderingBufferMat3ch(roi);
				cvtColor(srcImage, renderingBufferMat3ch, CV_BGR2RGB);

				glGenTextures(1, &textureNo);
				glBindTexture(GL_TEXTURE_2D, textureNo);

				/* Set Texture Parameters */
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				/* Bind Texture */
				glTexImage2D(GL_TEXTURE_2D, 0, format, textureBufferWidth, textureBufferHeight, 0, format, GL_UNSIGNED_BYTE, roiImage.data);
				break;

			case 4:
				roi.x = roi.y = 0;
				roi.width = srcImage.cols;
				roi.height = srcImage.rows;
				roiImage = renderingBufferMat4ch(roi);
				cvtColor(srcImage, renderingBufferMat4ch, CV_BGRA2RGBA);
	
				glGenTextures(1, &textureNo);
				glBindTexture(GL_TEXTURE_2D, textureNo);

				/* Set Texture Parameters */
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				/* Bind Texture */
				glTexImage2D(GL_TEXTURE_2D, 0, format, textureBufferWidth, textureBufferHeight, 0, format, GL_UNSIGNED_BYTE, roiImage.data);
				break;

			default:
				return 0;
		} // switch

		return textureNo;
	} // loadTextureMat

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