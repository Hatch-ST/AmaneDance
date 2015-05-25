#include "GameScene.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

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

CCScene* Game::scene()
{
	CCScene *scene = SceneCreater::create();
	SceneCreater* sceneCreater = (SceneCreater*)scene;
	sceneCreater->setSceneName(SceneCreater::GAME);
	Game *layer = Game::create();
	scene->addChild(layer,0,0);
	return scene;
}


bool Game::init()
{
	srand((unsigned int)time(NULL));
	if ( !CCLayer::init() )
	{
		return false;
	}
	//オプションデータを読み込む
	CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();

	//曲の音量をセット
	MusicData::setBGMVolume( userDefault->getFloatForKey("bgmVolume",0.8f) );

	//曲の長さ取得
	mMusicTime = MusicData::getMusicTime(gMusicName);

	//SEを読み込む
	SEData::preloadSE(SE_TAP_NOTE);

	//SEの音量をセット
	SEData::setSEVolume( userDefault->getFloatForKey("seVolume",0.8f) );

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//ステージ
	CCSprite* bgSprite = CCSprite::create("stage/stage.png");
	bgSprite->setPosition( ccp(visibleSize.width * 0.5f,visibleSize.height * 0.5f) );
	this->addChild(bgSprite, BackgroundPriority);

	this->scheduleUpdate();
	this->setTouchEnabled(true);
	
	//時間初期化
	mGameSecond = 0.0f;
	mGameFrame = 0;

	//プレイヤー初期化
	mPlayer = new PlayerCharacter(gMusicName);
	mPlayer->setSSPlayer(this);
	mPlayer->pause();



	//タッチマネージャー初期化
	int touchNum = 2;
	mTouchManager = new TouchManager(touchNum,this);

	mScoreManager = new ScoreManager(gMusicName,this);

	mGameStarted = false;
	mGameEnded = false;
	mPaused = false;


	//暗転用背景
	mBlackBack = CCSprite::create("interface/black.png");
	mBlackBack->setPosition( ccp(visibleSize.width * 0.5f,visibleSize.height * 0.5f) );
	mBlackBack->setOpacity(128);
	this->addChild(mBlackBack,BlackPriority);
	//ゲーム前インフォ
	mTouchToStart = CCSprite::create("interface/info.png");
	mTouchToStart->setPosition( ccp(visibleSize.width * 0.5f,visibleSize.height * 0.5f) );
	this->addChild(mTouchToStart,InfoPriority);

	//メニューバー
	CCSprite* pauseButton = CCSprite::create("interface/pause_button.png");
	pauseButton->setOpacity(170);
	CCSprite* pauseButtonSelected = CCSprite::create("interface/pause_button.png");
	pauseButtonSelected->setColor(ccc3(100,100,100));
	pauseButtonSelected->setOpacity(200);
	CCMenuItemSprite *pCloseItem = CCMenuItemSprite::create(
		pauseButton,
		pauseButtonSelected,
		this,
		menu_selector(Game::pauseMenuCallback));

	pCloseItem->setPosition(ccp(origin.x + pCloseItem->getContentSize().width/2  ,	origin.y + pCloseItem->getContentSize().height/2));
	mMenu = CCMenu::create(pCloseItem, NULL);
	mMenu->setPosition(CCPointZero);
	mMenu->setEnabled(false);
	this->addChild(mMenu, ButtonPriority);

	//プログレス
	mMusicProgress = new MusicProgress(gMusicName,this); 

	//得点バー
	CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage("interface/frame01.png");
	int width = 720;
	int height = 100;
	CCRect rect = CCRectMake(0, 0 , width, height);
	mGradeBar = CCSprite::createWithTexture (pTexture,rect);
	mGradeBar->setPosition(ccp(origin.x + mGradeBar->getContentSize().width/2  ,	origin.y + visibleSize.height - mGradeBar->getContentSize().height/2));
	this->addChild(mGradeBar,InfoPriority);

	//得点管理
	mGradeManager = new GradeManager();
	mGradeManager->setSprite(this);

	//加速度表示
	//CCString* str = CCString::createWithFormat(
	//	" X:%1.3f, Y:%1.3f, Z:%1.3f",0.0f,0.0f,0.0f);
	//CCLabelTTF* accelLabel = CCLabelTTF::create(str->getCString(), "", 48.0f);
	//accelLabel->setPosition(ccp(visibleSize.width * 0.5f, visibleSize.height - 120));
	//accelLabel->setTag(1);
	//this->addChild(accelLabel);

	mAccel_x = 0.0f;
	mAccel_y = 0.0f;
	mAccel_z = 0.0f;

	mShaked = false;
	mShakeStartTime = 0.0f;
	mOneShakeTime = 30.0f / MusicData::getMusicBPM(gMusicName);
	mShakeLevel = userDefault->getFloatForKey("shakeLevel",3.0f);
	
	//スコアの初期化
	gGradeData.totalGrade = 0;
	gGradeData.coolNum = 0;
	gGradeData.greatNum = 0;
	gGradeData.goodNum = 0;
	gGradeData.badNum = 0;
	gGradeData.missNum = 0;

	return true;

}

