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
	//�I�v�V�����f�[�^��ǂݍ���
	CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();

	//�Ȃ̉��ʂ��Z�b�g
	MusicData::setBGMVolume( userDefault->getFloatForKey("bgmVolume",0.8f) );

	//�Ȃ̒����擾
	mMusicTime = MusicData::getMusicTime(gMusicName);

	//SE��ǂݍ���
	SEData::preloadSE(SE_TAP_NOTE);

	//SE�̉��ʂ��Z�b�g
	SEData::setSEVolume( userDefault->getFloatForKey("seVolume",0.8f) );

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//�X�e�[�W
	CCSprite* bgSprite = CCSprite::create("stage/stage.png");
	bgSprite->setPosition( ccp(visibleSize.width * 0.5f,visibleSize.height * 0.5f) );
	this->addChild(bgSprite, BackgroundPriority);

	this->scheduleUpdate();
	this->setTouchEnabled(true);
	
	//���ԏ�����
	mGameSecond = 0.0f;
	mGameFrame = 0;

	//�v���C���[������
	mPlayer = new PlayerCharacter(gMusicName);
	mPlayer->setSSPlayer(this);
	mPlayer->pause();



	//�^�b�`�}�l�[�W���[������
	int touchNum = 2;
	mTouchManager = new TouchManager(touchNum,this);

	mScoreManager = new ScoreManager(gMusicName,this);

	mGameStarted = false;
	mGameEnded = false;
	mPaused = false;


	//�Ó]�p�w�i
	mBlackBack = CCSprite::create("interface/black.png");
	mBlackBack->setPosition( ccp(visibleSize.width * 0.5f,visibleSize.height * 0.5f) );
	mBlackBack->setOpacity(128);
	this->addChild(mBlackBack,BlackPriority);
	//�Q�[���O�C���t�H
	mTouchToStart = CCSprite::create("interface/info.png");
	mTouchToStart->setPosition( ccp(visibleSize.width * 0.5f,visibleSize.height * 0.5f) );
	this->addChild(mTouchToStart,InfoPriority);

	//���j���[�o�[
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

	//�v���O���X
	mMusicProgress = new MusicProgress(gMusicName,this); 

	//���_�o�[
	CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage("interface/frame01.png");
	int width = 720;
	int height = 100;
	CCRect rect = CCRectMake(0, 0 , width, height);
	mGradeBar = CCSprite::createWithTexture (pTexture,rect);
	mGradeBar->setPosition(ccp(origin.x + mGradeBar->getContentSize().width/2  ,	origin.y + visibleSize.height - mGradeBar->getContentSize().height/2));
	this->addChild(mGradeBar,InfoPriority);

	//���_�Ǘ�
	mGradeManager = new GradeManager();
	mGradeManager->setSprite(this);

	//�����x�\��
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
	
	//�X�R�A�̏�����
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

