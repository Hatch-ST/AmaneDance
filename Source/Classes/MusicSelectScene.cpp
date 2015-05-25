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

	//�^�b�`�}�l�[�W���[������
	int touchNum = 1;
	mTouchManager = new TouchManager(touchNum);

	//�ȃZ���N�g���x��
	CCSprite* musicSelectLabel = CCSprite::create("music_select_label.png");
	int labelPos_y = visibleSize.height - musicSelectLabel->getContentSize().height/2 - 30;
	musicSelectLabel->setPosition( ccp(260 + musicSelectLabel->getContentSize().width/2,labelPos_y) );
	this->addChild(musicSelectLabel);

	//���j���[
	//�o�b�N�{�^��
	CCSprite* backButton = CCSprite::create("back_button.png");
	CCSprite* backButtonSelected = CCSprite::create("back_button.png");
	backButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pBackItem = CCMenuItemSprite::create(
		backButton,
		backButtonSelected,
		this,
		menu_selector(MusicSelect::backButtonCallback));
	pBackItem->setPosition(ccp( pBackItem->getContentSize().width/2 + 30 , labelPos_y ));

	//�X�^�[�g�{�^��
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

	
	//���{�^��
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

	//���{�^��
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

	//�r���[�p���C���[
	mViewLayer = CCLayer::create();
	mViewLayer->setContentSize( ccp(visibleSize.width * LayerPageNum , 20 + 180 * ListNum_y) );
	mViewLayer->setPosition( ccp(0,visibleSize.height * 0.5f - mViewLayer->getContentSize().height * 0.5f) );
	
	mSelectedMusicIndex = -1;
	mCurrentPageIndex = 0;

	for(int x=0;x<MusicSelect::LayerPageNum;x++){
		for(int y=0;y<MusicSelect::ListNum_y;y++){
			//1�Ȃ��܂Ƃ߂郌�C���[
			CCLayer* musicLayer = CCLayer::create();
			musicLayer->setContentSize( ccp(visibleSize.width, 150) );
			int pos_y = mViewLayer->getContentSize().height - 30 - 180 * y - musicLayer->getContentSize().height;
			musicLayer->setPosition( ccp( visibleSize.width * x, pos_y) );

			//�t���[��
			CCSprite* musicFrame = CCSprite::create("music_frame.png");
			musicFrame->setPosition( ccp(musicLayer->getContentSize().width * 0.5f , musicLayer->getContentSize().height * 0.5f) );
			musicLayer->addChild(musicFrame,1);

			//�A�C�R��
			CCSprite* musicIcon = CCSprite::create("music_icon.png");
			musicIcon->setPosition( ccp( 45 + musicIcon->getContentSize().width * 0.5f , musicLayer->getContentSize().height * 0.5f ) );
			musicLayer->addChild(musicIcon,2);

			//�Ȗ�
			CCString* str = getMusicName(y + x * MusicSelect::ListNum_y);
			CCLabelTTF* musicName = CCLabelTTF::create(str->getCString(), "", 28.0f);
			musicName->setPosition( ccp( 180 + musicName->getContentSize().width * 0.5f , musicLayer->getContentSize().height * 0.5f + musicName->getContentSize().height * 0.5f) );
			musicName->setTag(1);
			musicLayer->addChild(musicName,2);

			//�x�X�g�X�R�A���x��
			CCSprite* bestGradeLabel = CCSprite::create("best_score.png");
			bestGradeLabel->setPosition( ccp( 180 + bestGradeLabel->getContentSize().width * 0.5f , musicLayer->getContentSize().height * 0.5f - bestGradeLabel->getContentSize().height * 0.5f) );
			musicLayer->addChild(bestGradeLabel,2);

			//�x�X�g�X�R�A
			BestGrade* bestGradeSprite = new BestGrade(123456);
			bestGradeSprite->setSpritePosition( musicLayer->getContentSize().width - 30 , bestGradeLabel->getPositionY() );
			bestGradeSprite->setSprite(musicLayer);


			//�y�[�W�O�̉摜�������Ȃ����Ă���
			if(mCurrentPageIndex != x){
				musicLayer->setVisible(false);
			}
			mViewLayer->addChild(musicLayer, 1, y + x * MusicSelect::ListNum_y);
		}
	}
	this->addChild(mViewLayer);

	//�ȑI���t���[��
	mSelectedFrame = CCSprite::create("selected_frame.png");
	mSelectedFrame->setVisible(false);
	this->addChild( mSelectedFrame );
	

	mPageMoving = Stop;
	mSwiped = false;
	
	return true;

}

//�X�V����
void MusicSelect::update(float delta){
	//�X���C�v�𒲂ׂ�
	checkSwipeMusic();
	//�^�b�v�𒲂ׂ�
	checkTapMusic();
	//�^�b�`�̍X�V
	mTouchManager->update(delta);
}



