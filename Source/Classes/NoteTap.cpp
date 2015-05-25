#include "NoteTap.h"

NoteTap::NoteTap(){
	init();
}

NoteTap::NoteTap(CCPoint cood, int correctFrame){
	mPos_x = cood.x;
	mPos_y = cood.y;
	mCorrectFrame = correctFrame;
	init();
}

NoteTap::~NoteTap(){
}

void NoteTap::init(){
	base::mNoteType = base::TAP;
	mNoteAnimationEnded = false;
	mValueActive = false;
	mCheckFlag = false;
	mCheckStarted = false;
	mNaviFlag = false;
	base::mNaviStarted = false;
}

//ミスした瞬間はtrueを返す
bool NoteTap::update(int currentFrame,float delta){
	bool missed = false;
	//フレームのずれを取得
	mDeltaFromCorrect = mCorrectFrame - currentFrame;
	//ノート画像の更新
	if(!mNoteAnimationEnded){
		mNoteAnimationEnded = mNoteTapSprite->update(currentFrame,delta);
	}
	//判定時間を超えた場合
	if(currentFrame >= mCorrectFrame + BadFrame){
		if(mCheckFlag){
			mCheckFlag = false;
			missed = true;
		}
	}
	//判定開始
	else if(currentFrame >= mCorrectFrame - BadFrame){
		if(!mCheckStarted){
			mCheckFlag = true;
			mCheckStarted = true;
		}
	}

	//評価画像の更新
	if(mValueActive){
		mValuePlayer->update(delta);
		//アニメーションが終了したとき
		if(mValuePlayer->getLoopCount() == 1){
			mValuePlayer->setVisible(false);
			mValueActive = false;
		}
	}
	//ナビ終了
	if(currentFrame >= mCorrectFrame){
		mNaviFlag = false;
	}
	//ナビ開始
	else if(currentFrame >= mCorrectFrame - BeforeActiveFrame){
		mNaviFlag = true;
	}
	
	return missed;
}

Note::NoteType NoteTap::getNoteType(){
	return base::mNoteType;
}

//ナビが開始したか
bool NoteTap::isNaviStarted(){
	return base::mNaviStarted;
}

//ナビを開始する
void NoteTap::naviStart(){
	base::mNaviStarted = true;
}

Note::Value NoteTap::checkTap(int touch_x,int touch_y){
	base::Value value = base::VALUE_NUM;
	//判定するかのフラグチェック
	if(!mCheckFlag){
		return value;
	}
	//座標があってるかのチェック
	int dx = touch_x - mPos_x;
	int dy = touch_y - mPos_y;
	//当たっている
	if(dx * dx + dy * dy < HitRadiusTap * HitRadiusTap){
		value = getValue();
		mCheckFlag = false;
		//音符エフェクトを表示する
		mNoteTapSprite->setPressed(true);
	}
	return value;
}

Note::Value NoteTap::checkShake(){
	base::Value value = base::VALUE_NUM;
	//判定するかのフラグチェック
	if(!mCheckFlag){
		return value;
	}

	value = getValue();
	mCheckFlag = false;
	//音符エフェクトを表示する
	mNoteTapSprite->setPressed(true);
	
	return value;
}

Note::Value NoteTap::getValue(){
	if( -CoolFrame <= mDeltaFromCorrect && mDeltaFromCorrect <= CoolFrame){
		return Note::COOL;
	}else if( -GreatFrame <= mDeltaFromCorrect && mDeltaFromCorrect <= GreatFrame){
		return Note::GREAT;
	}else if( -GoodFrame <= mDeltaFromCorrect && mDeltaFromCorrect <= GoodFrame){
		return Note::GOOD;
	}else{
		return Note::BAD;
	}
}

void NoteTap::setNoteTapSprite(NoteTapSprite* ntSprite){
	mNoteTapSprite = ntSprite;
	//必要な情報をセットしておく
	ntSprite->setNoteData(mPos_x,mPos_y,mCorrectFrame);
}

//評価画像を登録する
void NoteTap::setValuePlayer(ss::Player* valuePlayer){
	mValuePlayer = valuePlayer;
	mValuePlayer->setPositionX(mPos_x);
	mValuePlayer->setPositionY(mPos_y);
	mValuePlayer->setVisible(true);
	mValuePlayer->clearLoopCount();
	mValuePlayer->setFrameNo(0);
	mValueActive = true;
}