int Game::screenWidth(){
	return CCDirector::sharedDirector()->getWinSize().width;
}

int Game::screenHeight(){
	return CCDirector::sharedDirector()->getWinSize().height;
}

//更新処理
void Game::update(float delta){
	//ゲーム開始前
	if(!mGameStarted){
		//タッチ開始時
		if(mTouchManager->getTouchObject(0)->touchBeganFlag){
			mTouchToStart->setVisible(false);
			mBlackBack->setVisible(false);
			mGameStarted = true;
			//曲再生開始
			MusicData::playBGM(gMusicName);
			//プログレス開始
			mMusicProgress->startProgressAnim();
			//ポーズボタン使用可
			mMenu->setEnabled(true);
			//プレイヤーアニメーション開始
			mPlayer->resume();
		}
		mTouchManager->update(delta);
		return;
	}
	
	//ゲーム開始後の処理
	mPlayer->update(mGameFrame,delta);
	//タッチ判定
	for(int i=0;i<mTouchManager->touchData->count();i++){
		//タップ時
		if(mTouchManager->getTouchObject(i)->touchBeganFlag){
			if(mScoreManager->checkTap(mTouchManager->getTouchObject(i)->touch_began_x,mTouchManager->getTouchObject(i)->touch_began_y,mGradeManager)){
				if(!mPlayer->mIsMissed){
					//バッドを出したとき
					mPlayer->mIsMissed = true;
					mPlayer->mMissMotion = MotionData::DAMEGE_02;
				}
			}			
		}
		//リリース時
		if(mTouchManager->getTouchObject(i)->touchEndedFlag){
			if(mScoreManager->checkRelease(mTouchManager->getTouchObject(i)->touch_ended_x,mTouchManager->getTouchObject(i)->touch_ended_y,mGradeManager)){
				if(!mPlayer->mIsMissed){
					//バッドを出したとき
					mPlayer->mIsMissed = true;
					mPlayer->mMissMotion = MotionData::DAMEGE_02;
				}
			}			
		}
		//ホールド時
		if(mTouchManager->getTouchObject(i)->touchFlag){
			mScoreManager->checkHold(mTouchManager->getTouchObject(i)->touch_x,mTouchManager->getTouchObject(i)->touch_y,mGradeManager);			
		}
	}
	
	if(mScoreManager->update(mGameFrame,delta)){
		if(!mPlayer->mIsMissed){
			//ミスを出したとき
			mPlayer->mIsMissed = true;
			mPlayer->mMissMotion = MotionData::DAMEGE_01;
		}
	}

	if(!mShaked){
		//シェイクを調べる
		if(checkShake()){
			mShaked = true;
			//効果音の鳴らす
			SEData::playSE(SE_TAP_NOTE);
			//シェイク開始時間を保存
			mShakeStartTime = mGameSecond;
			//シェイクの判定
			if(mScoreManager->checkShake(mGradeManager)){
				if(!mPlayer->mIsMissed){
					//バッドを出したとき
					mPlayer->mIsMissed = true;
					mPlayer->mMissMotion = MotionData::DAMEGE_02;
				}
			}
		}
	}else{
		//シェイク1回分の時間が経過したか
		if(mGameSecond - mShakeStartTime > mOneShakeTime){
			//元に戻す
			mShaked = false;
		}
	}


	mTouchManager->update(delta);
	mGradeManager->update();
	mGameSecond += delta;
	mGameFrame = (int) (mGameSecond * 60);
	
	if(mMusicTime < mGameSecond){
		mGameEnded = true;
	}

	//画面切り替え
	if(mMusicTime + 3.0f < mGameSecond){
		//曲を止める
		MusicData::stopBGM();
		//更新を止める
		this->unscheduleUpdate();
		//スコアを取得
		mGradeManager->getTotalGrade();
		//終了したら画面移動
		CCScene* nextScene = Result::scene();
		CCScene* pScene = CCTransitionTurnOffTiles::create(1.0f,nextScene);
		if(pScene){
			CCDirector::sharedDirector()->replaceScene(pScene);
		}
	}
}

