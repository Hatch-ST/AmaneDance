#ifndef __MUSIC__PROGRESS__H__
#define __MUSIC__PROGRESS__H__

#include "cocos2d.h"
#include "MusicData.h"

using namespace cocos2d;

//１フレーズの楽譜を作るクラス
class MusicProgress{
	public:
		MusicProgress(MusicName musicName,CCLayer* layer);
		~MusicProgress();
		void init(CCLayer* layer);
		
		//アニメーション開始
		void startProgressAnim();
		//アニメーション一時停止
		void pauseProgressAnim(float currentTime);
		

	private:
		//曲の再生した時間
		float mCurrentTime;
		//曲の長さ(秒)
		float mMusicTime;
		//プログレスバー
		CCSprite* mProgressBar;
		//プログレスアイコン
		CCSprite* mProgressIcon;
		//アイコンの座標
		int mStartPos_x;
		int mStartPos_y;
		int mEndPos_x;
		int mEndPos_y;


};

#endif //__MUSIC__PROGRESS__H__