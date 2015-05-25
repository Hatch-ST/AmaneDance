#include "TitleScene.h"

#ifdef G_MUSIC_NAME
MusicName gMusicName;
#else
extern MusicName gMusicName;
#endif


USING_NS_CC;

CCScene* Title::scene()
{
	CCScene *scene = SceneCreater::create();
	SceneCreater* sceneCreater = (SceneCreater*)scene;
	sceneCreater->setSceneName(SceneCreater::TITLE);
	Title *layer = Title::create();
	scene->addChild(layer);
	return scene;
}


bool Title::init()
{
	srand((unsigned int)time(NULL));
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//SSデータ読み込み
	ss::ResourceManager *resourceManager = ss::ResourceManager::getInstance();
	resourceManager->addData("chr_convert/tk01.ssbp");
	resourceManager->addData("effect_convert/effect.ssbp");
	resourceManager->addData("stage_convert/stageanim.ssbp");
	resourceManager->addData("titlebg_convert/titlebg.ssbp");

	mTitleBGPlayer = ss::Player::create();
	mTitleBGPlayer->setData("titlebg");
	mTitleBGPlayer->play("titlebg/anime_1");
	mTitleBGPlayer->setPosition( ccp(visibleSize.width/2 , visibleSize.height/2) );
	this->addChild(mTitleBGPlayer);

	this->scheduleUpdate();
	this->setTouchEnabled(true);

	//タッチマネージャー初期化
	int touchNum = 1;
	mTouchManager = new TouchManager(touchNum);

	//タイトルロゴ
	CCSprite* titleLogo = CCSprite::create("interface/amanetitle.png");
	titleLogo->setPosition( ccp(visibleSize.width * 0.5f, visibleSize.height - titleLogo->getContentSize().height/2 - 50) ); 
	this->addChild(titleLogo,1);

	

	mSSPlayer = ss::Player::create();
	mSSPlayer->setData("tk01");
	mSSPlayer->play("tk01/walk01");
	mSSPlayer->setPosition(ccp(visibleSize.width/2 , visibleSize.height/2 - 350) );
	mSSPlayer->setStep(0.75f);
	this->addChild(mSSPlayer);

	//メニュー
	//ゲームスタート
	CCSprite* startButton = CCSprite::create("interface/btn_gamestart.png");
	startButton->setOpacity(200);
	CCSprite* startButtonSelected = CCSprite::create("interface/btn_gamestart.png");
	startButtonSelected->setOpacity(200);
	startButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pStartItem = CCMenuItemSprite::create(
		startButton,
		startButtonSelected,
		this,
		menu_selector(Title::gameStartCallback));
	pStartItem->setPosition(ccp(visibleSize.width/2 , pStartItem->getContentSize().height/2 +300));

	//オプション
	CCSprite* optionButton = CCSprite::create("interface/btn_option.png");
	optionButton->setOpacity(200);
	CCSprite* optionButtonSelected = CCSprite::create("interface/btn_option.png");
	optionButtonSelected->setOpacity(200);
	optionButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pOptionItem = CCMenuItemSprite::create(
		optionButton,
		optionButtonSelected,
		this,
		menu_selector(Title::optionCallback));
	pOptionItem->setPosition(ccp(visibleSize.width/2 , pOptionItem->getContentSize().height/2 +175));

	//終了
	CCSprite* exitButton = CCSprite::create("interface/btn_exit.png");
	exitButton->setOpacity(200);
	CCSprite* exitButtonSelected = CCSprite::create("interface/btn_exit.png");
	exitButtonSelected->setOpacity(200);
	exitButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pExitItem = CCMenuItemSprite::create(
		exitButton,
		exitButtonSelected,
		this,
		menu_selector(Title::exitCallback));
	pExitItem->setPosition(ccp(visibleSize.width/2 , pExitItem->getContentSize().height/2 + 50));

	CCMenu* pMenu = CCMenu::create(pStartItem,pOptionItem,pExitItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu,1);

	mOptionOpened = false;


	return true;

}

//更新処理
void Title::update(float delta){
	mTouchManager->update(delta);

}

//レイヤーが表示されたとき
void Title::onEnter(){
    CCLayer::onEnter();
	//ハードキー有効
	this->setKeypadEnabled(true);
}

void Title::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
        mTouchManager->setTouchBegan(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void Title::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
		mTouchManager->setTouchMoved(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void Title::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
		mTouchManager->setTouchCancelled(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void Title::ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
        mTouchManager->setTouchEnded(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
	}
}

void Title::keyBackClicked(){
	if(!mOptionOpened){
		exitApp();
	}
	//オプション画面の時
	else{
		this->removeChildByTag(mOptionTagNum);
		closeOption();
	}
}

void Title::keyMenuClicked(){
	if(!mOptionOpened){
		openOption();
	}
}

void Title::gameStartCallback(CCObject* pSender){
	//ゲーム開始
	this->unscheduleUpdate();
	CCScene* nextScene = MusicSelect::scene();
	CCScene* pScene = CCTransitionFlipX::create(1.0f,nextScene);
	if(pScene){
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

void Title::optionCallback(CCObject* pSender){
	openOption();
}

void Title::exitCallback(CCObject* pSender){
	exitApp();
}

void Title::exitApp(){
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
		CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	#else
		CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
	#endif
}

void Title::openOption(){
	//オプションレイヤー呼び出し
	mOptionOpened = true;
	OptionLayer* optionLayer = OptionLayer::create();
	this->addChild(optionLayer,2,mOptionTagNum);
	optionLayer->setParentLayer(this);
}

void Title::closeOption(){
	mOptionOpened = false;
}

