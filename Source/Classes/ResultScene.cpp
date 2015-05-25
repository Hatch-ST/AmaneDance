#include "ResultScene.h"

#ifdef G_MUSIC_NAME
MusicName gMusicName;
#else
extern MusicName gMusicName;
#endif

#ifdef G_GRADE_DATA
struct GradeData gGradeData;
#else
extern struct GradeData gGradeData;
#endif

USING_NS_CC;

CCScene* Result::scene()
{
	CCScene *scene = SceneCreater::create();
	SceneCreater* sceneCreater = (SceneCreater*)scene;
	sceneCreater->setSceneName(SceneCreater::RESULT);
	Result *layer = Result::create();
	scene->addChild(layer);
	return scene;
}


bool Result::init()
{
	srand((unsigned int)time(NULL));
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite* bgSprite = CCSprite::create("white.png");
	bgSprite->setPosition( ccp(visibleSize.width * 0.5f,visibleSize.height * 0.5f) );
	this->addChild(bgSprite, 0);

	//resultラベル
	CCSprite* resultLabel = CCSprite::create("result/result_label.png");
	resultLabel->setPosition( ccp(visibleSize.width * 0.5f,visibleSize.height - 30 - resultLabel->getContentSize().height * 0.5f) );
	this->addChild(resultLabel, 2);

	//1曲をまとめるレイヤー
	CCLayer* musicLayer = CCLayer::create();
	musicLayer->setContentSize( ccp(visibleSize.width, 150) );
	musicLayer->setPosition( ccp( 0, visibleSize.height - 210 - musicLayer->getContentSize().height) );

	//フレーム
	CCSprite* musicFrame = CCSprite::create("music_frame.png");
	musicFrame->setPosition( ccp(musicLayer->getContentSize().width * 0.5f , musicLayer->getContentSize().height * 0.5f) );
	musicLayer->addChild(musicFrame,1);

	//アイコン
	CCSprite* musicIcon = CCSprite::create("music_icon.png");
	musicIcon->setPosition( ccp( 60 + musicIcon->getContentSize().width * 0.5f , musicLayer->getContentSize().height * 0.5f ) );
	musicLayer->addChild(musicIcon,2);

	this->addChild(musicLayer,2);

	//評価の回数の画像
	mResultGrade = new ResultGrade();
	mResultGrade->setSprite(this);

	//COOL
	CCSprite* coolLabel = CCSprite::create("result/cool_label.png");
	coolLabel->setPosition( ccp(30 + coolLabel->getContentSize().width * 0.5f, visibleSize.height - 390 - coolLabel->getContentSize().height * 0.5f) );
	this->addChild(coolLabel,2);
	mResultGrade->setSpritePosition(ResultGrade::SET_COOL,coolLabel->getPositionY());

	//GREAT
	CCSprite* greatLabel = CCSprite::create("result/great_label.png");
	greatLabel->setPosition( ccp(30 + greatLabel->getContentSize().width * 0.5f, visibleSize.height - 505 - greatLabel->getContentSize().height * 0.5f) );
	this->addChild(greatLabel,2);
	mResultGrade->setSpritePosition(ResultGrade::SET_GREAT,greatLabel->getPositionY());

	//GOOD
	CCSprite* goodLabel = CCSprite::create("result/good_label.png");
	goodLabel->setPosition( ccp(30 + goodLabel->getContentSize().width * 0.5f, visibleSize.height - 620 - goodLabel->getContentSize().height * 0.5f) );
	this->addChild(goodLabel,2);
	mResultGrade->setSpritePosition(ResultGrade::SET_GOOD,goodLabel->getPositionY());

	//BAD
	CCSprite* badLabel = CCSprite::create("result/bad_label.png");
	badLabel->setPosition( ccp(30 + badLabel->getContentSize().width * 0.5f, visibleSize.height - 735 - badLabel->getContentSize().height * 0.5f) );
	this->addChild(badLabel,2);
	mResultGrade->setSpritePosition(ResultGrade::SET_BAD,badLabel->getPositionY());

	//MISS
	CCSprite* missLabel = CCSprite::create("result/miss_label.png");
	missLabel->setPosition( ccp(30 + missLabel->getContentSize().width * 0.5f, visibleSize.height - 850 - missLabel->getContentSize().height * 0.5f) );
	this->addChild(missLabel,2);
	mResultGrade->setSpritePosition(ResultGrade::SET_MISS,missLabel->getPositionY());

	//SCOREラベル
	CCSprite* scoreLabel = CCSprite::create("result/score_label.png");
	scoreLabel->setPosition( ccp(30 + scoreLabel->getContentSize().width * 0.5f, visibleSize.height - 975 - scoreLabel->getContentSize().height * 0.5f) );
	this->addChild(scoreLabel,2);
	mResultGrade->setSpritePosition(ResultGrade::SET_GRADE,scoreLabel->getPositionY());

	//メニュー
	//AGAIN
	CCSprite* againButton = CCSprite::create("result/again_button.png");
	CCSprite* againButtonSelected = CCSprite::create("result/again_button.png");
	againButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pAgainItem = CCMenuItemSprite::create(
		againButton,
		againButtonSelected,
		this,
		menu_selector(Result::againCallback));
	pAgainItem->setPosition(ccp(30 + pAgainItem->getContentSize().width * 0.5f,30 + pAgainItem->getContentSize().height * 0.5f) );

	//QUIT
	CCSprite* quitButton = CCSprite::create("result/quit_button.png");
	CCSprite* quitButtonSelected = CCSprite::create("result/quit_button.png");
	quitButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pQuitItem = CCMenuItemSprite::create(
		quitButton,
		quitButtonSelected,
		this,
		menu_selector(Result::quitCallback));
	pQuitItem->setPosition(ccp(visibleSize.width - 30 - pQuitItem->getContentSize().width * 0.5f, 30 + pQuitItem->getContentSize().height * 0.5f) );

	mMenu = CCMenu::create(pAgainItem,pQuitItem, NULL);
	mMenu->setPosition(CCPointZero);
	mMenu->setEnabled(false);
	this->addChild(mMenu,2);
	
	//キャラクター
	CCSprite* player = CCSprite::create("result/amane.png");
	player->setPosition( ccp(visibleSize.width - player->getContentSize().width * 0.5f + 100 , visibleSize.height - 330 - player->getContentSize().height * 0.5f) );
	this->addChild(player,1);
	
	this->scheduleUpdate();
	this->setTouchEnabled(true);

	//タッチマネージャー初期化
	int touchNum = 1;
	mTouchManager = new TouchManager(touchNum);

	return true;

}

//更新処理
void Result::update(float delta){

	mTouchManager->update(delta);

	if(mResultGrade->updateGrade(delta)){
		mMenu->setEnabled(true);
	}

}

//レイヤーが表示されたとき
void Result::onEnter(){
    CCLayer::onEnter();     
}

void Result::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
        mTouchManager->setTouchBegan(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void Result::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
		mTouchManager->setTouchMoved(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void Result::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
		mTouchManager->setTouchCancelled(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void Result::ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
        mTouchManager->setTouchEnded(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
	}
}

void Result::againCallback(CCObject* pSender){
	//更新を止める
	this->unscheduleUpdate();
	CCScene* nextScene = Game::scene();
	CCScene* pScene = CCTransitionFlipX::create(1.0f,nextScene);
	if(pScene){
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

void Result::quitCallback(CCObject* pSender){
	//更新を止める
	this->unscheduleUpdate();
	CCScene* nextScene = Title::scene();
	CCScene* pScene = CCTransitionFlipX::create(1.0f,nextScene);
	if(pScene){
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}
