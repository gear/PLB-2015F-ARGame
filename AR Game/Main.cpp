#include <stdlib.h>
#include <Windows.h>
#include <opencv2/opencv.hpp>

using namespace cv;

#include "Definitions.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_net.h>

/* Utilities */
#include "./Utility/Inputter/Inputter.h"
#include "./Utility/Timer/Timer.h"
#include "./Utility/Renderer/Renderer.h"
#include "./Utility/Renderer/TextRenderer.h"
#include "./Utility/Camera/CaptureDevice_PGRCamera2_FireflyMV.h"
#include "./Utility/ImageProcessing/Binarizer.h"
#include "./Utility/ImageProcessing/RegionLabeler.h"
#include "./Utility/ImageProcessing/ObjectTracker.h"

int flagExit = 0;

/* Utility Classes */
Inputter *inputter;
Timer *timer;
TextRenderer textRenderer;
Renderer *renderer;

// Images
Mat srcImageGRAY;
Mat srcImageGRAYNot;
Mat srcImageGRAYNot4ch;
Mat binarizedImageGRAY;
Mat binarizedImageCopyGRAY;

// Camera
FireflyMV *fireflyMV;

// Camera Settings
static const int CAMERA_INDEX = 0;
static const int CAMERA_ROI_X = 0;
static const int CAMERA_ROI_Y = 0;
static const int CAMERA_ROI_WIDTH = 752;
static const int CAMERA_ROI_HEIGHT = 480;

static const PixelFormat pixelFormat = PIXEL_FORMAT_MONO8;

static const float CAMERA_GAIN = 0.0f;

IMAGE_PROCESSING_MODE imageProcessingMode = IMAGE_PROCESSING_MODE_FAR;

// Binarizing Class
Binarizer binarizer;

// Labelling Class
RegionLabeler regionLabeler;

// Labelling Result
Vector<Region> regions;

// Tracking Class
ObjectTracker tracker;

// Labelling Settings
static const int MAX_REGIONS = 5;
static const double MAX_REGION_SIZE_THRESHOLD = (CAMERA_ROI_WIDTH * CAMERA_ROI_HEIGHT);
static const double MIN_REGION_SIZE_THRESHOLD = 2000.0;

// Tracking Result
vector<TrackingObject> *trackingObjectArrayPtr;

RESULT_MODE resultMode = RESULT_MODE_DEBUG_SRC_IMAGE;

// Switch Rendering ON/OFF for CPU power saving
bool enableRendering = true;
int regionCounter = 0;

// Image Processing FPS
unsigned int imageProcessingInterval = 0;		// set if you want to reduce fps for saving CPU(battery) power

int flagDisableCamera = 0;
double shutterSpeed = 0.2;
double gain = 5.0f;
unsigned int binarizingThreshold = 20;

/*******************************************************************************/

// Initialization
int initialize(){

	// SDL init
	Renderer::initializeSDL();
	TextRenderer::initializeTTF();

	// Renderer init
	renderer = Renderer::getInstance();
	if ( renderer->initialize(WINDOW_WIDTH, WINDOW_HEIGHT, CAMERA_ROI_WIDTH, CAMERA_ROI_HEIGHT) < 0 ){
		printf("Renderer failed.\n");
		return -1;
	}

	// Text Renderer init
	if ( textRenderer.initialize("c:\\windows\\fonts\\Tahoma.ttf", 20, 255, 255, 0, WINDOW_WIDTH, WINDOW_HEIGHT) < 0 ){
		printf("TextRenderer failed.\n");
		return -1;
	}

	// Input init
	inputter = Inputter::getInstance();
	inputter->initializeKeyState();

	// Timer init
	timer = Timer::getInstance();
	timer->initialize();

	// Camera init
	if ( !flagDisableCamera ){

		fireflyMV = FireflyMV::getInstance();
		fireflyMV->setCameraIndex(CAMERA_INDEX);
		if ( fireflyMV->initialize() < 0 ){
			printf("Camera Initialize failedB\n");
			return -1;
		}
		if ( fireflyMV->setCustomImageParameter(CAMERA_ROI_X, CAMERA_ROI_Y, CAMERA_ROI_WIDTH, CAMERA_ROI_HEIGHT, pixelFormat) < 0 ){
			printf("Custom Video Mode Failed.\n");
			return -1;
		}
		fireflyMV->setShutterSpeed(shutterSpeed);		// mSec
		fireflyMV->setGain(gain);

		// Image Processing init
		binarizer.initialize(CAMERA_ROI_WIDTH, CAMERA_ROI_HEIGHT);
		regions.resize(MAX_REGIONS);

		// Image init
		srcImageGRAY = Mat(CAMERA_ROI_HEIGHT, CAMERA_ROI_WIDTH, CV_8UC1);
		binarizedImageGRAY = Mat(CAMERA_ROI_HEIGHT, CAMERA_ROI_WIDTH, CV_8UC1);
		binarizedImageCopyGRAY = Mat(CAMERA_ROI_HEIGHT, CAMERA_ROI_WIDTH, CV_8UC1);
		srcImageGRAYNot = Mat(CAMERA_ROI_HEIGHT, CAMERA_ROI_WIDTH, CV_8UC1);
		srcImageGRAYNot4ch = Mat(CAMERA_ROI_HEIGHT, CAMERA_ROI_WIDTH, CV_8UC4);

		if ( fireflyMV->startCamera() < 0){
			printf("Start Camera failed.\n");
			return -1;
		}

		// Tracking init
		tracker.initialize(MAX_TRACKING_OBJECTS);
		trackingObjectArrayPtr = tracker.getTrackingObjectArrayPtr();

#ifdef _DEBUG
	namedWindow("binarizedImage");
	namedWindow("resultImage");
#endif
	}

	return 0;
} // initialize

