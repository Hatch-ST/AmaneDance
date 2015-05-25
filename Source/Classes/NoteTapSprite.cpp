#include "NoteTapSprite.h"
#include "GameScene.h"

NoteTapSprite::NoteTapSprite(){
	init();
}

NoteTapSprite::~NoteTapSprite(){
	mNotePlayer->release();
	mCirclePlayer->release();
	mNoteEffectPlayer->release();
}

void NoteTapSprite::init(){
	//SSデータ読み込み
	//ノート
	mNotePlayer = ss::Player::create();
	mNotePlayer->setData("effect");
	mNotePlayer->retain();

	//サークル
	mCirclePlayer = ss::Player::create();
	mCirclePlayer->setData("effect");
	mCirclePlayer->retain();

	//ノートエフェクト
	mNoteEffectPlayer = ss::Player::create();
	mNoteEffectPlayer->setData("effect");
	mNoteEffectPlayer->retain();

	mActive = false;
}

//アクティブかどうか
bool NoteTapSprite::isActive(){
	return mActive;
}

//レイヤーに登録する
void NoteTapSprite::setPlayer(CCLayer *layer){
	layer->addChild(mNotePlayer,NotePriority);
	layer->addChild(mCirclePlayer,CirclePriority);
	layer->addChild(mNoteEffectPlayer,NotePriority);
}

//noteの情報を登録する
void NoteTapSprite::setNoteData(int notePos_x,int notePos_y,int correctFrame){
	mNotePlayer->setPositionX(notePos_x);
	mNotePlayer->setPositionY(notePos_y);
	mNotePlayer->clearLoopCount();
	mNotePlayer->play("timingnote_re/anime_1");
	mNotePlayer->setVisible(false);
	mNotePlayer->setLoop(1);
	mNotePlayer->pause();
	mCirclePlayer->setPositionX(notePos_x);
	mCirclePlayer->setPositionY(notePos_y);
	mCirclePlayer->clearLoopCount();
	mCirclePlayer->play("timingcircle_re/anime_1");
	mCirclePlayer->setVisible(false);
	mCirclePlayer->setLoop(1);
	mCirclePlayer->pause();
	mNoteEffectPlayer->setPositionX(notePos_x);
	mNoteEffectPlayer->setPositionY(notePos_y);
	mNoteEffectPlayer->clearLoopCount();
	mNoteEffectPlayer->play("onpu1_re/anime_1");
	mNoteEffectPlayer->setVisible(false);
	mNoteEffectPlayer->setLoop(1);
	mNoteEffectPlayer->pause();
	mCorrectFrame = correctFrame;
	mActive = true;
	mFirstFlag = true;
	mPressed = false;
}

//ノートが押されたかを設定する
void NoteTapSprite::setPressed(bool pressed){
	mPressed = pressed;
	mFirstFlag = true;
}

//アニメーションが終了したらtrueを返す
bool NoteTapSprite::update(int currentFrame,float delta){
	bool animationEnded = false;
	//アクティブでなければ終了
	if(!mActive){
		return animationEnded;
	}
	//押されていない場合
	if(!mPressed){
		//フレームのずれを取得
		mDeltaFromCorrect = mCorrectFrame - currentFrame;
		//表示フレームより前
		if(currentFrame < mCorrectFrame - BeforeActiveFrame){
			mNotePlayer->setVisible(false);
			mCirclePlayer->setVisible(false);			
		}
		//非表示フレーム後
		else if(currentFrame >= mCorrectFrame + AfterActiveFrame){
			mNotePlayer->setVisible(false);
			mCirclePlayer->setVisible(false);
			mActive = false;
			animationEnded = true;
		}
		//表示フレームより後
		else if(currentFrame >= mCorrectFrame - BeforeActiveFrame){
			if(mFirstFlag){
				mNotePlayer->resume();
				mCirclePlayer->resume();

				mNotePlayer->setVisible(true);
				mCirclePlayer->setVisible(true);
				mFirstFlag = false;
				//フレーム位置を調整する
				int frame = BeforeActiveFrame - mDeltaFromCorrect;
				if(frame > mNotePlayer->getMaxFrame()-1){
					frame = mNotePlayer->getMaxFrame()-1;
				}
				if(frame > mCirclePlayer->getMaxFrame()-1){
					frame = mCirclePlayer->getMaxFrame()-1;
				}
				mNotePlayer->setFrameNo(frame);
				mCirclePlayer->setFrameNo(frame);
			}

			
		}
	}
	//押された場合
	else{
		if(mFirstFlag){
			mNotePlayer->setVisible(false);
			mCirclePlayer->setVisible(false);
			mFirstFlag = false;
			mNoteEffectPlayer->setVisible(true);
			mNoteEffectPlayer->resume();
		}
		//１回アニメーションしたら終了
		if(mNoteEffectPlayer->getLoopCount() == 1){
			mNoteEffectPlayer->setVisible(false);
			mActive = false;
			animationEnded = true;
		}
	}
	return animationEnded;
}

void NoteTapSprite::pauseAnimation(){
	mNotePlayer->pause();
	mCirclePlayer->pause();
	mNoteEffectPlayer->pause();
}

void NoteTapSprite::resumeAnimation(){
	mNotePlayer->resume();
	mCirclePlayer->resume();
	mNoteEffectPlayer->resume();

}