#include "PlayerCharacter.h"
#include "GameScene.h"

using namespace cocos2d;

PlayerCharacter::PlayerCharacter(MusicName musicName){
	PlayerCharacter::init(musicName);
}

void PlayerCharacter::init(MusicName musicName){
	mSSPlayer = ss::Player::create();
	mSSPlayer->setData("tk01");
	mSSPlayer->setPosition( ccp( Game::screenWidth() * 0.5f, Game::screenHeight () * 0.5f - 420 ) );
	mSSPlayer->setLoop(1);
	mSSPlayer->play("tk01/wait");
	mSSPlayer->stop();


	//�o�b�N���C�g
	mBackSSPlayer = ss::Player::create();
	mBackSSPlayer->setData("stageanim");
	mBackSSPlayer->setPosition( ccp(Game::screenWidth() * 0.5f,Game::screenHeight () * 0.5f) );
	mBackSSPlayer->play("stage_effect/light4");

	//�Ȗ�������擾
	m_bpm = MusicData::getMusicBPM(musicName);
	mRhythm = MusicData::getMusicRhythm(musicName);
	MotionData::getMotionData(musicName,mMotionData);
	mOnePhraseBarNumber = 1;

	//���[�V��������_���X�̏I��������
	mMotionNumber = sizeof mMotionData /sizeof mMotionData[0];
	for(int i=0;i<mMotionNumber;i++){
		if(mMotionData[i] == MotionData::END){
			mMotionNumber = i;
			break;
		}
	}
	mBeforeMotion = -1;
	//�P�t���[�Y�̒������v�Z
	mOnePhraseFrame = 60 * mRhythm * 60 * mOnePhraseBarNumber / m_bpm;

	//�o�b�N���C�g�̑��x��ύX
	float step = (float)mBackSSPlayer->getMaxFrame() / mOnePhraseFrame;
	mBackSSPlayer->setStep(step);
	

	mIsMissed = false;
	mMissMotion = MotionData::DAMEGE_01;

	mMotionChanged = false;
}

//���[�V�������I��������true��Ԃ�
bool PlayerCharacter::update(int gameTime,float delta){
	bool motionEnded = false;
	//���݂̃��[�V�������v�Z
	int currentMotion = gameTime / mOnePhraseFrame;
	//�I�����Ă��Ȃ��Ƃ�
	if(currentMotion < mMotionNumber){
		//�~�X�����Ƃ�
		if(mIsMissed){
			//�~�X�����u��
			if(!mMotionChanged){
				mMotionChanged = true;
				changeMotion(mMissMotion, 0);
				mBeforeMotion = -1;
			}
			//���[�V�������I�������猳�̃A�j���[�V������
			if(mSSPlayer->getLoopCount() == 1){
				mIsMissed = false;
				mMotionChanged = false;
			}
		}else{
			//���[�V�������ύX���ꂽ��
			if(mBeforeMotion != currentMotion){
				mBeforeMotion = currentMotion;
				changeMotion(mMotionData[currentMotion],gameTime);
			}
		}
		
	}else{
		//�Ō�̃��[�V�����ŃX�g�b�v����
		//changeMotion(MotionData::WAIT);

	}

	return motionEnded;
}

//���[�V������ύX����
void PlayerCharacter::changeMotion(MotionData::PlayerMotion motion, int gameTime){
	float step = 0.0f;
	switch(motion){
	case MotionData::DANCE_01:
		mSSPlayer->play("dance01");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::DANCE_02:
		mSSPlayer->play("dance02");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::DANCE_03:
		mSSPlayer->play("dance03");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::DANCE_04:
		mSSPlayer->play("dance04");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::DANCE_05:
		mSSPlayer->play("dance05");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::POSE_01:
		mSSPlayer->play("pose01");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::POSE_02:
		mSSPlayer->play("pose02");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::STEP_01:
		mSSPlayer->play("step01");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::STEP_02:
		mSSPlayer->play("step02");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::WALK_01:
		mSSPlayer->play("walk01");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::WALK_02:
		mSSPlayer->play("walk02");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::JUMP:
		mSSPlayer->play("jump");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::WAIT:
		mSSPlayer->play("wait");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	case MotionData::DAMEGE_01:
		mSSPlayer->play("damege01");
		mSSPlayer->setStep( 0.5f );
		break;
	case MotionData::DAMEGE_02:
		mSSPlayer->play("damege02");
		mSSPlayer->setStep( 1.0f );
		break;
	case MotionData::DEF:
		mSSPlayer->play("def");
		step = (float)mSSPlayer->getMaxFrame() / mOnePhraseFrame;
		mSSPlayer->setStep(step);
		break;
	}
	if( motion == MotionData::DAMEGE_01 || motion == MotionData::DAMEGE_02 ){
		mSSPlayer->setFrameNo(0);
	}else{
		//�t���[���ʒu�𒲐�����
		int currentFrame = gameTime % mOnePhraseFrame;
		currentFrame *= mSSPlayer->getStep();
		if(currentFrame > mSSPlayer->getMaxFrame()-1){
			currentFrame = mSSPlayer->getMaxFrame()-1;
		}
		mSSPlayer->setFrameNo(currentFrame);
	}
	mSSPlayer->clearLoopCount();

}

void PlayerCharacter::setSSPlayer(cocos2d::CCLayer *layer){
	layer->addChild(mSSPlayer,CharacterPriority);
	layer->addChild(mBackSSPlayer,CharacterPriority+1);
}

void PlayerCharacter::pause(){
	mSSPlayer->pause();
	mBackSSPlayer->pause();
}

void PlayerCharacter::resume(){
	mSSPlayer->resume();
	mBackSSPlayer->resume();
}