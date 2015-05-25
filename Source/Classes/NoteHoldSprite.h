#ifndef __NOTE__HOLD__SPRITE__H__
#define __NOTE__HOLD__SPRITE__H__

#include "cocos2d.h" 
#include "Note.h" 
#include "SSPlayer.h"
using namespace cocos2d;


class NoteHoldSprite:public CCObject{
	public:
		NoteHoldSprite(SSImageList* effectImage);
		~NoteHoldSprite();
		void init();

		//エフェクト類用イメージリスト
		SSImageList* mEffectImage;

		//ノートプレイヤー
		SSPlayer* mNotePlayer;

		//開始時サークルプレイヤー
		SSPlayer* mCirclePlayerStart;

		//終了時サークルプレイヤー
		SSPlayer* mCirclePlayerEnd;

		//ノートエフェクト
		SSPlayer* mNoteEffectPlayer;

		bool update(int currentFrame,float delta);

		//画像が使われてるか
		bool isActive();

		void setPlayer(CCLayer *layer);

		void setNoteData(int startPos_x,int startPos_y,int endPos_x,int endPos_y,int startFrame,int endFrame);

		void setPressed(bool pressed);
		void setReleased(bool released);
	private:
		bool mActive;

		//ホールド開始フレーム
		int mStartFrame;

		//ホールド終了フレーム
		int mEndFrame;

		//開始フレームとのずれ
		int mDeltaFromStart;

		//終了フレームとのずれ
		int mDeltaFromEnd;

		//開始サークル用フラグ
		bool mCircleStartFirstFlag;
		//終了サークル用フラグ
		bool mCircleEndFirstFlag;
		//ノート用フラグ
		bool mNoteFirstFlag;
		
		//押されたか
		bool mPressed;
		//離されたか
		bool mReleased;
};


#endif //__NOTE__HOLD__SPRITE__H__