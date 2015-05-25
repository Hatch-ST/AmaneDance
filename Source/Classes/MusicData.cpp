#include "MusicData.h"

int MusicData::getMusicBPM(MusicName musicName){
	int bpm = 60;
	switch(musicName){
		case ORETA_TSUBASA:
		case ORETA_TSUBASA_OKE:
			bpm = 160;
			break;
		case PLASTIC_SKY:
		case PLASTIC_SKY_OKE:
			bpm = 140;
			break;
	}
	return bpm;
}

int MusicData::getMusicRhythm(MusicName musicName){
	//4.0f‚ª4”Žq
	float rhythm = 4.0f;
	switch(musicName){
		case ORETA_TSUBASA:
		case ORETA_TSUBASA_OKE:
			rhythm = 4.0f;
			break;
		case PLASTIC_SKY:
		case PLASTIC_SKY_OKE:
			rhythm = 4.0f;
			break;
	}
	return rhythm;
}

//‹È‚Ì’·‚³i•bj‚ðŽæ“¾
int MusicData::getMusicTime(MusicName musicName){
	float musicTime = 0.0f;
	switch(musicName){
		case ORETA_TSUBASA:
		case ORETA_TSUBASA_OKE:
			//4:07
			musicTime = 247.0f;
			break;
		case PLASTIC_SKY:
		case PLASTIC_SKY_OKE:
			//3:11
			musicTime = 191.0f;
			break;
	}
	return musicTime;
}

void MusicData::setBGMVolume(float bgmVolume){
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(bgmVolume);
}

void MusicData::preloadMusic(){
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/oreta_tubasa.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/oreta_tubasa_oke.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/plastic_sky.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/plastic_sky_oke.mp3");
}

void MusicData::playBGM(MusicName musicName){
	switch(musicName){
		case ORETA_TSUBASA:
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/oreta_tubasa.mp3");
			break;
		case ORETA_TSUBASA_OKE:
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/oreta_tubasa_oke.mp3");
			break;
		case PLASTIC_SKY:
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/plastic_sky.mp3");
			break;
		case PLASTIC_SKY_OKE:
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/plastic_sky_oke.mp3");
			break;
	}
}

void MusicData::stopBGM(){
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void MusicData::resumeBGM(){
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void MusicData::pauseBGM(){
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}