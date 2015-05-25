#include "MusicSelectScene.h"
#include <cmath>

#ifdef G_MUSIC_NAME
MusicName gMusicName;
#else
extern MusicName gMusicName;
#endif

USING_NS_CC;
//#pragma execution_character_set("utf-8")

CCScene* MusicSelect::scene()
{
	CCScene *scene = SceneCreater::create();
	SceneCreater* sceneCreater = (SceneCreater*)scene;
	sceneCreater->setSceneName(SceneCreater::MUSIC_SELECT);
	MusicSelect *layer = MusicSelect::create();
	scene->addChild(layer);
	return scene;
}


bool MusicSelect::init()
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

	this->scheduleUpdate();
	this->setTouchEnabled(true);

	//タッチマネージャー初期化
	int touchNum = 1;
	mTouchManager = new TouchManager(touchNum);

	//曲セレクトラベル
	CCSprite* musicSelectLabel = CCSprite::create("music_select_label.png");
	int labelPos_y = visibleSize.height - musicSelectLabel->getContentSize().height/2 - 30;
	musicSelectLabel->setPosition( ccp(260 + musicSelectLabel->getContentSize().width/2,labelPos_y) );
	this->addChild(musicSelectLabel);

	//メニュー
	//バックボタン
	CCSprite* backButton = CCSprite::create("back_button.png");
	CCSprite* backButtonSelected = CCSprite::create("back_button.png");
	backButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pBackItem = CCMenuItemSprite::create(
		backButton,
		backButtonSelected,
		this,
		menu_selector(MusicSelect::backButtonCallback));
	pBackItem->setPosition(ccp( pBackItem->getContentSize().width/2 + 30 , labelPos_y ));

	//スタートボタン
	CCSprite* startButton = CCSprite::create("select_start_button.png");
	CCSprite* startButtonSelected = CCSprite::create("select_start_button.png");
	int startPos_y = 30 + startButton->getContentSize().height*0.5f;
	startButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pStartItem = CCMenuItemSprite::create(
		startButton,
		startButtonSelected,
		this,
		menu_selector(MusicSelect::startButtonCallback));
	pStartItem->setPosition(ccp( visibleSize.width * 0.5f , startPos_y ));

	
	//←ボタン
	CCSprite* leftButton = CCSprite::create("allow_left.png");
	CCSprite* leftButtonSelected = CCSprite::create("allow_left.png");
	leftButtonSelected->setColor(ccc3(100,100,100));
	mLeftItem = CCMenuItemSprite::create(
		leftButton,
		leftButtonSelected,
		this,
		menu_selector(MusicSelect::leftButtonCallback));
	mLeftItem->setPosition(ccp( 30 + mLeftItem->getContentSize().width * 0.5f , startPos_y ));
	mLeftItem->setEnabled(false);

	//→ボタン
	CCSprite* rightButton = CCSprite::create("allow_left.png");
	rightButton->setFlipX(true);
	CCSprite* rightButtonSelected = CCSprite::create("allow_left.png");
	rightButtonSelected->setFlipX(true);
	rightButtonSelected->setColor(ccc3(100,100,100));
	mRightItem = CCMenuItemSprite::create(
		rightButton,
		rightButtonSelected,
		this,
		menu_selector(MusicSelect::rightButtonCallback));
	mRightItem->setPosition(ccp( visibleSize.width - 30 - mRightItem->getContentSize().width * 0.5f , startPos_y ));

	CCMenu* pMenu = CCMenu::create(pBackItem,pStartItem,mLeftItem,mRightItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu,1);

	//ビュー用レイヤー
	mViewLayer = CCLayer::create();
	mViewLayer->setContentSize( ccp(visibleSize.width * LayerPageNum , 20 + 180 * ListNum_y) );
	mViewLayer->setPosition( ccp(0,visibleSize.height * 0.5f - mViewLayer->getContentSize().height * 0.5f) );
	
	mSelectedMusicIndex = -1;
	mCurrentPageIndex = 0;

	for(int x=0;x<MusicSelect::LayerPageNum;x++){
		for(int y=0;y<MusicSelect::ListNum_y;y++){
			//1曲をまとめるレイヤー
			CCLayer* musicLayer = CCLayer::create();
			musicLayer->setContentSize( ccp(visibleSize.width, 150) );
			int pos_y = mViewLayer->getContentSize().height - 30 - 180 * y - musicLayer->getContentSize().height;
			musicLayer->setPosition( ccp( visibleSize.width * x, pos_y) );

			//フレーム
			CCSprite* musicFrame = CCSprite::create("music_frame.png");
			musicFrame->setPosition( ccp(musicLayer->getContentSize().width * 0.5f , musicLayer->getContentSize().height * 0.5f) );
			musicLayer->addChild(musicFrame,1);

			//アイコン
			CCSprite* musicIcon = CCSprite::create("music_icon.png");
			musicIcon->setPosition( ccp( 45 + musicIcon->getContentSize().width * 0.5f , musicLayer->getContentSize().height * 0.5f ) );
			musicLayer->addChild(musicIcon,2);

			//曲名
			CCString* str = getMusicName(y + x * MusicSelect::ListNum_y);
			CCLabelTTF* musicName = CCLabelTTF::create(str->getCString(), "", 28.0f);
			musicName->setPosition( ccp( 180 + musicName->getContentSize().width * 0.5f , musicLayer->getContentSize().height * 0.5f + musicName->getContentSize().height * 0.5f) );
			musicName->setTag(1);
			musicLayer->addChild(musicName,2);

			//ベストスコアラベル
			CCSprite* bestGradeLabel = CCSprite::create("best_score.png");
			bestGradeLabel->setPosition( ccp( 180 + bestGradeLabel->getContentSize().width * 0.5f , musicLayer->getContentSize().height * 0.5f - bestGradeLabel->getContentSize().height * 0.5f) );
			musicLayer->addChild(bestGradeLabel,2);

			//ベストスコア
			BestGrade* bestGradeSprite = new BestGrade(123456);
			bestGradeSprite->setSpritePosition( musicLayer->getContentSize().width - 30 , bestGradeLabel->getPositionY() );
			bestGradeSprite->setSprite(musicLayer);


			//ページ外の画像を見えなくしておく
			if(mCurrentPageIndex != x){
				musicLayer->setVisible(false);
			}
			mViewLayer->addChild(musicLayer, 1, y + x * MusicSelect::ListNum_y);
		}
	}
	this->addChild(mViewLayer);

	//曲選択フレーム
	mSelectedFrame = CCSprite::create("selected_frame.png");
	mSelectedFrame->setVisible(false);
	this->addChild( mSelectedFrame );
	

	mPageMoving = Stop;
	mSwiped = false;
	
	return true;

}

