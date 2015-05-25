#include "NoteHoldSprite.h"
#include "GameScene.h"

NoteHoldSprite::NoteHoldSprite(SSImageList* effectImage){
	mEffectImage = effectImage;
	init();
}

NoteHoldSprite::~NoteHoldSprite(){
	mNotePlayer->release();
	mCirclePlayerStart->release();
	mCirclePlayerEnd->release();
	mNoteEffectPlayer->release();
}

void NoteHoldSprite::init(){
	//SSデータ読み込み
	//ノート
	unsigned char* data = SSPlayerHelper::loadFile("timingnote_anime_1.ssba","effect/");
	SSData* ssdata = reinterpret_cast<SSData*>(data);
	mNotePlayer = SSPlayer::create(ssdata,mEffectImage);
	mNotePlayer->setVisible(false);
	mNotePlayer->setLoop(1);
	mNotePlayer->setColor(ccc3(150,150,255));
	mNotePlayer->retain();

	//開始時サークル
	data = SSPlayerHelper::loadFile("timingcircle_anime_1.ssba","effect/");
	ssdata = reinterpret_cast<SSData*>(data);
	mCirclePlayerStart = SSPlayer::create(ssdata,mEffectImage);
	mCirclePlayerStart->setVisible(false);
	mCirclePlayerStart->setLoop(1);
	mCirclePlayerStart->retain();

	//終了時サークル
	mCirclePlayerEnd = SSPlayer::create(ssdata,mEffectImage);
	mCirclePlayerEnd->setVisible(false);
	mCirclePlayerEnd->setLoop(1);
	mCirclePlayerEnd->retain();

	//ノートエフェクト
	data = SSPlayerHelper::loadFile("onpu2_anime_1.ssba","effect/");
	ssdata = reinterpret_cast<SSData*>(data);
	mNoteEffectPlayer = SSPlayer::create(ssdata,mEffectImage);
	mNoteEffectPlayer->setVisible(false);
	mNoteEffectPlayer->setLoop(1);
	mNoteEffectPlayer->retain();

	mActive = false;
}

//アクティブかどうか
bool NoteHoldSprite::isActive(){
	return mActive;
}

//レイヤーに登録する
void NoteHoldSprite::setPlayer(CCLayer *layer){
	layer->addChild(mNotePlayer,NotePriority);
	layer->addChild(mCirclePlayerStart,CirclePriority);
	layer->addChild(mCirclePlayerEnd,CirclePriority);
	layer->addChild(mNoteEffectPlayer,NotePriority);
}

//noteの情報を登録する
void NoteHoldSprite::setNoteData(int startPos_x,int startPos_y,int endPos_x,int endPos_y,int startFrame,int endFrame){
	mNotePlayer->setPositionX(startPos_x);
	mNotePlayer->setPositionY(startPos_y);
	mNotePlayer->clearLoopCount();
	mCirclePlayerStart->setPositionX(startPos_x);
	mCirclePlayerStart->setPositionY(startPos_y);
	mCirclePlayerStart->clearLoopCount();
	mCirclePlayerEnd->setPositionX(endPos_x);
	mCirclePlayerEnd->setPositionY(endPos_y);
	mCirclePlayerEnd->clearLoopCount();
	mNoteEffectPlayer->setPositionX(endPos_x);
	mNoteEffectPlayer->setPositionY(endPos_y);
	mNoteEffectPlayer->clearLoopCount();
	mNoteEffectPlayer->setVisible(false);
	mStartFrame = startFrame;
	mEndFrame = endFrame;
	mActive = true;
	mCircleStartFirstFlag = true;
	mCircleEndFirstFlag = true;
	mNoteFirstFlag = true;
	mPressed = false;
	mReleased = false;
}

//ノートが押されたかを設定する
void NoteHoldSprite::setPressed(bool pressed){
	mPressed = pressed;
	mCircleStartFirstFlag = true;
}

//ノートが離されたかを設定する
void NoteHoldSprite::setReleased(bool released){
	mReleased = released;
	mNoteFirstFlag = true;
}

