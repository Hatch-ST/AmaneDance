#include "PauseLayer.h"

bool PauseLayer::init()
{
	srand((unsigned int)time(NULL));
	if ( !CCLayer::init() )
	{
		return false;
	}

	this->setTouchPriority(mPauseLayerTouchPriority);
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

	CCPoint screenSize = CCDirector::sharedDirector()->getWinSize();
	//暗転用背景
	mBlackBack = CCSprite::create("interface/black.png");
	mBlackBack->setPosition( ccp(screenSize.x * 0.5f,screenSize.y * 0.5f) );
	mBlackBack->setOpacity(128);
	this->addChild(mBlackBack,0);

	//ポーズインフォ
	mPauseInfo = CCSprite::create("interface/pause.png");
	mPauseInfo->setPosition( ccp(screenSize.x * 0.5f,screenSize.y * 0.5f) );
	this->addChild(mPauseInfo,1);

	//メニュー
	CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage("interface/button01.png");
	int width = 425;
	int height = 100;
	CCRect rect;
	int num=0;
	//Return
	rect= CCRectMake(0, 0 + num*height , width, height);
	CCSprite* returnButton = CCSprite::createWithTexture(pTexture,rect);
	CCSprite* returnButtonSelected = CCSprite::createWithTexture(pTexture,rect);
	returnButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pReturnItem = CCMenuItemSprite::create(
		returnButton,
		returnButtonSelected,
		this,
		menu_selector(PauseLayer::pauseReturnCallback));
	pReturnItem->setPosition(ccp(screenSize.x/2 , screenSize.y/2 - pReturnItem->getContentSize().height/2 +150));

	//Restart
	num++;
	rect= CCRectMake(0, 0 + num*height , width, height);
	CCSprite* restartButton = CCSprite::createWithTexture(pTexture,rect);
	CCSprite* restartButtonSelected = CCSprite::createWithTexture(pTexture,rect);
	restartButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pRestartItem = CCMenuItemSprite::create(
		restartButton,
		restartButtonSelected,
		this,
		menu_selector(PauseLayer::pauseRestartCallback));
	pRestartItem->setPosition(ccp(screenSize.x/2 , screenSize.y/2 - pRestartItem->getContentSize().height/2));

	//Quit
	num++;
	rect= CCRectMake(0, 0 + num*height , width, height);
	CCSprite* quitButton = CCSprite::createWithTexture(pTexture,rect);
	CCSprite* quitButtonSelected = CCSprite::createWithTexture(pTexture,rect);
	quitButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pQuitItem = CCMenuItemSprite::create(
		quitButton,
		quitButtonSelected,
		this,
		menu_selector(PauseLayer::pauseQuitCallback));
	pQuitItem->setPosition(ccp(screenSize.x/2 , screenSize.y/2 - pQuitItem->getContentSize().height/2-150));

	CCMenu* pMenu = CCMenu::create(pReturnItem,pRestartItem,pQuitItem, NULL);
	pMenu->setPosition(CCPointZero);
	pMenu->setTouchPriority(mPauseMenuTouchPriority);
	this->addChild(pMenu,1);
	return true;

}
//下レイヤーのタッチを無効化する
bool PauseLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

    return true;
     
}

//戻るボタンが呼ばれたとき
void PauseLayer::pauseReturnCallback(CCObject* pSender){
	Game* game = (Game*) mParent;
	game->pauseClosedCallback();
	this->removeFromParentAndCleanup(true);
}

//最初からボタンが呼ばれたとき
void PauseLayer::pauseRestartCallback(CCObject* pSender){
	this->removeFromParentAndCleanup(true);
	//ゲーム画面を作り直す
	CCScene* nextScene = Game::scene();
	CCScene* pScene = CCTransitionFade::create(1.0f,nextScene);
	if(pScene){
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}
//やめるボタンが呼ばれたとき
void PauseLayer::pauseQuitCallback(CCObject* pSender){
	this->removeFromParentAndCleanup(true);
	//タイトル画面へ
	CCScene* nextScene = Title::scene();
	CCScene* pScene = CCTransitionFlipX::create(1.0f,nextScene);
	if(pScene){
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

void PauseLayer::setParentLayer(CCLayer* parent){
	mParent = parent;
}