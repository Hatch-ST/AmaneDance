#include "MusicProgress.h"
#include "GameScene.h"

MusicProgress::MusicProgress(MusicName musicName,CCLayer* layer){
	mMusicTime = MusicData::getMusicTime(musicName);
	init(layer);
}

MusicProgress::~MusicProgress(){
}

void MusicProgress::init(CCLayer* layer){
	CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage("interface/frame01.png");
	int width = 720;
	int height = 100;
	CCRect rect = CCRectMake(0, height , width, height);

	//プログレスバー
	mProgressBar = CCSprite::createWithTexture(pTexture,rect);
	mProgressBar->setPositionX(mProgressBar->getContentSize().width/2);
	mProgressBar->setPositionY(mProgressBar->getContentSize().height/2);
	layer->addChild(mProgressBar,InfoPriority);

	//プログレスアイコン
	mProgressIcon = CCSprite::create("progress_icon.png");
	//アイコンの座標
	mStartPos_x = mProgressIcon->getContentSize().width/2;
	mStartPos_y = mProgressIcon->getContentSize().height/2;
	mEndPos_x = CCDirector::sharedDirector()->getWinSize().width - mProgressIcon->getContentSize().width/2;
	mEndPos_y = mStartPos_y;

	mProgressIcon->setPositionX(mStartPos_x);
	mProgressIcon->setPositionY(mStartPos_y);	

	//デバッグ、ゴール地点表示
	//CCSprite* progressIcon = CCSprite::create("progress_icon.png");
	//progressIcon->setPositionX(mEndPos_x);
	//progressIcon->setPositionY(mEndPos_y);
	//layer->addChild(progressIcon);

	layer->addChild(mProgressIcon,InterfacePriority);
	mCurrentTime = 0.0f;
}

void MusicProgress::startProgressAnim(){
	CCActionInterval* action = CCMoveTo::create(mMusicTime - mCurrentTime ,ccp(mEndPos_x,mEndPos_y));
	action->setTag(0);
	mProgressIcon->runAction(action);
}

void MusicProgress::pauseProgressAnim(float currentTime){
	mProgressIcon->stopActionByTag(0);
	mCurrentTime = currentTime;
}