//アニメーションが終了したらtrueを返す
bool NoteHoldSprite::update(int currentFrame,float delta){
	bool animationEnded = false;
	//アクティブでなければ終了
	if(!mActive){
		return animationEnded;
	}
	//フレームのずれを取得
	mDeltaFromEnd = mEndFrame - currentFrame;
	mDeltaFromStart = mStartFrame - currentFrame;

	//----開始時サークル----
	//押されていない場合
	if(!mPressed){
		//表示フレームより前
		if(currentFrame < mStartFrame - BeforeActiveFrame){
			mCirclePlayerStart->setVisible(false);
		}
		//非表示フレーム後
		else if(currentFrame >= mStartFrame + AfterActiveFrame){
			mCirclePlayerStart->setVisible(false);
		}
		//表示フレームより後
		else if(currentFrame >= mStartFrame - BeforeActiveFrame){
			if(mCircleStartFirstFlag){
				mCirclePlayerStart->setVisible(true);
				mCircleStartFirstFlag = false;
				//フレーム位置を調整する
				int frame = BeforeActiveFrame - mDeltaFromStart;
				if(frame > mCirclePlayerStart->getNumFrames()-1){
					frame = mCirclePlayerStart->getNumFrames()-1;
				}
				mCirclePlayerStart->setFrameNo(frame);
			}
			//フレームを進める
			mCirclePlayerStart->update(delta);
		}
	//押された場合
	}else{
		if(mCircleStartFirstFlag){
			mCirclePlayerStart->setVisible(false);
			mCircleStartFirstFlag = false;
		}
	}

	//離されていない場合
	if(!mReleased){
		//----終了時サークル----
		//表示フレームより前
		if(currentFrame < mEndFrame - BeforeActiveFrame){
			mCirclePlayerEnd->setVisible(false);
		}
		//非表示フレーム後
		else if(currentFrame >= mEndFrame + AfterActiveFrame){
			mCirclePlayerEnd->setVisible(false);
		}
		//表示フレームより後
		else if(currentFrame >= mEndFrame - BeforeActiveFrame){
			if(mCircleEndFirstFlag){
				mCirclePlayerEnd->setVisible(true);
				mCircleEndFirstFlag = false;
				//フレーム位置を調整する
				int frame = BeforeActiveFrame - mDeltaFromEnd;
				if(frame > mCirclePlayerEnd->getNumFrames()-1){
					frame = mCirclePlayerEnd->getNumFrames()-1;
				}
				mCirclePlayerEnd->setFrameNo(143);
				mCirclePlayerEnd->update(delta);
			}
			//フレームを進める
			//mCirclePlayerEnd->update(delta);
		}

		//----ノート----
		//表示フレームより前
		if(currentFrame < mStartFrame - BeforeActiveFrame){
			mNotePlayer->setVisible(false);
		}
		//非表示フレーム後
		else if(currentFrame >= mEndFrame + AfterActiveFrame){
			mNotePlayer->setVisible(false);
			animationEnded = true;
			mActive = false;
			animationEnded = true;
		}
		//フェードアウト
		else if(currentFrame >= mEndFrame){
			if(mNoteFirstFlag){
				mNoteFirstFlag = false;
				//フレーム位置を調整する
				int frame =  169 - mDeltaFromEnd;
				if(frame > mNotePlayer->getNumFrames()-1){
					frame = mNotePlayer->getNumFrames()-1;
				}
				mNotePlayer->setFrameNo(frame);
			}
			//フレームを進める
			mNotePlayer->update(delta);
		}
		//ホールド中
		else if(currentFrame >= mStartFrame){
			mNoteFirstFlag = true;
		}
		//フェードイン
		else if(currentFrame >= mStartFrame - BeforeActiveFrame){
			if(mNoteFirstFlag){
				mNotePlayer->setVisible(true);
				mNoteFirstFlag = false;
				//フレーム位置を調整する
				int frame = BeforeActiveFrame - mDeltaFromStart;
				if(frame > mNotePlayer->getNumFrames()-1){
					frame = mNotePlayer->getNumFrames()-1;
				}
				mNotePlayer->setFrameNo(frame);
			}
			//フレームを進める
			mNotePlayer->update(delta);
		}

	//離された場合
	}else{
		if(mNoteFirstFlag){
			mCirclePlayerEnd->setVisible(false);
			mCircleEndFirstFlag = false;
			mNotePlayer->setVisible(false);
			mNoteFirstFlag = false;
			mNoteEffectPlayer->setVisible(true);
		}
		//フレームを進める
		mNoteEffectPlayer->update(delta);
		//１回アニメーションしたら終了
		if(mNoteEffectPlayer->getLoopCount() == 1){
			mNoteEffectPlayer->setVisible(false);
			mActive = false;
			animationEnded = true;
		}
	}

	return animationEnded;
}
