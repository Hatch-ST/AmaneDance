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

//�~�X�����u�Ԃ�true��Ԃ�
bool NoteTap::update(int currentFrame,float delta){
	bool missed = false;
	//�t���[���̂�����擾
	mDeltaFromCorrect = mCorrectFrame - currentFrame;
	//�m�[�g�摜�̍X�V
	if(!mNoteAnimationEnded){
		mNoteAnimationEnded = mNoteTapSprite->update(currentFrame,delta);
	}
	//���莞�Ԃ𒴂����ꍇ
	if(currentFrame >= mCorrectFrame + BadFrame){
		if(mCheckFlag){
			mCheckFlag = false;
			missed = true;
		}
	}
	//����J�n
	else if(currentFrame >= mCorrectFrame - BadFrame){
		if(!mCheckStarted){
			mCheckFlag = true;
			mCheckStarted = true;
		}
	}

	//�]���摜�̍X�V
	if(mValueActive){
		mValuePlayer->update(delta);
		//�A�j���[�V�������I�������Ƃ�
		if(mValuePlayer->getLoopCount() == 1){
			mValuePlayer->setVisible(false);
			mValueActive = false;
		}
	}
	//�i�r�I��
	if(currentFrame >= mCorrectFrame){
		mNaviFlag = false;
	}
	//�i�r�J�n
	else if(currentFrame >= mCorrectFrame - BeforeActiveFrame){
		mNaviFlag = true;
	}
	
	return missed;
}

Note::NoteType NoteTap::getNoteType(){
	return base::mNoteType;
}

//�i�r���J�n������
bool NoteTap::isNaviStarted(){
	return base::mNaviStarted;
}

//�i�r���J�n����
void NoteTap::naviStart(){
	base::mNaviStarted = true;
}

Note::Value NoteTap::checkTap(int touch_x,int touch_y){
	base::Value value = base::VALUE_NUM;
	//���肷�邩�̃t���O�`�F�b�N
	if(!mCheckFlag){
		return value;
	}
	//���W�������Ă邩�̃`�F�b�N
	int dx = touch_x - mPos_x;
	int dy = touch_y - mPos_y;
	//�������Ă���
	if(dx * dx + dy * dy < HitRadiusTap * HitRadiusTap){
		value = getValue();
		mCheckFlag = false;
		//�����G�t�F�N�g��\������
		mNoteTapSprite->setPressed(true);
	}
	return value;
}

Note::Value NoteTap::checkShake(){
	base::Value value = base::VALUE_NUM;
	//���肷�邩�̃t���O�`�F�b�N
	if(!mCheckFlag){
		return value;
	}

	value = getValue();
	mCheckFlag = false;
	//�����G�t�F�N�g��\������
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
	//�K�v�ȏ����Z�b�g���Ă���
	ntSprite->setNoteData(mPos_x,mPos_y,mCorrectFrame);
}

//�]���摜��o�^����
void NoteTap::setValuePlayer(ss::Player* valuePlayer){
	mValuePlayer = valuePlayer;
	mValuePlayer->setPositionX(mPos_x);
	mValuePlayer->setPositionY(mPos_y);
	mValuePlayer->setVisible(true);
	mValuePlayer->clearLoopCount();
	mValuePlayer->setFrameNo(0);
	mValueActive = true;
}