#include "ScoreManager.h"



#ifdef G_GRADE_DATA
struct GradeData gGradeData;
#else
extern struct GradeData gGradeData;
#endif

//�R�X�g���N�^
ScoreManager::ScoreManager(MusicName musicName,cocos2d::CCLayer *layer){
	init(musicName,layer);
}

ScoreManager::~ScoreManager(){
	mPhraseQueue->release();
}

//������
void ScoreManager::init(MusicName musicName,cocos2d::CCLayer *layer){
	//�Ȗ�������擾
	m_bpm = MusicData::getMusicBPM(musicName);
	mRhythm = MusicData::getMusicRhythm(musicName);
	mOnePhraseBarNumber = 1;
	//1�t���[�Y�̊�̒���
	mBaseFrame = 120 * mOnePhraseBarNumber;

	//�P�t���[�Y�̒������v�Z
	mOnePhraseFrame = 60 * mRhythm * 60 * mOnePhraseBarNumber / m_bpm;
	mStep = (float) mOnePhraseFrame / mBaseFrame;
	ScoreData::getScoreData(musicName,mScoreData);
	mScoreMaxNumber = sizeof mScoreData / sizeof mScoreData[0];
	for(int i=0;i<mScoreMaxNumber;i++){
		if(mScoreData[i] == ScoreData::END){
			mScoreMaxNumber = i;
			break;
		}
	}
	mCurrentPhraseNumber = 0;
	mCurrentNumberOfQueue = AfterActiveFrame / mOnePhraseFrame + 1;
	mQueueSize = 2 + BeforeActiveFrame / mOnePhraseFrame + mCurrentNumberOfQueue;

	//�m�[�g�^�b�v�̉摜
	mNoteTapSprite = CCArray::create();
	mNoteTapSprite->retain();
	for(int i=0;i<mNoteTapSpriteNumber;i++){
		NoteTapSprite* ntSprite = new NoteTapSprite();
		mNoteTapSprite->addObject(ntSprite);
		ntSprite->setPlayer(layer);
	}
	//�m�[�g�z�[���h�̉摜
	/*
	mNoteHoldSprite = CCArray::create();
	mNoteHoldSprite->retain();
	for(int i=0;i<mNoteHoldSpriteNumber;i++){
		NoteHoldSprite* nhSprite = new NoteHoldSprite(mEffectResourceManager);
		mNoteHoldSprite->addObject(nhSprite);
		nhSprite->setPlayer(layer);
	}
	*/

	//�]���摜
	mValueEffect = new ValueEffect();
	mValueEffect->setPlayer(layer);

	//�L���[�z��
	mPhraseQueue = CCArray::create();
	mPhraseQueue->retain();
	for(int i=0;i<mQueueSize;i++){
		OnePhraseScore* phrase;
		if(i<mCurrentNumberOfQueue){
			phrase= new OnePhraseScore(ScoreData::NOTHING,mStep,mOnePhraseBarNumber);
		}else{
			phrase= new OnePhraseScore(mScoreData[i-mCurrentNumberOfQueue],mStep,mOnePhraseBarNumber);
			setNoteData(phrase);
		}
		mPhraseQueue->addObject(phrase);
	}
	//�I�v�V�����f�[�^��ǂݍ���
	CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
	mNaviUsed = userDefault->getBoolForKey("naviUsed",true);
	if(mNaviUsed){
		mNavi = new Navigator(layer);
	}

}

bool ScoreManager::update(int gameTime,float delta){
	bool isMissed = false;
	//���݂̃t���[�Y���v�Z
	int currentPhrase = gameTime / mOnePhraseFrame;
	int currentFrame = gameTime % mOnePhraseFrame;

	//�t���[�Y���ύX���ꂽ��
	if(mCurrentPhraseNumber != currentPhrase){
		mCurrentPhraseNumber = currentPhrase;
		//�z�񂪂Ȃ��Ȃ��Ă��Ȃ����`�F�b�N
		if(mPhraseQueue->count() > 0){
			//�z��̐擪���폜
			mPhraseQueue->removeObjectAtIndex(0);
		}
		//�I���łȂ���Ύ��̃t���[�Y���쐬
		if(mCurrentPhraseNumber < mScoreMaxNumber){
			OnePhraseScore* phrase = new OnePhraseScore(mScoreData[mCurrentPhraseNumber + mQueueSize - mCurrentNumberOfQueue - 1],mStep,mOnePhraseBarNumber);
			mPhraseQueue->addObject(phrase);
			setNoteData(phrase);
		}
	}
	//�t���[����i�߂�
	for(int i=0;i<mPhraseQueue->count();i++){
		OnePhraseScore* phrase = dynamic_cast<OnePhraseScore*>( mPhraseQueue->objectAtIndex(i) );
		int deltaFrame = (i-mCurrentNumberOfQueue) * mOnePhraseFrame;
		if(updateFrame(phrase,currentFrame-deltaFrame,delta)){
			isMissed = true;
		}
	}
	//�i�r�X�V
	if(mNaviUsed){
		setNaviNote();
		mNavi->update();
	}

	return isMissed;
}