// finalize
void finalize(){

#ifdef _DEBUG
	destroyWindow("resultImage");
	destroyWindow("binarizedImage");
#endif

	binarizedImageCopyGRAY.release();
	binarizedImageGRAY.release();
	srcImageGRAY.release();

	if ( !flagDisableCamera ){
		fireflyMV->stopCamera();
		fireflyMV->finalize();
		FireflyMV::releaseInstance();
	}

	//se1.releaseSE();

	textRenderer.finalize();
	renderer->finalize();

	// Finalize SDL
	TextRenderer::finalizeTTF();
	Renderer::finalizeSDL();

} // finalize

// Input Check
void checkInput(){
	
	static Uint32 inputTick = 0;
	inputter->update();

	// q to quit
	if ( (inputter->keyState[SDLK_q].state || inputter->keyState[SDLK_ESCAPE].state) && inputTick + 100 < timer->systemTime ){
		flagExit = 1;
		inputTick = timer->systemTime;

	// Up/Down: Shutter Speed Control
	}else if ( inputter->keyState[SDLK_RIGHT].state && inputTick + 5 < timer->systemTime && !flagDisableCamera ){

		shutterSpeed += 0.1f;
		fireflyMV->setShutterSpeed(shutterSpeed);
		printf("ShutterSpeed: %03.02lf\n", shutterSpeed);
		inputTick = timer->systemTime;

	}else if ( inputter->keyState[SDLK_LEFT].state && inputTick + 5 < timer->systemTime && !flagDisableCamera ){

		shutterSpeed -= 0.1f;
		if (shutterSpeed < 0.0f){
			shutterSpeed = 0.01f;
		}
		fireflyMV->setShutterSpeed(shutterSpeed);
		printf("ShutterSpeed: %03.02lf\n", shutterSpeed);
		inputTick = timer->systemTime;
	
	// Left/Right: Binarizing Threshold Control
	}else if ( inputter->keyState[SDLK_UP].state && inputTick + 5 < timer->systemTime && !flagDisableCamera ){

		binarizingThreshold++;
		if (binarizingThreshold > 255){
			binarizingThreshold = 255;
		}
		printf("BinarizingThreshold: %u\n", binarizingThreshold);
		inputTick = timer->systemTime;

	}else if ( inputter->keyState[SDLK_DOWN].state && inputTick + 5 < timer->systemTime && !flagDisableCamera ){

		binarizingThreshold--;
		if (binarizingThreshold == 0){
			binarizingThreshold = 1;
		}
		printf("BinarizingThreshold: %u\n", binarizingThreshold);
		inputTick = timer->systemTime;

	// 0: Switch Result Type
	}else if ( inputter->keyState[SDLK_0].state && inputTick + 200 < timer->systemTime ){
		if ( resultMode == RESULT_MODE_DEBUG_NO_IMAGE ){
			resultMode = RESULT_MODE_DEBUG_SRC_IMAGE;
		}else{
			int tmp = (int)resultMode;
			tmp++;
			resultMode = (RESULT_MODE)tmp;
		}
		inputTick = timer->systemTime;
	}

} // checkInput

/* Main Loop */
void processImage(){
		
	/* Grab Image */
	fireflyMV->grabImage(srcImageGRAY);

	/* Binarize Image */
	binarizer.calcThreshold(srcImageGRAY, binarizedImageGRAY, binarizingThreshold);
	binarizedImageGRAY.copyTo(binarizedImageCopyGRAY);

	/* Labeling */
	regionCounter = regionLabeler.detectRegion(binarizedImageCopyGRAY,		// binarizedImageCopyGRAY is updated by this function
MAX_REGION_SIZE_THRESHOLD, MIN_REGION_SIZE_THRESHOLD, regions);

	/* Tracking */
	tracker.trackObjects(regions, regionCounter);

} // processImage

