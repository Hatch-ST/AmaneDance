#ifndef __NOTE__H__
#define __NOTE__H__

#include "cocos2d.h" 
using namespace cocos2d;

//何フレーム前から音符を表示するか
static const int BeforeActiveFrame = 150;
//何フレーム後に音符を非表示にするか
static const int AfterActiveFrame = 60;
//評価の猶予フレーム
static const int CoolFrame = 5;
static const int GreatFrame = 8;
static const int GoodFrame = 11;
static const int BadFrame = 20;

class Note:public CCObject{
	public:
		//音符の種類
		enum NoteType{
			TAP,
			HOLD
		};
		//判定の評価
		enum Value{
			COOL,
			GREAT,
			GOOD,
			BAD,
			MISS,
			VALUE_NUM
		};
		virtual NoteType getNoteType();
		virtual bool isNaviStarted();
		virtual void naviStart();
	protected:
		NoteType mNoteType;
		bool mNaviStarted;
};


#endif //__NOTE__H__