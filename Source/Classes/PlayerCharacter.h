#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "SSPlayer.h"
#include "SS5Player.h"
#include "MotionData.h"

//プレイヤー管理クラス
class PlayerCharacter{
	public:
		//初期化
		virtual void init(MusicName musicName);

		//コンストラクタ
		PlayerCharacter(MusicName musicName);

		//更新処理
		virtual bool update(int gameTime,float delta);

		//SSPlayerのセット
		virtual void setSSPlayer(cocos2d::CCLayer *layer);

		//ミスしたか
		bool mIsMissed;
		//ミスモーション
		MotionData::PlayerMotion mMissMotion;

		//ポーズ
		void pause();

		//ポーズ解除
		void resume();
	private:
		//プレイヤーの座標
		//cocos2d::CCPoint* mPlayerPos;
		//イメージリスト
		//SSImageList*	mImageList;
		//SSプレイヤー
		ss::Player*		mSSPlayer;

		//バックライトイメージ
		//SSImageList*	mBackImage;
		//バックライトSSプレイヤー
		ss::Player*	mBackSSPlayer;

		//描画番号
		int mDrawNumber;
		//BPM
		int m_bpm;
		//拍子
		float mRhythm;
		//１フレーズの長さ
		int mOnePhraseFrame;
		//ダンスの1フレーズを何小節分とるか
		int mOnePhraseBarNumber;

		//モーションデータ
		MotionData::PlayerMotion mMotionData[MOTION_MAX_SIZE];
		//１フレーム前のモーション
		int mBeforeMotion;
		//曲終了までのモーション数
		int mMotionNumber;

		//モーションを変更する
		virtual void changeMotion(MotionData::PlayerMotion motion,int gameTime);

		//モーションが変更されたか
		bool mMotionChanged;

};

#endif // __PLAYER_H__