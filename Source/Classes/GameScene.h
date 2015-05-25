#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "SS5Player.h"
#include "PlayerCharacter.h"
#include "TouchManager.h"
#include "MusicData.h"
#include "SEData.h"
#include "ScoreManager.h"
#include "Note.h"
#include "ResultScene.h"
#include "MusicProgress.h"
#include "PauseLayer.h"
#include "SceneCreater.h"
#include "GradeManager.h"

enum SpritePriority{
	BackgroundPriority = 0,
	CharacterPriority,
	NotePriority,
	CirclePriority,
	NavigatorPriority,
	ValuePriority,
	TouchEffectPriority,
	BlackPriority,
	InfoPriority,
	InterfacePriority,
	ButtonPriority,
	PausePriority,
};

class Game : public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(Game);

	virtual void update(float delta);

	//シーンが始まった時に呼び出されるメソッド
	virtual void onEnter();
	//シーンが終わった時に呼び出されるメソッド
	virtual void onExit();

	//加速度を取得する
	virtual void didAccelerate(CCAcceleration* pAccel);
	//シングルタッチ
	//virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	
	//マルチタッチ
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	// 画面サイズの取得
	static int screenWidth();
	static int screenHeight();

	//ポーズが押されたとき
	void pauseMenuCallback(CCObject* pSender);
	//ポーズが開始されたとき
	void pauseOpenedCallback();
	//ポーズが解除されたとき
	void pauseClosedCallback();

	//android バックキー
	void keyBackClicked();
	//android メニューキー
	void keyMenuClicked();
private:
	
	float mGameSecond;	//経過時間(秒)
	int mGameFrame;		//経過時間(フレーム)
	PlayerCharacter* mPlayer;
	ScoreManager* mScoreManager;

	TouchManager* mTouchManager;

	//ゲームが開始したか
	bool mGameStarted;
	//ゲームが終了したか
	bool mGameEnded;

	//プログレス
	MusicProgress* mMusicProgress;

	//得点バー
	CCSprite* mGradeBar;
	
	//ゲーム前インフォ
	CCSprite* mTouchToStart;
	//暗転用背景
	CCSprite* mBlackBack;

	//ステージ
	SSPlayer* mStagePlayer;
	SSPlayer* mStageEffect;

	//ポーズメニューボタン
	CCMenu* mMenu;

	//ポーズ中か
	bool mPaused;

	//ポーズレイヤータグ
	static const int mPauseTagNum = 100; 

	//得点管理
	GradeManager* mGradeManager;

	//曲の長さ
	float mMusicTime;

	//加速度の数値
	float mAccel_x;
	float mAccel_y;
	float mAccel_z;

	//シェイクされたか
	bool mShaked;
	//シェイク開始時間
	float mShakeStartTime;
	//1回のシェイク時間
	float mOneShakeTime;

	//シェイクされたか調べる
	bool checkShake();

	//シェイクの感度
	float mShakeLevel;

};

#endif // __GAME_SCENE_H__
