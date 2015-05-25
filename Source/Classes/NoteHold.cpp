#include "NoteHold.h"

NoteHold::NoteHold(){
	init();
}

//�J�n�ƏI���������ꏊ�̏ꍇ
NoteHold::NoteHold(CCPoint startCood, int startFrame,int endFrame){
	mStartPos_x = startCood.x;
	mStartPos_y = startCood.y;
	mEndPos_x = startCood.x;
	mEndPos_y = startCood.y;
	mStartFrame = startFrame;
	mEndFrame = endFrame;
	init();
}
//�J�n�ƏI�����Ⴄ�ꏊ�̏ꍇ
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

//�~�X�����u�Ԃ�true��Ԃ�
bool NoteHold::update(int currentFrame,float delta){
	bool missed = false;
	//�t���[���̂�����擾
	mDeltaFromEnd = mEndFrame - currentFrame;
	mDeltaFromStart = mStartFrame - currentFrame;
	//���W�̍X�V
	getCurrentCood(&mCurrentPos_x,&mCurrentPos_y,currentFrame);
	//�m�[�g�摜�̍X�V
	if(!mNoteAnimationEnded){
		mNoteAnimationEnded = mNoteHoldSprite->update(currentFrame,delta);
		//�m�[�g�̍��W���X�V����
		mNoteHoldSprite->mNotePlayer->setPositionX(mCurrentPos_x);
		mNoteHoldSprite->mNotePlayer->setPositionY(mCurrentPos_y);
	}

	//----�J�n���T�[�N���̔���----
	//���莞�Ԃ𒴂����ꍇ
	if(currentFrame >= mStartFrame + BadFrame){
		if(mCheckTapFlag){
			mCheckTapFlag = false;
			missed = true;
		}
	}
	//����J�n
	else if(currentFrame >= mStartFrame - BadFrame){
		if(!mCheckTapStarted){
			mCheckTapFlag = true;
			mCheckTapStarted = true;
		}
	}
	////�]���摜�̍X�V
	if(mValueActiveStart){
		mValuePlayerStart->update(delta);
		//�A�j���[�V�������I�������Ƃ�
		if(mValuePlayerStart->getLoopCount() == 1){
			mValuePlayerStart->setVisible(false);
			mValueActiveStart = false;
		}
	}

	//----�I�����T�[�N���̔���----
	//���莞�Ԃ𒴂����ꍇ
	if(currentFrame >= mEndFrame + BadFrame){
		if(mCheckReleaseFlag){
			mCheckReleaseFlag = false;
			missed = true;
		}
	}
	//����J�n
	else if(currentFrame >= mEndFrame - BadFrame){
		if(!mCheckReleaseStarted){
			mCheckReleaseFlag = true;
			mCheckReleaseStarted = true;
		}
	}
	////�]���摜�̍X�V
	if(mValueActiveEnd){
		mValuePlayerEnd->update(delta);
		//�A�j���[�V�������I�������Ƃ�
		if(mValuePlayerEnd->getLoopCount() == 1){
			mValuePlayerEnd->setVisible(false);
			mValueActiveEnd = false;
		}
	}

	//----�z�[���h�̔���----
	//���莞�Ԃ𒴂����ꍇ
	if(currentFrame >= mEndFrame){
		mCheckHoldFlag = false;
	}
	//����J�n
	else if(currentFrame >= mStartFrame){
		mCheckHoldFlag = true;
	}


	//�i�r�I��
	if(currentFrame >= mEndFrame){
		mNaviFlag = false;
	}
	//�i�r�J�n
	else if(currentFrame >= mStartFrame - BeforeActiveFrame){
		mNaviFlag = true;
	}
	
	return missed;
}

Note::NoteType NoteHold::getNoteType(){
	return base::mNoteType;
}

//�i�r���J�n������
bool NoteHold::isNaviStarted(){
	return base::mNaviStarted;
}

//�i�r���J�n����
void NoteHold::naviStart(){
	base::mNaviStarted = true;
}

Note::Value NoteHold::checkHit(int touch_x,int touch_y,bool isStart){
	base::Value value = base::VALUE_NUM;
	int dx,dy;
	if(isStart){
		//�^�b�v�𔻒肷�邩
		if(!mCheckTapFlag){
			return value;
		}
		//���W�������Ă邩�̃`�F�b�N
		dx = touch_x - mStartPos_x;
		dy = touch_y - mStartPos_y;
		//�������Ă���
		if(dx * dx + dy * dy < HitRadiusHold * HitRadiusHold){
			value = getValue(isStart);
			mCheckTapFlag = false;
		}
	}else{
		//�����[�X�𔻒肷�邩
		if(!mCheckReleaseFlag){
			return value;
		}
		//���W�������Ă邩�̃`�F�b�N
		dx = touch_x - mEndPos_x;
		dy = touch_y - mEndPos_y;
		//�������Ă���
		if(dx * dx + dy * dy < HitRadiusHold * HitRadiusHold){
			value = getValue(isStart);
			mCheckReleaseFlag = false;
			//�����G�t�F�N�g��\������
			mNoteHoldSprite->setReleased(true);
		}
	}
	
	
	return value;
}

bool NoteHold::checkHold(int touch_x,int touch_y){
	bool isHit = false;
	//�z�[���h�𔻒肷�邩
	if(!mCheckHoldFlag){
		return isHit;
	}
	//���W�������Ă邩�̃`�F�b�N
	int dx = touch_x - mCurrentPos_x;
	int dy = touch_y - mCurrentPos_y;
	//�������Ă���
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
	//�K�v�ȏ����Z�b�g���Ă���
	nhSprite->setNoteData(mStartPos_x,mStartPos_y,mEndPos_x,mEndPos_y,mStartFrame,mEndFrame);
}

//�]���摜��o�^����
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

//���݂̃m�[�g�̍��W���擾����
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
		//�ړ��������Ă��Ȃ����`�F�b�N����
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

//�i�r�p���W���擾����
CCPoint NoteHold::getNaviCood(){
	//�i�r�p�ɐ�̃t���[����̍��W���擾����
	int leadFrame = 20;
	//���݂̃t���[�����t�Z����
	int currenteFrame = mStartFrame- mDeltaFromStart;
	int navi_x,navi_y;
	getCurrentCood(&navi_x,&navi_y,currenteFrame + leadFrame);
	CCPoint naviCood = ccp(navi_x,navi_y);
	return naviCood;
}

//�J�n���̃~�X�����ׂ�
bool NoteHold::isStartMissed(){
	bool isStart = false;
	//���݂̃t���[�����t�Z����
	int currenteFrame = mStartFrame- mDeltaFromStart;
	if(currenteFrame < mEndFrame){
		isStart = true; 
	}
	return true;
}