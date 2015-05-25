#include "ScoreManager.h"



#ifdef G_GRADE_DATA
struct GradeData gGradeData;
#else
extern struct GradeData gGradeData;
#endif

//コストラクタ
ScoreManager::ScoreManager(MusicName musicName,cocos2d::CCLayer *layer){
	init(musicName,layer);
}

ScoreManager::~ScoreManager(){
	mPhraseQueue->release();
}

//初期化
void ScoreManager::init(MusicName musicName,cocos2d::CCLayer *layer){
	//曲名から情報取得
	m_bpm = MusicData::getMusicBPM(musicName);
	mRhythm = MusicData::getMusicRhythm(musicName);
	mOnePhraseBarNumber = 1;
	//1フレーズの基準の長さ
	mBaseFrame = 120 * mOnePhraseBarNumber;

	//１フレーズの長さを計算
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

	//ノートタップの画像
	mNoteTapSprite = CCArray::create();
	mNoteTapSprite->retain();
	for(int i=0;i<mNoteTapSpriteNumber;i++){
		NoteTapSprite* ntSprite = new NoteTapSprite();
		mNoteTapSprite->addObject(ntSprite);
		ntSprite->setPlayer(layer);
	}
	//ノートホールドの画像
	/*
	mNoteHoldSprite = CCArray::create();
	mNoteHoldSprite->retain();
	for(int i=0;i<mNoteHoldSpriteNumber;i++){
		NoteHoldSprite* nhSprite = new NoteHoldSprite(mEffectResourceManager);
		mNoteHoldSprite->addObject(nhSprite);
		nhSprite->setPlayer(layer);
	}
	*/

	//評価画像
	mValueEffect = new ValueEffect();
	mValueEffect->setPlayer(layer);

	//キュー配列
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
	//オプションデータを読み込む
	CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
	mNaviUsed = userDefault->getBoolForKey("naviUsed",true);
	if(mNaviUsed){
		mNavi = new Navigator(layer);
	}

}

bool ScoreManager::update(int gameTime,float delta){
	bool isMissed = false;
	//現在のフレーズを計算
	int currentPhrase = gameTime / mOnePhraseFrame;
	int currentFrame = gameTime % mOnePhraseFrame;

	//フレーズが変更されたか
	if(mCurrentPhraseNumber != currentPhrase){
		mCurrentPhraseNumber = currentPhrase;
		//配列がなくなっていないかチェック
		if(mPhraseQueue->count() > 0){
			//配列の先頭を削除
			mPhraseQueue->removeObjectAtIndex(0);
		}
		//終了でなければ次のフレーズを作成
		if(mCurrentPhraseNumber < mScoreMaxNumber){
			OnePhraseScore* phrase = new OnePhraseScore(mScoreData[mCurrentPhraseNumber + mQueueSize - mCurrentNumberOfQueue - 1],mStep,mOnePhraseBarNumber);
			mPhraseQueue->addObject(phrase);
			setNoteData(phrase);
		}
	}
	//フレームを進める
	for(int i=0;i<mPhraseQueue->count();i++){
		OnePhraseScore* phrase = dynamic_cast<OnePhraseScore*>( mPhraseQueue->objectAtIndex(i) );
		int deltaFrame = (i-mCurrentNumberOfQueue) * mOnePhraseFrame;
		if(updateFrame(phrase,currentFrame-deltaFrame,delta)){
			isMissed = true;
		}
	}
	//ナビ更新
	if(mNaviUsed){
		setNaviNote();
		mNavi->update();
	}

	return isMissed;
}

//フレーズごとにフレームを進める
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
					//評価画像を登録する
					noteTap->setValuePlayer( mValueEffect->getValuePlayer(Note::MISS) );
					isMissed = true;
					gGradeData.missNum++;
				}

				break;
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				flag = noteHold->update(frame,delta);
				if(flag){
					//評価画像を登録する
					//noteHold->setValuePlayer( mValueEffect->getValuePlayer(Note::MISS), noteHold->isStartMissed());
					isMissed = true;
					gGradeData.missNum++;
				}

				break;
		}
	}
	return isMissed;
}

//タップを判定する BADがでたときはtrueを返す
bool ScoreManager::checkTap(int touch_x,int touch_y,GradeManager* gradeMnager){
	bool isBad = false;
	NoteTap* noteTap;
	NoteHold* noteHold;
	//現在の楽譜を取得	判定をとる現在の配列は1番
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
					//評価画像を登録する
					noteTap->setValuePlayer( mValueEffect->getValuePlayer(value) );
				}
				break;
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				value = noteHold->checkHit(touch_x,touch_y,true);
				if(value != Note::VALUE_NUM){
					//評価画像を登録する
					//noteHold->setValuePlayer( mValueEffect->getValuePlayer(value),true );

				}

				break;
		}
		//バッド判定
		if(value == Note::BAD){
			isBad = true;
		}
		//得点加算
		gradeMnager->addCacheGrade(value);
	}
	return isBad;
}

