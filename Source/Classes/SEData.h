#ifndef __SE__DATA__
#define __SE__DATA__

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

enum SEName{
	SE_TAP_NOTE,
	seNumber
};

class SEData{
	public:
		static void setSEVolume(float seVolume);
		static void preloadSE(SEName seName);
		static void unloadSE(SEName seName);
		static int playSE(SEName seName);
		static void stopSE(int id);
		static void resumeSE(int id);
		static void pauseSE(int id);
};

#endif //__SE__DATA__