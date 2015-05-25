#ifndef __NOTE__HOLD__H__
#define __NOTE__HOLD__H__

#include "cocos2d.h" 
#include "Note.h"
#include "NoteHoldSprite.h"
#include "SSPlayer.h"
using namespace cocos2d;


class NoteHold:public Note{
	typedef Note base;
	public:
		NoteHold();
		NoteHold(CCPoint startCood,int startFrame,int endFrame);
		NoteHold(CCPoint startCood,CCPoint endCood,int startFrame,int endFrame);
		~NoteHold();
		void init();
     
		//ホールド開始座標
		int mStartPos_x;
		int mStartPos_y;

		//ホールド開始フレーム
		int mStartFrame;

		//ホールド終了座標
		int mEndPos_x;
		int mEndPos_y;

		//ホールド終了フレーム
		int mEndFrame;

		//現在のノートの座標
		int mCurrentPos_x;
		int mCurrentPos_y;

		//開始フレームとのずれ
		int mDeltaFromStart;

		//終了フレームとのずれ
		int mDeltaFromEnd;

		//タップをチェックするかのフラグ
		bool mCheckTapFlag;
		//ホールドをチェックするかのフラグ
		bool mCheckHoldFlag;
		//リリースをチェックするかのフラグ
		bool mCheckReleaseFlag;

		bool update(int currentFrame,float delta);

		//音符タイプを取得する
		base::NoteType getNoteType();

		//ナビを開始する
		void naviStart();

		//ナビが開始したか
		bool isNaviStarted();

		//タップとリリースを判定する
		base::Value checkHit(int touch_x,int touch_y,bool isStart);
		//ホールドを判定する
		bool checkHold(int touch_x,int touch_y);

		//ノート画像を登録する
		void setNoteHoldSprite(NoteHoldSprite* nhSprite);

		//評価画像を登録する
		void setValuePlayer(SSPlayer* valuePlayer,bool isStart);

		//ナビを表示するかのフラグ
		bool mNaviFlag;

		//ナビ用座標を取得する
		CCPoint getNaviCood();

		//開始時のミスか調べる
		bool isStartMissed();

	private:
		//判定用円の半径
		static const int HitRadiusHold = 72;

		//フレームのずれによって評価を取得する
		Note::Value getValue(bool isStart);

		//登録した画像のポインタを持たせる
		NoteHoldSprite* mNoteHoldSprite;
		//ノートのアニメーションが終了したか
		bool mNoteAnimationEnded;

		//判定開始したか
		bool mCheckTapStarted;
		bool mCheckReleaseStarted;

		//開始時の評価画像
		SSPlayer* mValuePlayerStart;
		//開始評価画像がアクティブか
		bool mValueActiveStart;
		//終了時の評価画像
		SSPlayer* mValuePlayerEnd;
		//評価画像がアクティブか
		bool mValueActiveEnd;

		//現在のノートの座標を取得する
		void getCurrentCood(int* current_x,int* current_y, int currentFrame);
};


#endif //__NOTE__HOLD__H__