//�X�V����
void Game::update(float delta){
	//�Q�[���J�n�O
	if(!mGameStarted){
		//�^�b�`�J�n��
		if(mTouchManager->getTouchObject(0)->touchBeganFlag){
			mTouchToStart->setVisible(false);
			mBlackBack->setVisible(false);
			mGameStarted = true;
			//�ȍĐ��J�n
			MusicData::playBGM(gMusicName);
			//�v���O���X�J�n
			mMusicProgress->startProgressAnim();
			//�|�[�Y�{�^���g�p��
			mMenu->setEnabled(true);
			//�v���C���[�A�j���[�V�����J�n
			mPlayer->resume();
		}
		mTouchManager->update(delta);
		return;
	}
	
	//�Q�[���J�n��̏���
	mPlayer->update(mGameFrame,delta);
	//�^�b�`����
	for(int i=0;i<mTouchManager->touchData->count();i++){
		//�^�b�v��
		if(mTouchManager->getTouchObject(i)->touchBeganFlag){
			if(mScoreManager->checkTap(mTouchManager->getTouchObject(i)->touch_began_x,mTouchManager->getTouchObject(i)->touch_began_y,mGradeManager)){
				if(!mPlayer->mIsMissed){
					//�o�b�h���o�����Ƃ�
					mPlayer->mIsMissed = true;
					mPlayer->mMissMotion = MotionData::DAMEGE_02;
				}
			}			
		}
		//�����[�X��
		if(mTouchManager->getTouchObject(i)->touchEndedFlag){
			if(mScoreManager->checkRelease(mTouchManager->getTouchObject(i)->touch_ended_x,mTouchManager->getTouchObject(i)->touch_ended_y,mGradeManager)){
				if(!mPlayer->mIsMissed){
					//�o�b�h���o�����Ƃ�
					mPlayer->mIsMissed = true;
					mPlayer->mMissMotion = MotionData::DAMEGE_02;
				}
			}			
		}
		//�z�[���h��
		if(mTouchManager->getTouchObject(i)->touchFlag){
			mScoreManager->checkHold(mTouchManager->getTouchObject(i)->touch_x,mTouchManager->getTouchObject(i)->touch_y,mGradeManager);			
		}
	}
	
	if(mScoreManager->update(mGameFrame,delta)){
		if(!mPlayer->mIsMissed){
			//�~�X���o�����Ƃ�
			mPlayer->mIsMissed = true;
			mPlayer->mMissMotion = MotionData::DAMEGE_01;
		}
	}

	if(!mShaked){
		//�V�F�C�N�𒲂ׂ�
		if(checkShake()){
			mShaked = true;
			//���ʉ��̖炷
			SEData::playSE(SE_TAP_NOTE);
			//�V�F�C�N�J�n���Ԃ�ۑ�
			mShakeStartTime = mGameSecond;
			//�V�F�C�N�̔���
			if(mScoreManager->checkShake(mGradeManager)){
				if(!mPlayer->mIsMissed){
					//�o�b�h���o�����Ƃ�
					mPlayer->mIsMissed = true;
					mPlayer->mMissMotion = MotionData::DAMEGE_02;
				}
			}
		}
	}else{
		//�V�F�C�N1�񕪂̎��Ԃ��o�߂�����
		if(mGameSecond - mShakeStartTime > mOneShakeTime){
			//���ɖ߂�
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

	//��ʐ؂�ւ�
	if(mMusicTime + 3.0f < mGameSecond){
		//�Ȃ��~�߂�
		MusicData::stopBGM();
		//�X�V���~�߂�
		this->unscheduleUpdate();
		//�X�R�A���擾
		mGradeManager->getTotalGrade();
		//�I���������ʈړ�
		CCScene* nextScene = Result::scene();
		CCScene* pScene = CCTransitionTurnOffTiles::create(1.0f,nextScene);
		if(pScene){
			CCDirector::sharedDirector()->replaceScene(pScene);
		}
	}
}

//�V�[�����n�܂������ɌĂяo����郁�\�b�h
void Game::onEnter(){
    CCLayer::onEnter();
	//�����x�Z���T�[�L��
	setAccelerometerEnabled(true);
	setAccelerometerInterval(1.0f);
		//�n�[�h�L�[�L��
	this->setKeypadEnabled(true);
}
//�V�[�����I��������ɌĂяo����郁�\�b�h
void Game::onExit(){
	CCLayer::onExit(); 
}
//�����x���擾����
void Game::didAccelerate(cocos2d::CCAcceleration* pAccel){
	//�����x�̎擾
	mAccel_x = pAccel->x;
	mAccel_y = pAccel->y;
	mAccel_z = pAccel->z;
	
	//�����x�\��
	//CCString* str = CCString::createWithFormat(
	//	" X:%1.3f, Y:%1.3f, Z:%1.3f",pAccel->x,pAccel->y,pAccel->z);
	//CCLabelTTF* label = (CCLabelTTF*)this->getChildByTag(1);
	//label->setString(str->getCString());

	//this->getCamera()->setEyeXYZ(
	//	pAccel->x * 0.00000063429789,
	//	pAccel->y * 0.00000095144683,
	//	0.0000001);
}

//�V�F�C�N���ꂽ�����ׂ�
bool Game::checkShake(){
	float total = fabs(mAccel_x) + fabs(mAccel_y) + fabs(mAccel_z);
	if(total > mShakeLevel){
		return true;
	}
	return false;
}

//�V���O���^�b�`
//bool Game::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//	//�^�b�`�������W���擾
//    CCPoint location =pTouch->getLocation();
//	mPlayer->setPlayerPos(location.x-100,location.y+375);
//
//    return true;
//     
//}
//void Game::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//	int width = CCDirector::sharedDirector()->getWinSize().width;
//	int height = CCDirector::sharedDirector()->getWinSize().height;
//	//�����ʒu�ɖ߂�
//	mPlayer->setPlayerPos(screenWidth() / 2 - 100 , screenHeight() / 2 + 375);
//}
//void Game::ccTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//
//	//�^�b�`�������W���擾
//	CCPoint location =pTouch->getLocation();
//	mPlayer->setPlayerPos(location.x-100,location.y+375);
//}

void Game::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	if(!mPaused){
		//�Q�[�����̂�
		if(mGameStarted && !mGameEnded){
			//���ʉ��̖炷
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


//�|�[�Y�������ꂽ�Ƃ�
void Game::pauseMenuCallback(CCObject* pSender){
	pauseOpenedCallback();
}

//�|�[�Y���J�n���ꂽ�Ƃ�
void Game::pauseOpenedCallback(){
	if(!mPaused){
		//�Q�[�����̂�
		if(mGameStarted && !mGameEnded){
			mPaused = true;
			//�X�V���~�߂�
			this->unscheduleUpdate();
			//�Ȃ��ꎞ��~������
			MusicData::pauseBGM();
			//�v���O���X���~������
			mMusicProgress->pauseProgressAnim(mGameSecond);
			//�|�[�Y�{�^��������
			mMenu->setVisible(false);
			//�|�[�Y���C���[�Ăяo��
			PauseLayer* pauseLayer = PauseLayer::create();
			this->addChild(pauseLayer,PausePriority,mPauseTagNum);
			pauseLayer->setParentLayer(this);
			//�Y�[���A�N�V����
			pauseLayer->setScale(0.5f);
			CCActionInterval* action = CCScaleTo::create(0.1f ,1.0f);
			action->setTag(0);
			pauseLayer->runAction(action);
			//�m�[�g�̃A�j���[�V�����ꎞ��~
			mScoreManager->pauseAnimation();
			//�L�����A�j���[�V�����ꎞ��~
			mPlayer->pause();
		}
	}
}

//�|�[�Y���������ꂽ�Ƃ�
void Game::pauseClosedCallback(){
	if(mPaused){
		mPaused = false;
		//�X�V���ĊJ����
		this->scheduleUpdate();
		//�Ȃ��ĊJ������
		MusicData::resumeBGM();
		//�v���O���X���ĊJ������
		mMusicProgress->startProgressAnim();
		//�|�[�Y�{�^�����o��
		mMenu->setVisible(true);
		//�m�[�g�̃A�j���[�V�����ĊJ
		mScoreManager->resumeAnimation();
		//�L�����A�j���[�V�����ĊJ
		mPlayer->resume();
	}
}

void Game::keyBackClicked(){
	//�|�[�Y�O
	if(!mPaused){
		//�|�[�Y����
		pauseOpenedCallback();
	}else{
		//�|�[�Y����
		pauseClosedCallback();
		this->removeChildByTag(mPauseTagNum);
	}
}

void Game::keyMenuClicked(){
	//�|�[�Y�O
	if(!mPaused){
		//�|�[�Y����
		pauseOpenedCallback();
	}else{
		//�|�[�Y����
		pauseClosedCallback();
		this->removeChildByTag(mPauseTagNum);
	}
}