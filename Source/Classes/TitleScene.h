#ifndef __TITLE__SCENE__H__
#define __TITLE__SCENE__H__

#include "cocos2d.h"
#include "SS5Player.h"
#include "TouchManager.h"
#include "MusicData.h"
#include "MusicSelectScene.h"
#include "SceneCreater.h"
#include "OptionLayer.h"

using namespace cocos2d;
class Title : public CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(Title);

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

	//ゲームスタートが押されたとき
	void gameStartCallback(CCObject* pSender);

	//オプションが押されたとき
	void optionCallback(CCObject* pSender);

	//終了ボタンが押されたとき
	void exitCallback(CCObject* pSender);

	//オプションが閉じたとき
	void closeOption();

private:
	TouchManager* mTouchManager;

	//ゲーム終了
	void exitApp();

	ss::Player* mSSPlayer;
	ss::Player* mTitleBGPlayer;

	bool mOptionOpened;

	static const int mOptionTagNum = 101;

	//オプションが開かれたとき
	void openOption();
};

#endif // __TITLE__SCENE__H__
