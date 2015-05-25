#ifndef __NOTE__TAP__SPRITE__H__
#define __NOTE__TAP__SPRITE__H__

#include "cocos2d.h" 
#include "Note.h" 
#include "SS5Player.h"
using namespace cocos2d;


class NoteTapSprite:public CCObject{
	public:
		NoteTapSprite();
		~NoteTapSprite();
		void init();

		//�m�[�g�v���C���[
		ss::Player* mNotePlayer;

		//�T�[�N���v���C���[
		ss::Player* mCirclePlayer;

		//�m�[�g�G�t�F�N�g
		ss::Player* mNoteEffectPlayer;

		bool update(int currentFrame,float delta);

		//�摜���g���Ă邩
		bool isActive();

		void setPlayer(CCLayer *layer);

		void setNoteData(int notePos_x,int notePos_y,int correctFrame);

		void setPressed(bool pressed);

		void pauseAnimation();
		void resumeAnimation();
	private:
		bool mActive;

		//�����̃t���[��
		int mCorrectFrame;

		//�����̃t���[���Ƃ̂���
		int mDeltaFromCorrect;

		//�ŏ��̈��p�t���O
		bool mFirstFlag;
		
		//�����ꂽ��
		bool mPressed;
};


#endif //__NOTE__TAP__SPRITE__H__