//タップを判定する BADがでたときはtrueを返す
bool ScoreManager::checkShake(GradeManager* gradeMnager){
	bool isBad = false;
	NoteTap* noteTap;
	NoteHold* noteHold;
	//現在の楽譜を取得	判定をとる現在の配列は1番
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
					//評価画像を登録する
					noteTap->setValuePlayer( mValueEffect->getValuePlayer(value) );
				}
				break;
			//case Note::HOLD:
			//	noteHold = dynamic_cast<NoteHold*>(note);
			//	value = noteHold->checkHit(touch_x,touch_y,true);
			//	if(value != Note::VALUE_NUM){
			//		//評価画像を登録する
			//		noteHold->setValuePlayer( mValueEffect->getValuePlayer(value),true );
			//	}
			//	break;
		}
		//バッド判定
		if(value == Note::BAD){
			isBad = true;
		}
		//得点加算
		gradeMnager->addCacheGrade(value);
	}
	return isBad;
}

//リリースを判定する BADがでたときはtrueを返す
bool ScoreManager::checkRelease(int touch_x,int touch_y,GradeManager* gradeMnager){
	bool isBad = false;
	NoteHold* noteHold;
	//現在の楽譜を取得	判定をとる現在の配列は1番
	OnePhraseScore* phrase = dynamic_cast<OnePhraseScore*>( mPhraseQueue->objectAtIndex(mCurrentNumberOfQueue) );
	for(int i=0; i<phrase->mNoteArray->count();i++){
		Note* note = dynamic_cast<Note*>( phrase->mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		//ホールドでなければ次へ
		if(type != Note::HOLD){
			continue;
		}
		Note::Value value = Note::VALUE_NUM;
		switch (type){
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				value = noteHold->checkHit(touch_x,touch_y,false);
				if(value != Note::VALUE_NUM){
					//評価画像を登録する
					//noteHold->setValuePlayer( mValueEffect->getValuePlayer(value),false );

				}
				break;
		}
		//バッド判定
		if(value == Note::BAD){
			isBad = true;
		}
		//得点加算
		gradeMnager->addCacheGrade(value);
	}
	return isBad;
}


//ホールドを判定する ホールドと当たっているときはtrueを返す
bool ScoreManager::checkHold(int touch_x,int touch_y,GradeManager* gradeMnager){
	bool isHit = false;
	NoteHold* noteHold;
	//現在の楽譜を取得	判定をとる現在の配列は1番
	OnePhraseScore* phrase = dynamic_cast<OnePhraseScore*>( mPhraseQueue->objectAtIndex(mCurrentNumberOfQueue) );
	for(int i=0; i<phrase->mNoteArray->count();i++){
		Note* note = dynamic_cast<Note*>( phrase->mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		//ホールドでなければ次へ
		if(type != Note::HOLD){
			continue;
		}
		switch (type){
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				isHit = noteHold->checkHold(touch_x,touch_y);
				if(isHit){
					//得点加算
					gradeMnager->addCacheGradeByHold();
				}
				break;
		}
	}
	return isHit;
}

//ノートの情報を画像に登録する
void ScoreManager::setNoteData(OnePhraseScore* phrase){
	for(int i=0; i<phrase->mNoteArray->count();i++){
		Note* note = dynamic_cast<Note*>( phrase->mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		NoteTap* noteTap;
		NoteHold* noteHold;
		switch (type){
			case Note::TAP:
				noteTap = dynamic_cast<NoteTap*>(note);
				//アクティブでないものを検索する
				for(int j=0; j<mNoteTapSprite->count();j++){
					NoteTapSprite* ntSprite = dynamic_cast<NoteTapSprite*>( mNoteTapSprite->objectAtIndex(j) ); 
					//アクティブでなければ登録しループを抜ける
					if(!ntSprite->isActive()){
						noteTap->setNoteTapSprite(ntSprite);
						break;
					}
				}
				break;
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				//アクティブでないものを検索する
				for(int j=0; j<mNoteHoldSprite->count();j++){
					NoteHoldSprite* nhSprite = dynamic_cast<NoteHoldSprite*>( mNoteHoldSprite->objectAtIndex(j) ); 
					//アクティブでなければ登録しループを抜ける
					if(!nhSprite->isActive()){
						noteHold->setNoteHoldSprite(nhSprite);
						break;
					}
				}
				break;
		}
	}
}

//ナビの更新
void ScoreManager::setNaviNote(){
	int count = mNavi->getNoteArrayCount();
	if(count >= 2){
		return;
	}

	//２つになるまでノートを取得する
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