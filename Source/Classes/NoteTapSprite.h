#ifndef __NOTE__TAP__SPRITE__H__
#define __NOTE__TAP__SPRITE__H__

#include "cocos2d.h" 
#include "Note.h" 
#include "SS5Player.h"
using namespace cocos2d;


class NoteTapSprite:public CCObject{
	public:
		NoteTapSprite();
		~NoteTapSprite();
		void init();

		//ノートプレイヤー
		ss::Player* mNotePlayer;

		//サークルプレイヤー
		ss::Player* mCirclePlayer;

		//ノートエフェクト
		ss::Player* mNoteEffectPlayer;

		bool update(int currentFrame,float delta);

		//画像が使われてるか
		bool isActive();

		void setPlayer(CCLayer *layer);

		void setNoteData(int notePos_x,int notePos_y,int correctFrame);

		void setPressed(bool pressed);

		void pauseAnimation();
		void resumeAnimation();
	private:
		bool mActive;

		//正解のフレーム
		int mCorrectFrame;

		//正解のフレームとのずれ
		int mDeltaFromCorrect;

		//最初の一回用フラグ
		bool mFirstFlag;
		
		//押されたか
		bool mPressed;
};


#endif //__NOTE__TAP__SPRITE__H__