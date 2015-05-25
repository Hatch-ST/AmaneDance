#ifndef __NOTE__H__
#define __NOTE__H__

#include "cocos2d.h" 
using namespace cocos2d;

//���t���[���O���特����\�����邩
static const int BeforeActiveFrame = 150;
//���t���[����ɉ������\���ɂ��邩
static const int AfterActiveFrame = 60;
//�]���̗P�\�t���[��
static const int CoolFrame = 5;
static const int GreatFrame = 8;
static const int GoodFrame = 11;
static const int BadFrame = 20;

class Note:public CCObject{
	public:
		//�����̎��
		enum NoteType{
			TAP,
			HOLD
		};
		//����̕]��
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