//更新処理
void MusicSelect::update(float delta){
	//スワイプを調べる
	checkSwipeMusic();
	//タップを調べる
	checkTapMusic();
	//タッチの更新
	mTouchManager->update(delta);
}



//レイヤーが表示されたとき
void MusicSelect::onEnter(){
    CCLayer::onEnter();
	//ハードキー有効
	this->setKeypadEnabled(true);
}

void MusicSelect::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
        mTouchManager->setTouchBegan(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void MusicSelect::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
		mTouchManager->setTouchMoved(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void MusicSelect::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
		mTouchManager->setTouchCancelled(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void MusicSelect::ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
        mTouchManager->setTouchEnded(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
	}
}

void MusicSelect::keyBackClicked(){
	backToTitle();
}

void MusicSelect::keyMenuClicked(){
}

//バックボタンが押されたとき
void MusicSelect::backButtonCallback(CCObject* pSender){
	backToTitle();
}

void MusicSelect::backToTitle(){
	//タイトルへ
	this->unscheduleUpdate();
	CCScene* nextScene = Title::scene();
	CCScene* pScene = CCTransitionFlipX::create(1.0f,nextScene);
	if(pScene){
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

//スタートボタンが押されたとき
void MusicSelect::startButtonCallback(CCObject* pSender){
	if(mSelectedMusicIndex == -1){
		return;
	}
	switch(mSelectedMusicIndex){
		case 0:
		default:
			gMusicName = ORETA_TSUBASA;
			break;
		case 1:
			gMusicName = ORETA_TSUBASA_OKE;
			break;
		case 2:
			gMusicName = PLASTIC_SKY;
			break;
		case 3:
			gMusicName = PLASTIC_SKY_OKE;
			break;
	}
	//ゲーム開始
	this->unscheduleUpdate();
	CCScene* nextScene = Game::scene();
	CCScene* pScene = CCTransitionFlipX::create(1.0f,nextScene);
	if(pScene){
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

//←ボタンが押されたとき
void MusicSelect::leftButtonCallback(CCObject* pSender){
	movePage(true);
}

//→ボタンが押されたとき
void MusicSelect::rightButtonCallback(CCObject* pSender){
	movePage(false);
}

//曲リストのページを移動する
void MusicSelect::movePage(bool isLeft){
	//アクション中か
	if(mPageMoving != Stop){
		return;
	}
	if(isLeft){
		//ページ端チェック
		if(mCurrentPageIndex != 0){
			//左のページを可視化
			for(int i=0;i<MusicSelect::ListNum_y;i++){
				mViewLayer->getChildByTag(i + (mCurrentPageIndex-1) * MusicSelect::ListNum_y )->setVisible(true);
			}
			//ボタンの有効チェック
			if(--mCurrentPageIndex == 0){
				mLeftItem->setEnabled(false);
			}
			mRightItem->setEnabled(true);

			CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
			CCActionInterval* action = CCMoveTo::create(0.5f ,ccp( (-mCurrentPageIndex) * visibleSize.width ,visibleSize.height * 0.5f - mViewLayer->getContentSize().height * 0.5f));
			CCCallFuncN* actionDone = CCCallFuncN::create( this, callfuncN_selector(MusicSelect::pageMovedCallback) );
			mViewLayer->runAction( CCSequence::create( action ,actionDone, NULL) );	
			mPageMoving = Left;

			mSelectedFrame->setVisible(false);
			mSelectedMusicIndex = -1;
		}
	}else{
		//ページ端チェック
		if(mCurrentPageIndex != MusicSelect::LayerPageNum-1){
			//右のページを可視化
			for(int i=0;i<ListNum_y;i++){
				mViewLayer->getChildByTag(i + (mCurrentPageIndex+1) * ListNum_y )->setVisible(true);
			}
			//ボタンの有効チェック
			if(++mCurrentPageIndex == MusicSelect::LayerPageNum-1){
				mRightItem->setEnabled(false);
			}
			mLeftItem->setEnabled(true);

			CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
			CCActionInterval* action = CCMoveTo::create(0.5f ,ccp( (-mCurrentPageIndex) * visibleSize.width ,visibleSize.height * 0.5f - mViewLayer->getContentSize().height * 0.5f));
			CCCallFuncN* actionDone = CCCallFuncN::create( this, callfuncN_selector(MusicSelect::pageMovedCallback) );
			mViewLayer->runAction( CCSequence::create( action ,actionDone, NULL) );	
			mPageMoving = Right;

			mSelectedFrame->setVisible(false);
			mSelectedMusicIndex = -1;
		}
	}

}

void MusicSelect::pageMovedCallback(CCNode* sender){
	//ページ外の画像を見えなくする
	int i;
	switch(mPageMoving){
		case Left:
			for(i=0;i<MusicSelect::ListNum_y;i++){
				mViewLayer->getChildByTag(i + (mCurrentPageIndex+1) * ListNum_y )->setVisible(false);
			}
			break;
		case Right:
			for(i=0;i<MusicSelect::ListNum_y;i++){
				mViewLayer->getChildByTag(i + (mCurrentPageIndex-1) * ListNum_y )->setVisible(false);
			}
			break;
	}
	mPageMoving = Stop;
}

void MusicSelect::checkTapMusic(){
	if(mSwiped){
		return;
	}

	//タッチを離した瞬間か
	if(mTouchManager->getTouchObject(0)->touchEndedFlag){
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCSize frameSize = mViewLayer->getChildByTag( 0 )->getContentSize();
		int firstPos_y = visibleSize.height - (210 + frameSize.height * 0.5f);
		//全共通x座標チェック
		if(30 <= mTouchManager->getTouchObject(0)->touch_ended_x &&
			mTouchManager->getTouchObject(0)->touch_ended_x <= frameSize.width - 30){
			//y座標チェック
			for(int i=0;i<MusicSelect::ListNum_y;i++){
				//終了時の座標
				if(firstPos_y - frameSize.height * 0.5f - i * (frameSize.height+30) <= mTouchManager->getTouchObject(0)->touch_ended_y &&
				mTouchManager->getTouchObject(0)->touch_ended_x <= firstPos_y + frameSize.height * 0.5f - i * frameSize.height){
					//開始時の座標
					if(firstPos_y - frameSize.height * 0.5f - i * (frameSize.height+30) <= mTouchManager->getTouchObject(0)->touch_began_y &&
					mTouchManager->getTouchObject(0)->touch_began_y <= firstPos_y + frameSize.height * 0.5f - i * frameSize.height){
						mSelectedMusicIndex = i + mCurrentPageIndex * ListNum_y;
						mSelectedFrame->setPosition(ccp(visibleSize.width * 0.5f,firstPos_y - i * (frameSize.height+30)) );
						mSelectedFrame->setVisible(true);
						break;
					}
				}
			}
		}
	}
}

void MusicSelect::checkSwipeMusic(){
	//タッチを離した瞬間か
	if(mTouchManager->getTouchObject(0)->touchEndedFlag){
		CCPoint viewPos = mViewLayer->getPosition();
		CCSize viewSize = mViewLayer->getContentSize();
		//開始時がビューの中かチェック(yのみ)
		if(viewPos.y <= mTouchManager->getTouchObject(0)->touch_began_y &&
			mTouchManager->getTouchObject(0)->touch_began_y <= viewPos.y + viewSize.height){
			//距離の計算
			int dir = 100;
			int dx = ( mTouchManager->getTouchObject(0)->touch_began_x - mTouchManager->getTouchObject(0)->touch_ended_x);
			int dy = ( mTouchManager->getTouchObject(0)->touch_began_y - mTouchManager->getTouchObject(0)->touch_ended_y);
			//横方向にスワイプしたか
			if( abs(dx) > abs(dy) ){
				//距離が一定以上超えたか
				if( dir < abs(dx) ){
					//スピードの計算
					if( mTouchManager->getTouchObject(0)->touch_time != 0){
						int speed = abs(dx) / mTouchManager->getTouchObject(0)->touch_time;
						//スピードが一定以上超えたか
						if( 80 < speed){
							//どちらにスワイプしたか
							if( dx < 0){
								movePage(true);
							}else{
								movePage(false);
							}
							mSwiped = true;
							return;
						}
					}
				}
			}
		}
	}
	mSwiped = false;
}

//曲名を登録する
CCString* MusicSelect::getMusicName(int index){
	CCString* str;
	switch(index){
		case ORETA_TSUBASA:
			str = CCString::createWithFormat( "Oreta Tsubasa / Rengoku Teien" );
			break;
		case ORETA_TSUBASA_OKE:
			str = CCString::createWithFormat( "Oreta Tsubasa(Oke ver) / Rengoku Teien" );
			break;
		case PLASTIC_SKY:
			str = CCString::createWithFormat( "Plastic Sky / Rengoku Teien" );
			break;
		case PLASTIC_SKY_OKE:
			str = CCString::createWithFormat( "Plastic Sky(Oke ver) / Rengoku Teien" );
			break;
		default:
			str = CCString::createWithFormat( "" );
	}
	return str;
}