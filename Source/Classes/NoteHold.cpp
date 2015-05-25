#include "NoteHold.h"

NoteHold::NoteHold(){
	init();
}

//開始と終了が同じ場所の場合
NoteHold::NoteHold(CCPoint startCood, int startFrame,int endFrame){
	mStartPos_x = startCood.x;
	mStartPos_y = startCood.y;
	mEndPos_x = startCood.x;
	mEndPos_y = startCood.y;
	mStartFrame = startFrame;
	mEndFrame = endFrame;
	init();
}
//開始と終了が違う場所の場合
NoteHold::NoteHold(CCPoint startCood, CCPoint endCood, int startFrame,int endFrame){
	mStartPos_x = startCood.x;
	mStartPos_y = startCood.y;
	mEndPos_x = endCood.x;
	mEndPos_y = endCood.y;
	mStartFrame = startFrame;
	mEndFrame = endFrame;
	init();
}

NoteHold::~NoteHold(){
}

void NoteHold::init(){
	base::mNoteType = base::HOLD;
	mCurrentPos_x = mStartPos_x;
	mCurrentPos_y = mStartPos_y;

	mNoteAnimationEnded = false;
	mValueActiveStart = false;
	mValueActiveEnd = false;
	mCheckTapFlag = false;
	mCheckHoldFlag = false;
	mCheckReleaseFlag = false;
	mCheckTapStarted = false;
	mCheckReleaseStarted = false;
	mNaviFlag = false;
	base::mNaviStarted = false;
}

//ミスした瞬間はtrueを返す
bool NoteHold::update(int currentFrame,float delta){
	bool missed = false;
	//フレームのずれを取得
	mDeltaFromEnd = mEndFrame - currentFrame;
	mDeltaFromStart = mStartFrame - currentFrame;
	//座標の更新
	getCurrentCood(&mCurrentPos_x,&mCurrentPos_y,currentFrame);
	//ノート画像の更新
	if(!mNoteAnimationEnded){
		mNoteAnimationEnded = mNoteHoldSprite->update(currentFrame,delta);
		//ノートの座標を更新する
		mNoteHoldSprite->mNotePlayer->setPositionX(mCurrentPos_x);
		mNoteHoldSprite->mNotePlayer->setPositionY(mCurrentPos_y);
	}

	//----開始時サークルの判定----
	//判定時間を超えた場合
	if(currentFrame >= mStartFrame + BadFrame){
		if(mCheckTapFlag){
			mCheckTapFlag = false;
			missed = true;
		}
	}
	//判定開始
	else if(currentFrame >= mStartFrame - BadFrame){
		if(!mCheckTapStarted){
			mCheckTapFlag = true;
			mCheckTapStarted = true;
		}
	}
	////評価画像の更新
	if(mValueActiveStart){
		mValuePlayerStart->update(delta);
		//アニメーションが終了したとき
		if(mValuePlayerStart->getLoopCount() == 1){
			mValuePlayerStart->setVisible(false);
			mValueActiveStart = false;
		}
	}

	//----終了時サークルの判定----
	//判定時間を超えた場合
	if(currentFrame >= mEndFrame + BadFrame){
		if(mCheckReleaseFlag){
			mCheckReleaseFlag = false;
			missed = true;
		}
	}
	//判定開始
	else if(currentFrame >= mEndFrame - BadFrame){
		if(!mCheckReleaseStarted){
			mCheckReleaseFlag = true;
			mCheckReleaseStarted = true;
		}
	}
	////評価画像の更新
	if(mValueActiveEnd){
		mValuePlayerEnd->update(delta);
		//アニメーションが終了したとき
		if(mValuePlayerEnd->getLoopCount() == 1){
			mValuePlayerEnd->setVisible(false);
			mValueActiveEnd = false;
		}
	}

	//----ホールドの判定----
	//判定時間を超えた場合
	if(currentFrame >= mEndFrame){
		mCheckHoldFlag = false;
	}
	//判定開始
	else if(currentFrame >= mStartFrame){
		mCheckHoldFlag = true;
	}


	//ナビ終了
	if(currentFrame >= mEndFrame){
		mNaviFlag = false;
	}
	//ナビ開始
	else if(currentFrame >= mStartFrame - BeforeActiveFrame){
		mNaviFlag = true;
	}
	
	return missed;
}

Note::NoteType NoteHold::getNoteType(){
	return base::mNoteType;
}

//ナビが開始したか
bool NoteHold::isNaviStarted(){
	return base::mNaviStarted;
}

//ナビを開始する
void NoteHold::naviStart(){
	base::mNaviStarted = true;
}

Note::Value NoteHold::checkHit(int touch_x,int touch_y,bool isStart){
	base::Value value = base::VALUE_NUM;
	int dx,dy;
	if(isStart){
		//タップを判定するか
		if(!mCheckTapFlag){
			return value;
		}
		//座標があってるかのチェック
		dx = touch_x - mStartPos_x;
		dy = touch_y - mStartPos_y;
		//当たっている
		if(dx * dx + dy * dy < HitRadiusHold * HitRadiusHold){
			value = getValue(isStart);
			mCheckTapFlag = false;
		}
	}else{
		//リリースを判定するか
		if(!mCheckReleaseFlag){
			return value;
		}
		//座標があってるかのチェック
		dx = touch_x - mEndPos_x;
		dy = touch_y - mEndPos_y;
		//当たっている
		if(dx * dx + dy * dy < HitRadiusHold * HitRadiusHold){
			value = getValue(isStart);
			mCheckReleaseFlag = false;
			//音符エフェクトを表示する
			mNoteHoldSprite->setReleased(true);
		}
	}
	
	
	return value;
}

