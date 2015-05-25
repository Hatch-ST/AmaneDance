#include "TouchEffect.h"
#include "GameScene.h"

TouchEffect::TouchEffect(){
	init();
}

TouchEffect::~TouchEffect(){
}

void TouchEffect::init(){
	//SS�f�[�^�ǂݍ���
	mTouchEffectPlayer = ss::Player::create();
	mTouchEffectPlayer->setData("effect");
	
	mActive = false;
}

//�A�N�e�B�u���ǂ���
bool TouchEffect::isActive(){
	return mActive;
}

//���C���[�ɓo�^����
void TouchEffect::setPlayer(CCLayer *layer){
	layer->addChild(mTouchEffectPlayer,TouchEffectPriority);
}

//�^�b�`�������W��o�^����
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
	//�A�N�e�B�u�łȂ���ΏI��
	if(!mActive){
		return;
	}

	//�t���[����i�߂�
	mTouchEffectPlayer->update(delta);
	//�A�j���[�V�������I�������ꍇ
	if(mTouchEffectPlayer->getLoopCount() == 1){
		mTouchEffectPlayer->setVisible(false);
		mActive = false;
	}
}
