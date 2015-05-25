#include "Navigator.h"
#include "GameScene.h"

Navigator::Navigator(CCLayer* layer){
	init(layer);
}

Navigator::~Navigator(){
	mNoteArray->release();
}

void Navigator::init(CCLayer* layer){
	mNaviSprite = CCSprite::create("navigator.png");
	mNaviSprite->setVisible(false);

	mNaviSpriteDouble = CCSprite::create("navigator.png");
	mNaviSpriteDouble->setVisible(false);

	mNoteArray = CCArray::create();
	mNoteArray->retain();

	layer->addChild(mNaviSprite,NavigatorPriority);
	layer->addChild(mNaviSpriteDouble,NavigatorPriority);

	mNoteChanged = false;
	mDouble = false;
	mIsHoldFirst = false;
	mIsHoldSecond = false;
}

//ナビの座標を登録する
void Navigator::setPosition(int x,int y, bool isDouble){
	if(!isDouble){
		mNaviSprite->setPositionX(x+50);
		mNaviSprite->setPositionY(y-50);
		mNaviSprite->setVisible(true);
	}else{
		mNaviSpriteDouble->setPositionX(x+50);
		mNaviSpriteDouble->setPositionY(y-50);
		mNaviSpriteDouble->setVisible(true);
	}
}

//ノート情報を登録する
void Navigator::setNoteData(Note* note){
	mNoteArray->addObject(note);
	mNoteChanged = true;
}

//ノートが今いくつあるか取得する
int Navigator::getNoteArrayCount(){
	return mNoteArray->count();
}

//更新
void Navigator::update(){
	if(getNoteArrayCount() == 0){
		return;
	}

	if(mNoteChanged){		
		if(checkNaviFlag()){
			mDouble = checkDouble();
			setDraw(mDouble);
		}
	}else{
		if(!checkNaviFlag()){
			mNoteArray->removeObjectAtIndex(0);
			mNaviSprite->setVisible(false);
			mIsHoldFirst = false;
			//同時押しの場合2つ目も削除
			if(mDouble){
				mNoteArray->removeObjectAtIndex(0);
				mNaviSpriteDouble->setVisible(false);
				mIsHoldSecond = false;
			}
			mNoteChanged = true;
		}
	}
	if(checkNaviFlag()){
		//ホールドの更新
		if(mIsHoldFirst){
			updateHoldNavi(false);
		}
		if(mIsHoldSecond){
			updateHoldNavi(true);
		}
	}
}

//ホールド時の更新
void Navigator::updateHoldNavi(bool isDouble){
	int count = 0;
	if(isDouble){
		count = 1;
	}
	Note* note = dynamic_cast<Note*>( mNoteArray->objectAtIndex(count) );
	NoteHold* noteHold = dynamic_cast<NoteHold*>(note);
	CCPoint naviCood = noteHold->getNaviCood();	
	setPosition((int)naviCood.x,(int)naviCood.y,isDouble);
}

//ノートからナビフラグが立っているか調べる
bool Navigator::checkNaviFlag(){
	for(int i=0;i<getNoteArrayCount();i++){
		Note* note = dynamic_cast<Note*>( mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		NoteTap* noteTap;
		NoteHold* noteHold;
		switch (type){
			case Note::TAP:
				noteTap = dynamic_cast<NoteTap*>(note);
				return noteTap->mNaviFlag;
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				return noteHold->mNaviFlag;
		}
	}
	return false;
}

//ナビを描画する
void Navigator::setDraw( bool isDouble){
	mNoteChanged = false;
	int count = 1;
	if(isDouble){
		count = 2;
	}
	for(int i=0;i<count;i++){
		Note* note = dynamic_cast<Note*>( mNoteArray->objectAtIndex(i) );
		Note::NoteType type = note->getNoteType();
		NoteTap* noteTap;
		NoteHold* noteHold;
		switch (type){
			case Note::TAP:
				noteTap = dynamic_cast<NoteTap*>(note);
				if(i == 0){
					setPosition(noteTap->mPos_x,noteTap->mPos_y,false);
				}else{
					setPosition(noteTap->mPos_x,noteTap->mPos_y,true);
				}
				break;
			case Note::HOLD:
				noteHold = dynamic_cast<NoteHold*>(note);
				if(i == 0){
					setPosition(noteHold->mCurrentPos_x,noteHold->mCurrentPos_y,false);
					mIsHoldFirst = true;
				}else{
					setPosition(noteHold->mCurrentPos_x,noteHold->mCurrentPos_y,true);
					mIsHoldSecond = true;
				}
				break;
		}
	}
	
}

//同時押しか調べる
bool Navigator::checkDouble(){
	bool isDouble = false;
	if(getNoteArrayCount() < 2){
		return isDouble;
	}

	Note* note1 = dynamic_cast<Note*>( mNoteArray->objectAtIndex(0) );
	Note* note2 = dynamic_cast<Note*>( mNoteArray->objectAtIndex(1) );
	Note::NoteType type1 = note1->getNoteType();
	Note::NoteType type2 = note2->getNoteType();
	NoteTap* noteTap1; NoteTap* noteTap2;
	NoteHold* noteHold1; NoteHold* noteHold2;
	//同じノートタイプの場合
	if(type1 == type2){
		switch (type1){
			case Note::TAP:
				noteTap1 = dynamic_cast<NoteTap*>(note1);
				noteTap2 = dynamic_cast<NoteTap*>(note2);
				//正解フレームが同じか調べる
				if(noteTap1->mCorrectFrame == noteTap2->mCorrectFrame){
					isDouble = true;
				}
				break;
			case Note::HOLD:
				noteHold1 = dynamic_cast<NoteHold*>(note1);
				noteHold2 = dynamic_cast<NoteHold*>(note2);
				//正解フレームが同じか調べる
				if(noteHold1->mStartFrame == noteHold2->mStartFrame){
					isDouble = true;
				}
				break;
		}
	}else{
	}
	
	return isDouble;
}
