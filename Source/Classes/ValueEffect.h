#ifndef __VALUE__EFFECT__H__
#define __VALUE__EFFECT__H__

#include "cocos2d.h" 
#include "SS5Player.h"
#include "Note.h"
using namespace cocos2d;


class ValueEffect:public CCObject{
	public:
		ValueEffect();
		~ValueEffect();
		void init();

		//評価文字プレイヤー
		CCArray* mValuePlayer;

		void setPlayer(CCLayer *layer);

		//評価ごとに空いているプレイヤーを渡す
		ss::Player* getValuePlayer(Note::Value value);

		//ポーズ
		void pause();

		//ポーズ解除
		void resume();

	private:
		static const int ArrayNumber = 10;
};


#endif //__VALUE__EFFECT__H__