/* Result Rendering */
void render(){

	static char tmp[64];

	switch ( resultMode ){
		case RESULT_MODE_DEBUG_NO_IMAGE:
			return;

		case RESULT_MODE_DEBUG_SRC_IMAGE:

			if ( flagDisableCamera ){
				return;
			}
			renderer->begin();

				/* Draw Result */
				for ( int i = 0; i < regionCounter; i++ ){

					/* Centroid */
					circle(srcImageGRAY, Point((int)regions[i].centroid.x, (int)regions[i].centroid.y), 3, Scalar(128, 128, 128));

					/* Bounding Box */
					rectangle(srcImageGRAY, 
						Point(regions[i].boundingRect.x, regions[i].boundingRect.y), 
						Point(regions[i].boundingRect.x + regions[i].boundingRect.width, regions[i].boundingRect.y + regions[i].boundingRect.height),
						Scalar(128, 128, 128), 3);

					/* Inertia Axis */
					line(srcImageGRAY, Point(regions[i].centroid.x + cosf(regions[i].angle) * 100.0f, 
						regions[i].centroid.y + sinf(regions[i].angle) * 100.0f), 
						Point(regions[i].centroid.x - cosf(regions[i].angle) * 100.0f, 
						regions[i].centroid.y - sinf(regions[i].angle) * 100.0f), Scalar(128, 128, 128), 2);
				}

				/* Camera Image */
				renderer->renderMatGRAY(srcImageGRAY, 0, 0);

				/* Camer Image Type */
				textRenderer.renderASCIICenter("Source Image", 440.0f);

			renderer->end();
			break;

		case RESULT_MODE_DEBUG_BINARIZED_IMAGE:

			if ( flagDisableCamera ){
				return;
			}
			renderer->begin();

				for ( int i = 0; i < regionCounter; i++ ){

					circle(binarizedImageGRAY, Point((int)regions[i].centroid.x, (int)regions[i].centroid.y), 3, Scalar(128, 128, 128));

					rectangle(binarizedImageGRAY, 
						Point(regions[i].boundingRect.x, regions[i].boundingRect.y), 
						Point(regions[i].boundingRect.x + regions[i].boundingRect.width, regions[i].boundingRect.y + regions[i].boundingRect.height),
						Scalar(128, 128, 128), 3);

					line(binarizedImageGRAY, Point(regions[i].centroid.x + cosf(regions[i].angle) * 100.0f, 
						regions[i].centroid.y + sinf(regions[i].angle) * 100.0f), 
						Point(regions[i].centroid.x - cosf(regions[i].angle) * 100.0f, 
						regions[i].centroid.y - sinf(regions[i].angle) * 100.0f), Scalar(128, 128, 128), 2);
				}

				renderer->renderMatGRAY(binarizedImageGRAY, 0, 0);

				textRenderer.renderASCIICenter("Binarized Image", 440.0f);

			renderer->end();
			break;

		case RESULT_MODE_DEBUG_RESULT_IMAGE:

			if ( flagDisableCamera ){
				return;
			}
			renderer->begin();

				for ( int i = 0; i < regionCounter; i++ ){

					circle(srcImageGRAY, Point((int)regions[i].centroid.x, (int)regions[i].centroid.y), 3, Scalar(128, 128, 128));

					rectangle(srcImageGRAY, 
						Point(regions[i].boundingRect.x, regions[i].boundingRect.y), 
						Point(regions[i].boundingRect.x + regions[i].boundingRect.width, regions[i].boundingRect.y + regions[i].boundingRect.height),
						Scalar(128, 128, 128), 3);

					line(srcImageGRAY, Point(regions[i].centroid.x + cosf(regions[i].angle) * 100.0f, 
						regions[i].centroid.y + sinf(regions[i].angle) * 100.0f), 
						Point(regions[i].centroid.x - cosf(regions[i].angle) * 100.0f, 
						regions[i].centroid.y - sinf(regions[i].angle) * 100.0f), Scalar(128, 128, 128), 2);
				}

				renderer->renderMatGRAY(srcImageGRAY, 0, 0);

				textRenderer.renderASCIICenter("Result Image", 440);

			renderer->end();
			break;

	} // switch

#ifdef _DEBUG
	if ( !flagDisableCamera ){
		imshow("binarizedImage", binarizedImageGRAY);
		imshow("resultImage", srcImageGRAY);
	}
#endif

} // render

/* Processing */
void process(){

	static Uint32 imageProcessingTick = 0;

	/* Image Processing */
	if (imageProcessingTick + imageProcessingInterval < timer->systemTime && !flagDisableCamera){
		processImage();
		imageProcessingTick = timer->systemTime;
	}

} // process 

/* calc FrameRate */
void calcFrameRate(){
	static Uint32 tick = 0;
	static Uint32 counter = 0;
	counter++;
	if ( tick + 1000 < timer->systemTime ){
		printf("%dfps\n", counter);
		counter = 0;
		tick = timer->systemTime;
	}
}

/*********************************************************************/
int main(int argc, char **argv){

	/* Initialize System */
	if ( initialize() < 0 ){
		printf("Initialize() failed.\n");
		return -1;
	}

	static int counter = 0;

	while (!flagExit){

		calcFrameRate();

		/* Update Timer */
		timer->update();

		/* Check Keyboard Input */
		checkInput();

		/* Processing */
		process();

		/* Result Rendering */
		if ( enableRendering ){
			render();
		}
	} // mainLoop
	
	/* release resources */
	finalize();

	return 0;
}
