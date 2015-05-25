#include "TouchEffect.h"
#include "GameScene.h"

TouchEffect::TouchEffect(){
	init();
}

TouchEffect::~TouchEffect(){
}

void TouchEffect::init(){
	//SSデータ読み込み
	mTouchEffectPlayer = ss::Player::create();
	mTouchEffectPlayer->setData("effect");
	
	mActive = false;
}

//アクティブかどうか
bool TouchEffect::isActive(){
	return mActive;
}

//レイヤーに登録する
void TouchEffect::setPlayer(CCLayer *layer){
	layer->addChild(mTouchEffectPlayer,TouchEffectPriority);
}

//タッチした座標を登録する
void TouchEffect::setPosition(int touchPos_x,int touchPos_y){
	mTouchEffectPlayer->setPositionX(touchPos_x);
	mTouchEffectPlayer->setPositionY(touchPos_y);
	mTouchEffectPlayer->clearLoopCount();
	mTouchEffectPlayer->play("tk_effect01_re/push01");
	mTouchEffectPlayer->setLoop(1);
	mTouchEffectPlayer->setFrameNo(0);
	mTouchEffectPlayer->setVisible(true);
	mActive = true;
}

void TouchEffect::update(float delta){
	//アクティブでなければ終了
	if(!mActive){
		return;
	}

	//フレームを進める
	mTouchEffectPlayer->update(delta);
	//アニメーションが終了した場合
	if(mTouchEffectPlayer->getLoopCount() == 1){
		mTouchEffectPlayer->setVisible(false);
		mActive = false;
	}
}
