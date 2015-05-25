#ifndef __ONE__PHRASE__SCORE__H__
#define __ONE__PHRASE__SCORE__H__

#include "cocos2d.h"
#include "ScoreData.h"
#include "Note.h"
#include "NoteTap.h"
#include "NoteHold.h"

//�P�t���[�Y�̊y�������N���X
class OnePhraseScore :public cocos2d::CCObject{
	public:
		OnePhraseScore(ScoreData::NoteType noteType,float step,int onePhraseBarNumber);
		~OnePhraseScore();
		cocos2d::CCArray* mNoteArray;
	private:
		//�m�[�g�̍��W���擾����
		cocos2d::CCPoint getNoteCood(int point_x,int point_y);
};

#endif //__ONE__PHRASE__SCORE__H__