//シーンが始まった時に呼び出されるメソッド
void Game::onEnter(){
    CCLayer::onEnter();
	//加速度センサー有効
	setAccelerometerEnabled(true);
	setAccelerometerInterval(1.0f);
		//ハードキー有効
	this->setKeypadEnabled(true);
}
//シーンが終わった時に呼び出されるメソッド
void Game::onExit(){
	CCLayer::onExit(); 
}
//加速度を取得する
void Game::didAccelerate(cocos2d::CCAcceleration* pAccel){
	//加速度の取得
	mAccel_x = pAccel->x;
	mAccel_y = pAccel->y;
	mAccel_z = pAccel->z;
	
	//加速度表示
	//CCString* str = CCString::createWithFormat(
	//	" X:%1.3f, Y:%1.3f, Z:%1.3f",pAccel->x,pAccel->y,pAccel->z);
	//CCLabelTTF* label = (CCLabelTTF*)this->getChildByTag(1);
	//label->setString(str->getCString());

	//this->getCamera()->setEyeXYZ(
	//	pAccel->x * 0.00000063429789,
	//	pAccel->y * 0.00000095144683,
	//	0.0000001);
}

//シェイクされたか調べる
bool Game::checkShake(){
	float total = fabs(mAccel_x) + fabs(mAccel_y) + fabs(mAccel_z);
	if(total > mShakeLevel){
		return true;
	}
	return false;
}

//シングルタッチ
//bool Game::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//	//タッチした座標を取得
//    CCPoint location =pTouch->getLocation();
//	mPlayer->setPlayerPos(location.x-100,location.y+375);
//
//    return true;
//     
//}
//void Game::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//	int width = CCDirector::sharedDirector()->getWinSize().width;
//	int height = CCDirector::sharedDirector()->getWinSize().height;
//	//初期位置に戻す
//	mPlayer->setPlayerPos(screenWidth() / 2 - 100 , screenHeight() / 2 + 375);
//}
//void Game::ccTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//
//	//タッチした座標を取得
//	CCPoint location =pTouch->getLocation();
//	mPlayer->setPlayerPos(location.x-100,location.y+375);
//}

void Game::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	if(!mPaused){
		//ゲーム中のみ
		if(mGameStarted && !mGameEnded){
			//効果音の鳴らす
			SEData::playSE(SE_TAP_NOTE);
		}
	}
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
        mTouchManager->setTouchBegan(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void Game::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
		mTouchManager->setTouchMoved(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void Game::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
		mTouchManager->setTouchCancelled(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void Game::ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
        mTouchManager->setTouchEnded(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
	}
}


//ポーズが押されたとき
void Game::pauseMenuCallback(CCObject* pSender){
	pauseOpenedCallback();
}

//ポーズが開始されたとき
void Game::pauseOpenedCallback(){
	if(!mPaused){
		//ゲーム中のみ
		if(mGameStarted && !mGameEnded){
			mPaused = true;
			//更新を止める
			this->unscheduleUpdate();
			//曲を一時停止させる
			MusicData::pauseBGM();
			//プログレスを停止させる
			mMusicProgress->pauseProgressAnim(mGameSecond);
			//ポーズボタンを消す
			mMenu->setVisible(false);
			//ポーズレイヤー呼び出し
			PauseLayer* pauseLayer = PauseLayer::create();
			this->addChild(pauseLayer,PausePriority,mPauseTagNum);
			pauseLayer->setParentLayer(this);
			//ズームアクション
			pauseLayer->setScale(0.5f);
			CCActionInterval* action = CCScaleTo::create(0.1f ,1.0f);
			action->setTag(0);
			pauseLayer->runAction(action);
			//ノートのアニメーション一時停止
			mScoreManager->pauseAnimation();
			//キャラアニメーション一時停止
			mPlayer->pause();
		}
	}
}

//ポーズが解除されたとき
void Game::pauseClosedCallback(){
	if(mPaused){
		mPaused = false;
		//更新を再開する
		this->scheduleUpdate();
		//曲を再開させる
		MusicData::resumeBGM();
		//プログレスを再開させる
		mMusicProgress->startProgressAnim();
		//ポーズボタンを出す
		mMenu->setVisible(true);
		//ノートのアニメーション再開
		mScoreManager->resumeAnimation();
		//キャラアニメーション再開
		mPlayer->resume();
	}
}

void Game::keyBackClicked(){
	//ポーズ前
	if(!mPaused){
		//ポーズする
		pauseOpenedCallback();
	}else{
		//ポーズ解除
		pauseClosedCallback();
		this->removeChildByTag(mPauseTagNum);
	}
}

void Game::keyMenuClicked(){
	//ポーズ前
	if(!mPaused){
		//ポーズする
		pauseOpenedCallback();
	}else{
		//ポーズ解除
		pauseClosedCallback();
		this->removeChildByTag(mPauseTagNum);
	}
}