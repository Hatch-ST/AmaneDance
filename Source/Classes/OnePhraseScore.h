#ifndef __ONE__PHRASE__SCORE__H__
#define __ONE__PHRASE__SCORE__H__

#include "cocos2d.h"
#include "ScoreData.h"
#include "Note.h"
#include "NoteTap.h"
#include "NoteHold.h"

//１フレーズの楽譜を作るクラス
class OnePhraseScore :public cocos2d::CCObject{
	public:
		OnePhraseScore(ScoreData::NoteType noteType,float step,int onePhraseBarNumber);
		~OnePhraseScore();
		cocos2d::CCArray* mNoteArray;
	private:
		//ノートの座標を取得する
		cocos2d::CCPoint getNoteCood(int point_x,int point_y);
};

#endif //__ONE__PHRASE__SCORE__H__