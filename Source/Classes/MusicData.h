#ifndef __MUSIC__DATA__
#define __MUSIC__DATA__

#include "SimpleAudioEngine.h"
#include "cocos2d.h"
using namespace CocosDenshion;
using namespace cocos2d;

enum MusicName{
	ORETA_TSUBASA,
	ORETA_TSUBASA_OKE,
	PLASTIC_SKY,
	PLASTIC_SKY_OKE,
	musicNumber
};

class MusicData{
	public:
		static int getMusicBPM(MusicName musicName);
		static int getMusicRhythm(MusicName musicName);
		static int getMusicTime(MusicName musicName);
		static void setBGMVolume(float bgmVolume);
		static void preloadMusic();
		static void playBGM(MusicName musicName);
		static void stopBGM();
		static void resumeBGM();
		static void pauseBGM();
};

#endif //__MUSIC__DATA__