//�t���[�Y���ƂɃt���[����i�߂�
bool ScoreManager::updateFrame(OnePhraseScore* phrase,int frame,float delta){
	bool isMissed = false;
	bool flag;
	NoteTap* noteTap;
	NoteHold* noteHold;
	for(int i=0; i<phrase->mNoteArray->count();i++){
		Note* note = dynamic_cast<Note*>( phrase->mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		switch (type){
			case Note::TAP:
				noteTap = dynamic_cast<NoteTap*>(note);
				flag = noteTap->update(frame,delta);
				if(flag){
					//�]���摜��o�^����
					noteTap->setValuePlayer( mValueEffect->getValuePlayer(Note::MISS) );
					isMissed = true;
					gGradeData.missNum++;
				}

				break;
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				flag = noteHold->update(frame,delta);
				if(flag){
					//�]���摜��o�^����
					//noteHold->setValuePlayer( mValueEffect->getValuePlayer(Note::MISS), noteHold->isStartMissed());
					isMissed = true;
					gGradeData.missNum++;
				}

				break;
		}
	}
	return isMissed;
}

//�^�b�v�𔻒肷�� BAD���ł��Ƃ���true��Ԃ�
bool ScoreManager::checkTap(int touch_x,int touch_y,GradeManager* gradeMnager){
	bool isBad = false;
	NoteTap* noteTap;
	NoteHold* noteHold;
	//���݂̊y�����擾	������Ƃ錻�݂̔z���1��
	OnePhraseScore* phrase = dynamic_cast<OnePhraseScore*>( mPhraseQueue->objectAtIndex(mCurrentNumberOfQueue) );
	for(int i=0; i<phrase->mNoteArray->count();i++){
		Note* note = dynamic_cast<Note*>( phrase->mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		Note::Value value = Note::VALUE_NUM;
		switch (type){
			case Note::TAP:
				noteTap = dynamic_cast<NoteTap*>(note);
				value = noteTap->checkTap(touch_x,touch_y);
				if(value != Note::VALUE_NUM){
					//�]���摜��o�^����
					noteTap->setValuePlayer( mValueEffect->getValuePlayer(value) );
				}
				break;
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				value = noteHold->checkHit(touch_x,touch_y,true);
				if(value != Note::VALUE_NUM){
					//�]���摜��o�^����
					//noteHold->setValuePlayer( mValueEffect->getValuePlayer(value),true );

				}

				break;
		}
		//�o�b�h����
		if(value == Note::BAD){
			isBad = true;
		}
		//���_���Z
		gradeMnager->addCacheGrade(value);
	}
	return isBad;
}

//�^�b�v�𔻒肷�� BAD���ł��Ƃ���true��Ԃ�
bool ScoreManager::checkShake(GradeManager* gradeMnager){
	bool isBad = false;
	NoteTap* noteTap;
	NoteHold* noteHold;
	//���݂̊y�����擾	������Ƃ錻�݂̔z���1��
	OnePhraseScore* phrase = dynamic_cast<OnePhraseScore*>( mPhraseQueue->objectAtIndex(mCurrentNumberOfQueue) );
	for(int i=0; i<phrase->mNoteArray->count();i++){
		Note* note = dynamic_cast<Note*>( phrase->mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		Note::Value value = Note::VALUE_NUM;
		switch (type){
			case Note::TAP:
				noteTap = dynamic_cast<NoteTap*>(note);
				value = noteTap->checkShake();
				if(value != Note::VALUE_NUM){
					//�]���摜��o�^����
					noteTap->setValuePlayer( mValueEffect->getValuePlayer(value) );
				}
				break;
			//case Note::HOLD:
			//	noteHold = dynamic_cast<NoteHold*>(note);
			//	value = noteHold->checkHit(touch_x,touch_y,true);
			//	if(value != Note::VALUE_NUM){
			//		//�]���摜��o�^����
			//		noteHold->setValuePlayer( mValueEffect->getValuePlayer(value),true );
			//	}
			//	break;
		}
		//�o�b�h����
		if(value == Note::BAD){
			isBad = true;
		}
		//���_���Z
		gradeMnager->addCacheGrade(value);
	}
	return isBad;
}

//�����[�X�𔻒肷�� BAD���ł��Ƃ���true��Ԃ�
bool ScoreManager::checkRelease(int touch_x,int touch_y,GradeManager* gradeMnager){
	bool isBad = false;
	NoteHold* noteHold;
	//���݂̊y�����擾	������Ƃ錻�݂̔z���1��
	OnePhraseScore* phrase = dynamic_cast<OnePhraseScore*>( mPhraseQueue->objectAtIndex(mCurrentNumberOfQueue) );
	for(int i=0; i<phrase->mNoteArray->count();i++){
		Note* note = dynamic_cast<Note*>( phrase->mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		//�z�[���h�łȂ���Ύ���
		if(type != Note::HOLD){
			continue;
		}
		Note::Value value = Note::VALUE_NUM;
		switch (type){
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				value = noteHold->checkHit(touch_x,touch_y,false);
				if(value != Note::VALUE_NUM){
					//�]���摜��o�^����
					//noteHold->setValuePlayer( mValueEffect->getValuePlayer(value),false );

				}
				break;
		}
		//�o�b�h����
		if(value == Note::BAD){
			isBad = true;
		}
		//���_���Z
		gradeMnager->addCacheGrade(value);
	}
	return isBad;
}


//�z�[���h�𔻒肷�� �z�[���h�Ɠ������Ă���Ƃ���true��Ԃ�
bool ScoreManager::checkHold(int touch_x,int touch_y,GradeManager* gradeMnager){
	bool isHit = false;
	NoteHold* noteHold;
	//���݂̊y�����擾	������Ƃ錻�݂̔z���1��
	OnePhraseScore* phrase = dynamic_cast<OnePhraseScore*>( mPhraseQueue->objectAtIndex(mCurrentNumberOfQueue) );
	for(int i=0; i<phrase->mNoteArray->count();i++){
		Note* note = dynamic_cast<Note*>( phrase->mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		//�z�[���h�łȂ���Ύ���
		if(type != Note::HOLD){
			continue;
		}
		switch (type){
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				isHit = noteHold->checkHold(touch_x,touch_y);
				if(isHit){
					//���_���Z
					gradeMnager->addCacheGradeByHold();
				}
				break;
		}
	}
	return isHit;
}

//�m�[�g�̏����摜�ɓo�^����
void ScoreManager::setNoteData(OnePhraseScore* phrase){
	for(int i=0; i<phrase->mNoteArray->count();i++){
		Note* note = dynamic_cast<Note*>( phrase->mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		NoteTap* noteTap;
		NoteHold* noteHold;
		switch (type){
			case Note::TAP:
				noteTap = dynamic_cast<NoteTap*>(note);
				//�A�N�e�B�u�łȂ����̂���������
				for(int j=0; j<mNoteTapSprite->count();j++){
					NoteTapSprite* ntSprite = dynamic_cast<NoteTapSprite*>( mNoteTapSprite->objectAtIndex(j) ); 
					//�A�N�e�B�u�łȂ���Γo�^�����[�v�𔲂���
					if(!ntSprite->isActive()){
						noteTap->setNoteTapSprite(ntSprite);
						break;
					}
				}
				break;
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				//�A�N�e�B�u�łȂ����̂���������
				for(int j=0; j<mNoteHoldSprite->count();j++){
					NoteHoldSprite* nhSprite = dynamic_cast<NoteHoldSprite*>( mNoteHoldSprite->objectAtIndex(j) ); 
					//�A�N�e�B�u�łȂ���Γo�^�����[�v�𔲂���
					if(!nhSprite->isActive()){
						noteHold->setNoteHoldSprite(nhSprite);
						break;
					}
				}
				break;
		}
	}
}

//�i�r�̍X�V
void ScoreManager::setNaviNote(){
	int count = mNavi->getNoteArrayCount();
	if(count >= 2){
		return;
	}

	//�Q�ɂȂ�܂Ńm�[�g���擾����
	for(int i=0;i<mPhraseQueue->count();i++){
		OnePhraseScore* phrase = dynamic_cast<OnePhraseScore*>( mPhraseQueue->objectAtIndex(i) );
		for(int i=0; i<phrase->mNoteArray->count();i++){
			Note* note = dynamic_cast<Note*>( phrase->mNoteArray->objectAtIndex(i) );
			if(!note->isNaviStarted()){
				note->naviStart();
				mNavi->setNoteData(note);
				if(count++ >= 2){
					return;
				}
			}
		}
	}
}

void ScoreManager::pauseAnimation(){
	for(int i=0;i<mNoteTapSpriteNumber;i++){
		NoteTapSprite* noteTapSprite = dynamic_cast<NoteTapSprite*>( mNoteTapSprite->objectAtIndex(i) );
		noteTapSprite->pauseAnimation();
	}
	mValueEffect->pause();
}

void ScoreManager::resumeAnimation(){
	for(int i=0;i<mNoteTapSpriteNumber;i++){
		NoteTapSprite* noteTapSprite = dynamic_cast<NoteTapSprite*>( mNoteTapSprite->objectAtIndex(i) );
		noteTapSprite->resumeAnimation();
	}
	mValueEffect->resume();
}