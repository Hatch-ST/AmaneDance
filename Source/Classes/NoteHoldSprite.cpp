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
	//SS�f�[�^�ǂݍ���
	//�m�[�g
	unsigned char* data = SSPlayerHelper::loadFile("timingnote_anime_1.ssba","effect/");
	SSData* ssdata = reinterpret_cast<SSData*>(data);
	mNotePlayer = SSPlayer::create(ssdata,mEffectImage);
	mNotePlayer->setVisible(false);
	mNotePlayer->setLoop(1);
	mNotePlayer->setColor(ccc3(150,150,255));
	mNotePlayer->retain();

	//�J�n���T�[�N��
	data = SSPlayerHelper::loadFile("timingcircle_anime_1.ssba","effect/");
	ssdata = reinterpret_cast<SSData*>(data);
	mCirclePlayerStart = SSPlayer::create(ssdata,mEffectImage);
	mCirclePlayerStart->setVisible(false);
	mCirclePlayerStart->setLoop(1);
	mCirclePlayerStart->retain();

	//�I�����T�[�N��
	mCirclePlayerEnd = SSPlayer::create(ssdata,mEffectImage);
	mCirclePlayerEnd->setVisible(false);
	mCirclePlayerEnd->setLoop(1);
	mCirclePlayerEnd->retain();

	//�m�[�g�G�t�F�N�g
	data = SSPlayerHelper::loadFile("onpu2_anime_1.ssba","effect/");
	ssdata = reinterpret_cast<SSData*>(data);
	mNoteEffectPlayer = SSPlayer::create(ssdata,mEffectImage);
	mNoteEffectPlayer->setVisible(false);
	mNoteEffectPlayer->setLoop(1);
	mNoteEffectPlayer->retain();

	mActive = false;
}

//�A�N�e�B�u���ǂ���
bool NoteHoldSprite::isActive(){
	return mActive;
}

//���C���[�ɓo�^����
void NoteHoldSprite::setPlayer(CCLayer *layer){
	layer->addChild(mNotePlayer,NotePriority);
	layer->addChild(mCirclePlayerStart,CirclePriority);
	layer->addChild(mCirclePlayerEnd,CirclePriority);
	layer->addChild(mNoteEffectPlayer,NotePriority);
}

//note�̏���o�^����
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

//�m�[�g�������ꂽ����ݒ肷��
void NoteHoldSprite::setPressed(bool pressed){
	mPressed = pressed;
	mCircleStartFirstFlag = true;
}

//�m�[�g�������ꂽ����ݒ肷��
void NoteHoldSprite::setReleased(bool released){
	mReleased = released;
	mNoteFirstFlag = true;
}

