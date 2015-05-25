#include "SEData.h"

void SEData::setSEVolume(float seVolume){
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(seVolume);
}

void SEData::preloadSE(SEName seName){
	switch(seName){
		case SE_TAP_NOTE:
			SimpleAudioEngine::sharedEngine()->preloadEffect("se/hitting01.mp3");
			break;
	}
}

void SEData::unloadSE(SEName seName){
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0);
	switch(seName){
		case SE_TAP_NOTE:
			SimpleAudioEngine::sharedEngine()->unloadEffect("se/hitting01.mp3");
			break;
	}
}

int SEData::playSE(SEName seName){
	switch(seName){
		case SE_TAP_NOTE:
			return SimpleAudioEngine::sharedEngine()->playEffect("se/hitting01.mp3");
	}
}

void SEData::stopSE(int id){
	SimpleAudioEngine::sharedEngine()->stopEffect(id);
}

void SEData::resumeSE(int id){
	SimpleAudioEngine::sharedEngine()->resumeEffect(id);
}

void SEData::pauseSE(int id){
	SimpleAudioEngine::sharedEngine()->pauseEffect(id);
}