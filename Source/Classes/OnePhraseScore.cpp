#include "OnePhraseScore.h"

//コンストラクタ
OnePhraseScore::OnePhraseScore(ScoreData::NoteType noteType,float step,int onePhraseBarNumber){
	Note* note;
	mNoteArray = CCArray::create();
	mNoteArray->retain();
	switch(noteType){
		case ScoreData::DANCE01_A:
			note = new NoteTap(getNoteCood(0,2), 31 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,2), 91 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::DANCE02_A:
			note = new NoteTap(getNoteCood(0,2), 31 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,3), 91 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::DANCE03_A:
			note = new NoteTap(getNoteCood(2,4), 33 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,3), 63 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,2), 93 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::DANCE04_A:
			note = new NoteTap(getNoteCood(2,4), 45 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,4), 105 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::DANCE05_A:
			note = new NoteTap(getNoteCood(0,2), 31 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,3), 91 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::POSE_01_A:
			note = new NoteTap(getNoteCood(2,3), 53 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,2), 53 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::POSE_02_A:
			note = new NoteTap(getNoteCood(0,2), 56 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,2), 56 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::STEP_01_A:
			note = new NoteTap(getNoteCood(2,2), 31 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,2), 61 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,3), 91 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::STEP_02_A:
			note = new NoteTap(getNoteCood(2,3), 31 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,3), 91 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::WALK_01_A:
			note = new NoteTap(getNoteCood(0,1), 60 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,2), 120 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::WALK_02_A:
			note = new NoteTap(getNoteCood(0,2), 30 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,2), 60 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,3), 120 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::TAP_TWIN:
			note = new NoteTap(getNoteCood(0,2), 10 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,2), 30 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::TAP_TRIPLE:
			note = new NoteTap(getNoteCood(0,4), 10 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(1,4), 30 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,4), 50 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::TAP_DOUBLE:
			note = new NoteTap(getNoteCood(0,2), 30 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,2), 30 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
		case ScoreData::TAP_QUARTET:
			note = new NoteTap(getNoteCood(0,2), 15 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,2), 30 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,2), 45 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,2), 60 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,2), 75 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,2), 90 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,2), 105 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(0,2), 120 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::HOLD_SINGLE:
			note = new NoteHold(getNoteCood(0,1),getNoteCood(2,1), 10 * onePhraseBarNumber * step, 110 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::HOLD_DOUBLE:
			note = new NoteHold(getNoteCood(0,1),getNoteCood(0,3), 10 * onePhraseBarNumber * step, 110 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteHold(getNoteCood(2,1),getNoteCood(2,3), 10 * onePhraseBarNumber * step, 110 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
		case ScoreData::HOLD_AND_TAP:
			note = new NoteHold(getNoteCood(0,1),getNoteCood(0,3), 10 * onePhraseBarNumber * step, 110 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,1),10 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			note = new NoteTap(getNoteCood(2,3),110 * onePhraseBarNumber * step);
			mNoteArray->addObject(note);
			break;
	}	
}

//デストラクタ
OnePhraseScore::~OnePhraseScore(){
	//for(int i=0;i<mNoteArray->count();i++){
	//	delete mNoteArray->objectAtIndex(i);
	//}
	//mNoteArray->release();
}

cocos2d::CCPoint OnePhraseScore::getNoteCood(int point_x,int point_y){
	//画面サイズ取得
	CCPoint screenSize = CCDirector::sharedDirector()->getWinSize();
	//ノートの幅
	int space_x = 200,space_y = 200;
	//中心位置のpoint
	int center_x = 1,center_y = 2;
	CCPoint cood;
	cood.x = screenSize.x / 2 + (point_x-center_x) * space_x;
	cood.y = screenSize.y / 2 + (point_y-center_y) * space_y;
	return cood; 
}