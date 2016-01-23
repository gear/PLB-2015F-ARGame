#pragma once

#include <stdlib.h>
#include <vector>
using namespace std;

/* Window size */
static const int WINDOW_WIDTH = 752;				// must be same as the camera resolution
static const int WINDOW_HEIGHT = 480;
//static const int WINDOW_WIDTH = 1920;
//static const int WINDOW_HEIGHT = 1080;

typedef struct{
	unsigned int id;
	unsigned int lastUpdatedTime;
	float x, y;
	float size;
} TrackingObject;

/* 状態 - State */
typedef enum {
	STATE_PROCESS_IMAGE,
} FLASHLIGHT_DETECTION_STATE;

/* マーカコマンド - Marker Commands */
typedef enum{
	MARKER_COMMAND_LIGHT_ON = 1,											// ライトのOn
	MARKER_COMMAND_LIGHT_OFF = 2,											// ライトのOff
	MARKER_COMMAND_SWITCH_TO_NAVIGATION_MODE = 3,			// ナビゲーションモードへ
	MARKER_COMMAND_SWITCH_TO_AUGMENTATION_MODE = 4,		// 情報重畳モードへ
} MARKER_COMMAND;

/* シャッタスピードモード */
typedef enum{
	IMAGE_PROCESSING_MODE_FAR = 1,			// 明るくする
	IMAGE_PROCESSING_MODE_NEAR = 2,			// 暗くする
	IMAGE_PROCESSING_MODE_MARKER_DETECTION_ONLY = 3,			// マーカ検出のみ
} IMAGE_PROCESSING_MODE;

/* 結果表示タイプ */
typedef enum{
	RESULT_MODE_DEBUG_SRC_IMAGE,
	RESULT_MODE_DEBUG_BINARIZED_IMAGE,
	RESULT_MODE_DEBUG_RESULT_IMAGE,
	RESULT_MODE_DEBUG_NO_IMAGE,
	RESULT_MODE_SENSOR_DATA,					// センサデータ
	RESULT_MODE_APPLICATION,					// アプリ画面表示
} RESULT_MODE;

/* 2次元ベクトル */
typedef struct{
	float x;
	float y;
} Vector2f;

/* 3次元ベクトル */
typedef struct{
	float x;
	float y;
	float z;
} Vector3f;

/* 緯度経度 */
typedef struct{
	double latitude;
	double longitude;
} Location;

static const int MAX_TRACKING_OBJECTS = 10;

/* 加速度センサキャリブレーションデータ */
typedef struct{
	Vector3f accelerationOffset;	// 各軸の無負荷時のセンサ値
	Vector3f gyroOffset;					// 同じくジャイロ
	float gravityAccelerationOffset;		// 重力加速度の測定結果
} SensorOffsetData;

/*******************************************************
											結果データ
 *******************************************************/

/* 画像処理結果データ */
typedef struct{
	TrackingObject trackingObjectArray[MAX_TRACKING_OBJECTS];
	IMAGE_PROCESSING_MODE imageProcessingMode;
	char detectedMarkerID;		// 未検出時は-1
	float detectedMarkerPositionX;
	float detectedMarkerPositionY;
} ImageProcessingResult;

/* センサデータ */
typedef struct{
	Vector3f acceleration;		// 16bit x, y and z
	Vector3f gyro;						// 16bit x, y and z
	Vector3f compass;
	Location location;
	unsigned char numSatellites;							// 衛星の数
} RawSensorData;

/* 処理したセンサデータ */
typedef struct{
	float accelerationXYZ;
	float gyroXYZ;
	float orientationXY;
} ProcessedSensorData;

/* ネットワークデータ */
typedef struct{
	ImageProcessingResult imageProcessingResult;
	RawSensorData rawSensorData;
	ProcessedSensorData processedSensorData;
} NetworkData;

/* RS232Cパケットサイズ */
static const int RS232C_PACKET_SIZE = 27;		// 2byte x 3axis x 2sensors + 4byte * 3axis + 4byte(CompassXY) + 4byte(Latitude) + 4byte(Longitude) = 36byte
static const int PACKET_WITH_HEADER_SIZE = (RS232C_PACKET_SIZE + 2);					// 頭・終端文字を含む1パケットサイズ
static const int PACKET_SIZE_WITHOUT_HEADER = (PACKET_WITH_HEADER_SIZE - 1);			// ヘッダ文字を除いたパケットサイズ
