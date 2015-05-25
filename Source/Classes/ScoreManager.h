#ifndef __SCORE__MANAGER__H__
#define __SCORE__MANAGER__H__

#include "cocos2d.h"
#include "ScoreData.h"
#include "Note.h"
#include "NoteTap.h"
#include "NoteTapSprite.h"
//#include "NoteHold.h"
//#include "NoteHoldSprite.h"
#include "OnePhraseScore.h"
#include "ValueEffect.h"
#include "Navigator.h"
#include "GradeManager.h"

using namespace cocos2d;


class ScoreManager:public cocos2d::CCObject{
	public:
		ScoreManager(MusicName musicName,CCLayer *layer);
		~ScoreManager();
		//更新処理
		virtual bool update(int gameTime,float delta);

		//初期化
		virtual void init(MusicName musicName,CCLayer *layer);

		//タップを判定する
		bool checkTap(int touch_x,int touch_y,GradeManager* gradeMnager);

		//シェイクを判定する
		bool checkShake(GradeManager* gradeMnager);

		//リリースを判定する
		bool checkRelease(int touch_x,int touch_y,GradeManager* gradeMnager);

		//ホールドを判定する
		bool checkHold(int touch_x,int touch_y,GradeManager* gradeMnager);

		//ポーズ時にアニメーションを一時停止する
		void pauseAnimation();
		//ポーズ解除時にアニメーションを再開する
		void resumeAnimation();


	private:
		//BPM
		int m_bpm;
		//拍子
		float mRhythm;
		//１フレーズの長さ
		int mOnePhraseFrame;
		//楽譜の1フレーズを何小節分とるか
		int mOnePhraseBarNumber;
		float mStep;

		//楽譜データ
		ScoreData::NoteType mScoreData[SCORE_MAX_SIZE];

		//フレーズがいくつあるか
		int mScoreMaxNumber;

		//現在のフレーズ
		int mCurrentPhraseNumber;

		//１フレーズの基準フレーム数
		int mBaseFrame;

		//１フレーズの楽譜		
		CCArray* mPhraseQueue;
		//キュー配列の数
		int mQueueSize;
		//キューの何番目が現在地か
		int mCurrentNumberOfQueue;

		//フレーズごとにフレームを進める
		virtual bool updateFrame(OnePhraseScore* phrase,int frame,float delta);

		//ノートタップの画像管理
		CCArray* mNoteTapSprite;
		static const int mNoteTapSpriteNumber = 10;

		//ノートホールドの画像管理
		CCArray* mNoteHoldSprite;
		static const int mNoteHoldSpriteNumber = 4;

		//ノート情報を画像に登録する
		void setNoteData(OnePhraseScore* phrase);

		//評価画像管理
		ValueEffect* mValueEffect;

		//ナビ
		Navigator* mNavi;

		//ナビの更新
		void setNaviNote();

		//ナビが有効か
		bool mNaviUsed;

};


#endif //__SCORE__MANAGER__H__