//���C���[���\�����ꂽ�Ƃ�
void MusicSelect::onEnter(){
    CCLayer::onEnter();
	//�n�[�h�L�[�L��
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

//�o�b�N�{�^���������ꂽ�Ƃ�
void MusicSelect::backButtonCallback(CCObject* pSender){
	backToTitle();
}

void MusicSelect::backToTitle(){
	//�^�C�g����
	this->unscheduleUpdate();
	CCScene* nextScene = Title::scene();
	CCScene* pScene = CCTransitionFlipX::create(1.0f,nextScene);
	if(pScene){
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

//�X�^�[�g�{�^���������ꂽ�Ƃ�
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
	//�Q�[���J�n
	this->unscheduleUpdate();
	CCScene* nextScene = Game::scene();
	CCScene* pScene = CCTransitionFlipX::create(1.0f,nextScene);
	if(pScene){
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

//���{�^���������ꂽ�Ƃ�
void MusicSelect::leftButtonCallback(CCObject* pSender){
	movePage(true);
}

//���{�^���������ꂽ�Ƃ�
void MusicSelect::rightButtonCallback(CCObject* pSender){
	movePage(false);
}

//�ȃ��X�g�̃y�[�W���ړ�����
void MusicSelect::movePage(bool isLeft){
	//�A�N�V��������
	if(mPageMoving != Stop){
		return;
	}
	if(isLeft){
		//�y�[�W�[�`�F�b�N
		if(mCurrentPageIndex != 0){
			//���̃y�[�W������
			for(int i=0;i<MusicSelect::ListNum_y;i++){
				mViewLayer->getChildByTag(i + (mCurrentPageIndex-1) * MusicSelect::ListNum_y )->setVisible(true);
			}
			//�{�^���̗L���`�F�b�N
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
		//�y�[�W�[�`�F�b�N
		if(mCurrentPageIndex != MusicSelect::LayerPageNum-1){
			//�E�̃y�[�W������
			for(int i=0;i<ListNum_y;i++){
				mViewLayer->getChildByTag(i + (mCurrentPageIndex+1) * ListNum_y )->setVisible(true);
			}
			//�{�^���̗L���`�F�b�N
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
	//�y�[�W�O�̉摜�������Ȃ�����
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

	//�^�b�`�𗣂����u�Ԃ�
	if(mTouchManager->getTouchObject(0)->touchEndedFlag){
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCSize frameSize = mViewLayer->getChildByTag( 0 )->getContentSize();
		int firstPos_y = visibleSize.height - (210 + frameSize.height * 0.5f);
		//�S����x���W�`�F�b�N
		if(30 <= mTouchManager->getTouchObject(0)->touch_ended_x &&
			mTouchManager->getTouchObject(0)->touch_ended_x <= frameSize.width - 30){
			//y���W�`�F�b�N
			for(int i=0;i<MusicSelect::ListNum_y;i++){
				//�I�����̍��W
				if(firstPos_y - frameSize.height * 0.5f - i * (frameSize.height+30) <= mTouchManager->getTouchObject(0)->touch_ended_y &&
				mTouchManager->getTouchObject(0)->touch_ended_x <= firstPos_y + frameSize.height * 0.5f - i * frameSize.height){
					//�J�n���̍��W
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
	//�^�b�`�𗣂����u�Ԃ�
	if(mTouchManager->getTouchObject(0)->touchEndedFlag){
		CCPoint viewPos = mViewLayer->getPosition();
		CCSize viewSize = mViewLayer->getContentSize();
		//�J�n�����r���[�̒����`�F�b�N(y�̂�)
		if(viewPos.y <= mTouchManager->getTouchObject(0)->touch_began_y &&
			mTouchManager->getTouchObject(0)->touch_began_y <= viewPos.y + viewSize.height){
			//�����̌v�Z
			int dir = 100;
			int dx = ( mTouchManager->getTouchObject(0)->touch_began_x - mTouchManager->getTouchObject(0)->touch_ended_x);
			int dy = ( mTouchManager->getTouchObject(0)->touch_began_y - mTouchManager->getTouchObject(0)->touch_ended_y);
			//�������ɃX���C�v������
			if( abs(dx) > abs(dy) ){
				//���������ȏ㒴������
				if( dir < abs(dx) ){
					//�X�s�[�h�̌v�Z
					if( mTouchManager->getTouchObject(0)->touch_time != 0){
						int speed = abs(dx) / mTouchManager->getTouchObject(0)->touch_time;
						//�X�s�[�h�����ȏ㒴������
						if( 80 < speed){
							//�ǂ���ɃX���C�v������
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

//�Ȗ���o�^����
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