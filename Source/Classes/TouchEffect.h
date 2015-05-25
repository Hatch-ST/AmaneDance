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

		//�^�b�`�G�t�F�N�g
		ss::Player* mTouchEffectPlayer;

		void update(float delta);

		//�摜���g���Ă邩
		bool isActive();

		void setPlayer(CCLayer *layer);

		void setPosition(int touchPos_x,int touchPos_y);
	private:
		bool mActive;
};


#endif //__TOUCH__EFFECT__H__