//�A�j���[�V�������I��������true��Ԃ�
bool NoteHoldSprite::update(int currentFrame,float delta){
	bool animationEnded = false;
	//�A�N�e�B�u�łȂ���ΏI��
	if(!mActive){
		return animationEnded;
	}
	//�t���[���̂�����擾
	mDeltaFromEnd = mEndFrame - currentFrame;
	mDeltaFromStart = mStartFrame - currentFrame;

	//----�J�n���T�[�N��----
	//������Ă��Ȃ��ꍇ
	if(!mPressed){
		//�\���t���[�����O
		if(currentFrame < mStartFrame - BeforeActiveFrame){
			mCirclePlayerStart->setVisible(false);
		}
		//��\���t���[����
		else if(currentFrame >= mStartFrame + AfterActiveFrame){
			mCirclePlayerStart->setVisible(false);
		}
		//�\���t���[������
		else if(currentFrame >= mStartFrame - BeforeActiveFrame){
			if(mCircleStartFirstFlag){
				mCirclePlayerStart->setVisible(true);
				mCircleStartFirstFlag = false;
				//�t���[���ʒu�𒲐�����
				int frame = BeforeActiveFrame - mDeltaFromStart;
				if(frame > mCirclePlayerStart->getNumFrames()-1){
					frame = mCirclePlayerStart->getNumFrames()-1;
				}
				mCirclePlayerStart->setFrameNo(frame);
			}
			//�t���[����i�߂�
			mCirclePlayerStart->update(delta);
		}
	//�����ꂽ�ꍇ
	}else{
		if(mCircleStartFirstFlag){
			mCirclePlayerStart->setVisible(false);
			mCircleStartFirstFlag = false;
		}
	}

	//������Ă��Ȃ��ꍇ
	if(!mReleased){
		//----�I�����T�[�N��----
		//�\���t���[�����O
		if(currentFrame < mEndFrame - BeforeActiveFrame){
			mCirclePlayerEnd->setVisible(false);
		}
		//��\���t���[����
		else if(currentFrame >= mEndFrame + AfterActiveFrame){
			mCirclePlayerEnd->setVisible(false);
		}
		//�\���t���[������
		else if(currentFrame >= mEndFrame - BeforeActiveFrame){
			if(mCircleEndFirstFlag){
				mCirclePlayerEnd->setVisible(true);
				mCircleEndFirstFlag = false;
				//�t���[���ʒu�𒲐�����
				int frame = BeforeActiveFrame - mDeltaFromEnd;
				if(frame > mCirclePlayerEnd->getNumFrames()-1){
					frame = mCirclePlayerEnd->getNumFrames()-1;
				}
				mCirclePlayerEnd->setFrameNo(143);
				mCirclePlayerEnd->update(delta);
			}
			//�t���[����i�߂�
			//mCirclePlayerEnd->update(delta);
		}

		//----�m�[�g----
		//�\���t���[�����O
		if(currentFrame < mStartFrame - BeforeActiveFrame){
			mNotePlayer->setVisible(false);
		}
		//��\���t���[����
		else if(currentFrame >= mEndFrame + AfterActiveFrame){
			mNotePlayer->setVisible(false);
			animationEnded = true;
			mActive = false;
			animationEnded = true;
		}
		//�t�F�[�h�A�E�g
		else if(currentFrame >= mEndFrame){
			if(mNoteFirstFlag){
				mNoteFirstFlag = false;
				//�t���[���ʒu�𒲐�����
				int frame =  169 - mDeltaFromEnd;
				if(frame > mNotePlayer->getNumFrames()-1){
					frame = mNotePlayer->getNumFrames()-1;
				}
				mNotePlayer->setFrameNo(frame);
			}
			//�t���[����i�߂�
			mNotePlayer->update(delta);
		}
		//�z�[���h��
		else if(currentFrame >= mStartFrame){
			mNoteFirstFlag = true;
		}
		//�t�F�[�h�C��
		else if(currentFrame >= mStartFrame - BeforeActiveFrame){
			if(mNoteFirstFlag){
				mNotePlayer->setVisible(true);
				mNoteFirstFlag = false;
				//�t���[���ʒu�𒲐�����
				int frame = BeforeActiveFrame - mDeltaFromStart;
				if(frame > mNotePlayer->getNumFrames()-1){
					frame = mNotePlayer->getNumFrames()-1;
				}
				mNotePlayer->setFrameNo(frame);
			}
			//�t���[����i�߂�
			mNotePlayer->update(delta);
		}

	//�����ꂽ�ꍇ
	}else{
		if(mNoteFirstFlag){
			mCirclePlayerEnd->setVisible(false);
			mCircleEndFirstFlag = false;
			mNotePlayer->setVisible(false);
			mNoteFirstFlag = false;
			mNoteEffectPlayer->setVisible(true);
		}
		//�t���[����i�߂�
		mNoteEffectPlayer->update(delta);
		//�P��A�j���[�V����������I��
		if(mNoteEffectPlayer->getLoopCount() == 1){
			mNoteEffectPlayer->setVisible(false);
			mActive = false;
			animationEnded = true;
		}
	}

	return animationEnded;
}
