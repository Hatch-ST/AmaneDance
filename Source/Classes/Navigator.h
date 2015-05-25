#ifndef __NAVIGATOR__H__
#define __NAVIGATOR__H__

#include "cocos2d.h" 
#include "Note.h"
#include "NoteTap.h"
#include "NoteTapSprite.h"
#include "NoteHold.h"
#include "NoteHoldSprite.h"

using namespace cocos2d;

class Navigator{
	public:
		Navigator(CCLayer* layer);
		~Navigator();
		void init(CCLayer* layer);

		//�i�r�摜
		CCSprite* mNaviSprite;
		//�i�r�摜�i���������̂Q�ځj
		CCSprite* mNaviSpriteDouble;

		void setPosition(int x,int y, bool isDouble);

		void setNoteData(Note* note);

		int getNoteArrayCount();

		void update();
	private:
		//���̂Q�̃m�[�g��ێ����Ă���
		CCArray* mNoteArray;

		//����������
		bool mDouble;
		
		//�m�[�g���X�V���ꂽ��
		bool mNoteChanged;

		//�i�r�̕`��t���O�����������`�F�b�N����
		bool checkNaviFlag();

		//�i�r�摜��\������
		void setDraw( bool isDouble);

		//�������������ׂ�
		bool checkDouble();

		//1�ڂ��z�[���h�ł��邩
		bool mIsHoldFirst;
		//2�ڂ��z�[���h�ł��邩
		bool mIsHoldSecond;

		//�z�[���h���̍X�V
		void updateHoldNavi(bool isDouble);

};


#endif //__NAVIGATOR__H__