#ifndef __PAUSE__LAYER__H__
#define __PAUSE__LAYER__H__

#include "cocos2d.h"
#include "GameScene.h"
#include "TitleScene.h"

using namespace cocos2d;

class PauseLayer:public CCLayer{
	public:
		virtual bool init();
		CREATE_FUNC(PauseLayer);

		virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

		void setParentLayer(CCLayer* parent);
		
	private:
		//レイヤーの優先順位
		static const int mPauseLayerTouchPriority = kCCMenuHandlerPriority - 1;
		//ポーズメニューボタンの優先順位
		static const int mPauseMenuTouchPriority = kCCMenuHandlerPriority - 2;
		//ポーズ中インフォ
		CCSprite* mPauseInfo;
		//暗転用背景
		CCSprite* mBlackBack;

		//戻るボタンが呼ばれたとき
		void pauseReturnCallback(CCObject* pSender);
		//最初からボタンが呼ばれたとき
		void pauseRestartCallback(CCObject* pSender);
		//やめるボタンが呼ばれたとき
		void pauseQuitCallback(CCObject* pSender);

		CCLayer* mParent;
};


#endif //__PAUSE__LAYER__H__