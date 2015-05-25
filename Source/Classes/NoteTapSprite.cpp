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
	//SS�f�[�^�ǂݍ���
	//�m�[�g
	mNotePlayer = ss::Player::create();
	mNotePlayer->setData("effect");
	mNotePlayer->retain();

	//�T�[�N��
	mCirclePlayer = ss::Player::create();
	mCirclePlayer->setData("effect");
	mCirclePlayer->retain();

	//�m�[�g�G�t�F�N�g
	mNoteEffectPlayer = ss::Player::create();
	mNoteEffectPlayer->setData("effect");
	mNoteEffectPlayer->retain();

	mActive = false;
}

//�A�N�e�B�u���ǂ���
bool NoteTapSprite::isActive(){
	return mActive;
}

//���C���[�ɓo�^����
void NoteTapSprite::setPlayer(CCLayer *layer){
	layer->addChild(mNotePlayer,NotePriority);
	layer->addChild(mCirclePlayer,CirclePriority);
	layer->addChild(mNoteEffectPlayer,NotePriority);
}

//note�̏���o�^����
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

//�m�[�g�������ꂽ����ݒ肷��
void NoteTapSprite::setPressed(bool pressed){
	mPressed = pressed;
	mFirstFlag = true;
}

//�A�j���[�V�������I��������true��Ԃ�
bool NoteTapSprite::update(int currentFrame,float delta){
	bool animationEnded = false;
	//�A�N�e�B�u�łȂ���ΏI��
	if(!mActive){
		return animationEnded;
	}
	//������Ă��Ȃ��ꍇ
	if(!mPressed){
		//�t���[���̂�����擾
		mDeltaFromCorrect = mCorrectFrame - currentFrame;
		//�\���t���[�����O
		if(currentFrame < mCorrectFrame - BeforeActiveFrame){
			mNotePlayer->setVisible(false);
			mCirclePlayer->setVisible(false);			
		}
		//��\���t���[����
		else if(currentFrame >= mCorrectFrame + AfterActiveFrame){
			mNotePlayer->setVisible(false);
			mCirclePlayer->setVisible(false);
			mActive = false;
			animationEnded = true;
		}
		//�\���t���[������
		else if(currentFrame >= mCorrectFrame - BeforeActiveFrame){
			if(mFirstFlag){
				mNotePlayer->resume();
				mCirclePlayer->resume();

				mNotePlayer->setVisible(true);
				mCirclePlayer->setVisible(true);
				mFirstFlag = false;
				//�t���[���ʒu�𒲐�����
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
	//�����ꂽ�ꍇ
	else{
		if(mFirstFlag){
			mNotePlayer->setVisible(false);
			mCirclePlayer->setVisible(false);
			mFirstFlag = false;
			mNoteEffectPlayer->setVisible(true);
			mNoteEffectPlayer->resume();
		}
		//�P��A�j���[�V����������I��
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