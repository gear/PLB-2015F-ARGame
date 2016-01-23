#include "GraphRenderer.h"

GraphRenderer::GraphRenderer(){
	windowWidth = windowHeight = 0;
	isInitialize = 0;
	graphYPosition = 0;
	graphMagnificationRatio = 1.0f;
	r = g = b = 1.0f;
	lineWidth = 1.0f;
	currentPosition = 0;
	enableXAxisRendering = 1;
	enableCurrentPositionRendering = 1;
	offsetData = 0.0f;
}

GraphRenderer::~GraphRenderer(){
	finalize();
} // GraphRenderer

/* ������ */
int GraphRenderer::initialize(float windowWidth, float windowHeight, int graphYPosition, int enableCurrentPositionRendering, int enableXAxisRendering, float graphMagnificationRatio){
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->graphYPosition = graphYPosition;
	this->graphMagnificationRatio = graphMagnificationRatio;
	this->enableCurrentPositionRendering = enableCurrentPositionRendering;
	this->enableXAxisRendering = enableXAxisRendering;
	dataArray = new float[(int)windowWidth];
	clear();
	isInitialize = 1;
	return 0;
} // initialize

/* ��� */
void GraphRenderer::finalize(){
	if ( isInitialize ){
		delete [] dataArray;
		isInitialize = 0;
	}
} // finalize

/* �`�悷�� */
void GraphRenderer::render(){

	/* X���̕`��*/
	if ( enableXAxisRendering ){
		renderXAxis();
	}
		
	/* ���݈ʒu�̕`�� */
	if ( enableCurrentPositionRendering ){
		renderCurrentPosition();
	}

	/* �f�[�^�̕`�� */
	float x, y, prevX = -windowWidth, prevY = 0.0f;
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
		glColor3f(r, g, b);
		for (float i = -windowWidth / 2.0f; i < windowWidth / 2.0f; i += 1.0f){
			x = i; 
			y = (dataArray[(int)i + (int)(windowWidth / 2)] - offsetData) * graphMagnificationRatio + graphYPosition;
			glVertex2f(x, y);
			glVertex2f(prevX, prevY);
			prevX = x;
			prevY = y;
		}
	glEnd();
} // render

/* �����ڐݒ� */
void GraphRenderer::setRenderingOption(float lineWidth, float r, float g, float b){
	this->r = r;		this->g = g;		this->b = b;
	this->lineWidth = lineWidth;
} // setRenderingOption

/* �N���A */
void GraphRenderer::clear(){
	currentPosition = 0;
	memset(dataArray, 0, sizeof(float) * (int)windowWidth);
} // clear

/* �l�̒ǉ� */
void GraphRenderer::addData(float value, int position){
	if (position < 0 || position >= windowWidth){
		printf("�ǉ��ꏊ���T�C�Y���I�[�o�[���Ă��܂�: %d\n", position);
		return;
	}
	currentPosition = position;
	dataArray[position] = value;
} // addValue

/* �I�t�Z�b�g�l�̃Z�b�g */
void GraphRenderer::setOffsetValue(float value){
	offsetData = value;
} // setOffsetValue

/* �f�[�^�z��̃|�C���^�𓾂� */
float *GraphRenderer::getDataArrayPtr(){
	return dataArray;
} // getDataArrayPtr

/* �f�[�^�T�C�Y��Ԃ� */
int GraphRenderer::getDataArraySize(){
	return (int)windowWidth;
} // int getDataArraySize

/* x���̕`�� */
void GraphRenderer::renderXAxis(){
			
	glLineWidth(3);
	glBegin(GL_LINES);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(-windowWidth / 2, -windowHeight / 2.0f + graphYPosition);
		glVertex2f(windowWidth / 2, -windowHeight / 2.0f + graphYPosition);

	glEnd();
} // renderXAxis

/* ���݈ʒu�̕`�� */
void GraphRenderer::renderCurrentPosition(){
	glLineWidth(3);
	glBegin(GL_LINES);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(currentPosition - windowWidth / 2.0f, windowHeight / 2.0f);
		glVertex2f(currentPosition - windowWidth / 2.0f, -windowHeight / 2.0f);

	glEnd();
} // renderCurrentPosition