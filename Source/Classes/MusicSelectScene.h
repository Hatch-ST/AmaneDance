#ifndef __MUSIC__SELECT__SCENE__H__
#define __MUSIC__SELECT__SCENE__H__

#include "cocos2d.h"
#include "SSPlayer.h"
#include "TouchManager.h"
#include "MusicData.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SceneCreater.h"
#include "BestGrade.h"

using namespace cocos2d;
using namespace std;
class MusicSelect : public CCLayer
{
public:
	enum MovingDirection{
		Stop,
		Left,
		Right
	};

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(MusicSelect);

	virtual void update(float delta);

	//シーンが始まった時に呼び出されるメソッド
	virtual void onEnter();
	
	//マルチタッチ
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	//android バックキー
	void keyBackClicked();
	//android メニューキー
	void keyMenuClicked();

	//バックボタンが押されたとき
	void backButtonCallback(CCObject* pSender);

	//スタートボタンが押されたとき
	void startButtonCallback(CCObject* pSender);

	//←ボタンが押されたとき
	void leftButtonCallback(CCObject* pSender);

	//→ボタンが押されたとき
	void rightButtonCallback(CCObject* pSender);

private:
	TouchManager* mTouchManager;

	//タイトルに戻る
	void backToTitle();

	//選択されている曲のインデックス
	int mSelectedMusicIndex;

	//現在の曲リストのページ数
	int mCurrentPageIndex;

	//曲リストのページを移動する
	void movePage(bool isLeft);

	//ビュー用レイヤー
	CCLayer* mViewLayer;

	//レイヤーのページ数
	static const int LayerPageNum = 2;
	//リストの縦に一度に表示する数
	static const int ListNum_y = 5;

	//ページ移動アクション中か
	MovingDirection mPageMoving;

	//ページ移動アクション後に呼ぶ
	void pageMovedCallback(CCNode* sender);

	//←ボタン
	CCMenuItemSprite *mLeftItem;

	//→ボタン
	CCMenuItemSprite *mRightItem;

	//曲リストのタップをチェックする
	void checkTapMusic();

	//曲選択フレーム
	CCSprite* mSelectedFrame;

	//曲リストのスワイプをチェックする
	void checkSwipeMusic();
	//スワイプしたか
	bool mSwiped;
	CCString* getMusicName(int index);
};

#endif // __TITLE_SCENE_H__