bool NoteHold::checkHold(int touch_x,int touch_y){
	bool isHit = false;
	//ホールドを判定するか
	if(!mCheckHoldFlag){
		return isHit;
	}
	//座標があってるかのチェック
	int dx = touch_x - mCurrentPos_x;
	int dy = touch_y - mCurrentPos_y;
	//当たっている
	if(dx * dx + dy * dy < HitRadiusHold * HitRadiusHold){
		isHit = true;
	}
	return isHit;
}

Note::Value NoteHold::getValue(bool isStart){
	if(isStart){
		if( -CoolFrame <= mDeltaFromStart && mDeltaFromStart <= CoolFrame){
			return Note::COOL;
		}else if( -GreatFrame <= mDeltaFromStart && mDeltaFromStart <= GreatFrame){
			return Note::GREAT;
		}else if( -GoodFrame <= mDeltaFromStart && mDeltaFromStart <= GoodFrame){
			return Note::GOOD;
		}else{
			return Note::BAD;
		}
	}else{
		if( -CoolFrame <= mDeltaFromEnd && mDeltaFromEnd <= CoolFrame){
			return Note::COOL;
		}else if( -GreatFrame <= mDeltaFromEnd && mDeltaFromEnd <= GreatFrame){
			return Note::GREAT;
		}else if( -GoodFrame <= mDeltaFromEnd && mDeltaFromEnd <= GoodFrame){
			return Note::GOOD;
		}else{
			return Note::BAD;
		}
	}
}

void NoteHold::setNoteHoldSprite(NoteHoldSprite* nhSprite){
	mNoteHoldSprite = nhSprite;
	//必要な情報をセットしておく
	nhSprite->setNoteData(mStartPos_x,mStartPos_y,mEndPos_x,mEndPos_y,mStartFrame,mEndFrame);
}

//評価画像を登録する
void NoteHold::setValuePlayer(SSPlayer* valuePlayer,bool isStart){
	if(isStart){
		mValuePlayerStart = valuePlayer;
		mValuePlayerStart->setPositionX(mStartPos_x);
		mValuePlayerStart->setPositionY(mStartPos_y);
		mValuePlayerStart->setVisible(true);
		mValuePlayerStart->clearLoopCount();
		mValuePlayerStart->setFrameNo(0);
		mValueActiveStart = true;
	}else{
		mValuePlayerEnd = valuePlayer;
		mValuePlayerEnd->setPositionX(mEndPos_x);
		mValuePlayerEnd->setPositionY(mEndPos_y);
		mValuePlayerEnd->setVisible(true);
		mValuePlayerEnd->clearLoopCount();
		mValuePlayerEnd->setFrameNo(0);
		mValueActiveEnd = true;
	}

}

//現在のノートの座標を取得する
void NoteHold::getCurrentCood(int* current_x,int* current_y, int currentFrame){
	float move_x = (float)(mEndPos_x - mStartPos_x) / (mEndFrame-5 - mStartFrame);
	float move_y = (float)(mEndPos_y - mStartPos_y) / (mEndFrame-5 - mStartFrame);
	if(currentFrame <= mStartFrame){
		*current_x = mStartPos_x;
		*current_y = mStartPos_y;
	}else if(currentFrame >= mEndFrame){
		*current_x = mEndPos_x;
		*current_y = mEndPos_y;
	}else{
		int x = mStartPos_x + move_x * (currentFrame - mStartFrame);
		int y = mStartPos_y + move_y * (currentFrame - mStartFrame);
		//移動しすぎていないかチェックする
		if( (move_x < 0 && x < mEndPos_x) || (move_x > 0 && x > mEndPos_x) ){
			*current_x = mEndPos_x;
		}else{
			*current_x = x;
		}
		if( (move_y < 0 && y < mEndPos_y) || (move_y > 0 && y > mEndPos_y) ){
			*current_y = mEndPos_y;
		}else{
			*current_y = y;
		}		
	}
}

//ナビ用座標を取得する
CCPoint NoteHold::getNaviCood(){
	//ナビ用に先のフレーム先の座標を取得する
	int leadFrame = 20;
	//現在のフレームを逆算する
	int currenteFrame = mStartFrame- mDeltaFromStart;
	int navi_x,navi_y;
	getCurrentCood(&navi_x,&navi_y,currenteFrame + leadFrame);
	CCPoint naviCood = ccp(navi_x,navi_y);
	return naviCood;
}

//開始時のミスか調べる
bool NoteHold::isStartMissed(){
	bool isStart = false;
	//現在のフレームを逆算する
	int currenteFrame = mStartFrame- mDeltaFromStart;
	if(currenteFrame < mEndFrame){
		isStart = true; 
	}
	return true;
}