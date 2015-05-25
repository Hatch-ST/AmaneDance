#ifndef __TOUCH__EFFECT__H__
#define __TOUCH__EFFECT__H__

#include "cocos2d.h" 
#include "SS5Player.h"
using namespace cocos2d;


class TouchEffect:public CCObject{
	public:
		TouchEffect();
		~TouchEffect();
		void init();

		//タッチエフェクト
		ss::Player* mTouchEffectPlayer;

		void update(float delta);

		//画像が使われてるか
		bool isActive();

		void setPlayer(CCLayer *layer);

		void setPosition(int touchPos_x,int touchPos_y);
	private:
		bool mActive;
};


#endif //__TOUCH__EFFECT__H__