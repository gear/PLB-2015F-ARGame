/*********************************************************************
	Timerクラス			「Timer.h」

	概要:
		SDLを使った1msec単位のタイマと、CPUのクロックカウンタを使った高分解能タイマ。

	Date:
		Created: 2009/01/16	By 佐藤俊樹
		Updated: 2010/07/10	By 佐藤俊樹	関数名を分かりやすくした
		Updated: 2013/03/07	By 佐藤俊樹	x64用のRDTSCタイマーを実装(インラインアセンブラ廃止)
 *********************************************************************/
#ifndef __Timer_H__
#define __Timer_H__

#pragma warning(disable: 4293)		// シフト幅がでかすぎ的な警告を無視

#include <SDL.h>

#ifndef STRICT
  #define STRICT
#endif

#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif

#ifndef _WINDOWS_
  #include <windows.h>
#endif

#ifndef _INC_MMSYSTEM
  #include <mmsystem.h>
  #pragma comment(lib,"winmm.lib")
#endif

#include <intrin.h>

class Timer{

public:

	/* 現在時刻と前のフレームの時刻 */
	Uint32 systemTime, prevTime;

	/* 1ステップの長さ(msec) */
	float dt;

	/* タイマーを初期化 */
	void initialize(void);

	/******************************************
								Singletonメソッド
	 ******************************************/
	/* Singletonインスタンス生成 */
	static Timer *getInstance();

	/* Singletonインスタンス破棄 */
	static void releaseInstance();

	/***************************************************
												インタフェース
	 ***************************************************/
	/* 時間をセットする関数。
		 シーンクラスから毎ループ呼ばれる必要がある */
	void update(void);

	/******************************************************************
								RDTSC命令を用いたタイマについての関数
	 ******************************************************************/
	/* RDTSC命令でCPUのクロックカウンタレジスタの値を読み出す */
	ULONGLONG getRDTSCClockCount();

	/* クロックカウントをmsecに変換する */
	double getRDTSCMSeconds( ULONGLONG clockCount );

private:

	ULONGLONG cpuClock;			// CPUクロック周波数値

	/***************************
					Timerインスタンス
	 ***************************/
	static Timer *timInstance;
	static int timInitialized;		// 初期化されているかいないか(1: すでに初期化されている, 0: 初期化されていない)

	/***************************************************
							コンストラクタ / デストラクタ
	 ***************************************************/
	/* コンストラクタ */
	Timer();

	/* デストラクタ */
	virtual ~Timer();

	/******************************************************************
						RDTSC命令を用いたタイマ処理についての補助関数
	 ******************************************************************/
	/* プライオリティを設定する */
	void hiPriority( BOOL enable );

	/* CPUの動作クロックの測定(1000msecかかる) */
	void calcCPUClock();

};
#endif