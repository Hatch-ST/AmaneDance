#ifndef __NOTE__HOLD__SPRITE__H__
#define __NOTE__HOLD__SPRITE__H__

#include "cocos2d.h" 
#include "Note.h" 
#include "SSPlayer.h"
using namespace cocos2d;


class NoteHoldSprite:public CCObject{
	public:
		NoteHoldSprite(SSImageList* effectImage);
		~NoteHoldSprite();
		void init();

		//�G�t�F�N�g�ޗp�C���[�W���X�g
		SSImageList* mEffectImage;

		//�m�[�g�v���C���[
		SSPlayer* mNotePlayer;

		//�J�n���T�[�N���v���C���[
		SSPlayer* mCirclePlayerStart;

		//�I�����T�[�N���v���C���[
		SSPlayer* mCirclePlayerEnd;

		//�m�[�g�G�t�F�N�g
		SSPlayer* mNoteEffectPlayer;

		bool update(int currentFrame,float delta);

		//�摜���g���Ă邩
		bool isActive();

		void setPlayer(CCLayer *layer);

		void setNoteData(int startPos_x,int startPos_y,int endPos_x,int endPos_y,int startFrame,int endFrame);

		void setPressed(bool pressed);
		void setReleased(bool released);
	private:
		bool mActive;

		//�z�[���h�J�n�t���[��
		int mStartFrame;

		//�z�[���h�I���t���[��
		int mEndFrame;

		//�J�n�t���[���Ƃ̂���
		int mDeltaFromStart;

		//�I���t���[���Ƃ̂���
		int mDeltaFromEnd;

		//�J�n�T�[�N���p�t���O
		bool mCircleStartFirstFlag;
		//�I���T�[�N���p�t���O
		bool mCircleEndFirstFlag;
		//�m�[�g�p�t���O
		bool mNoteFirstFlag;
		
		//�����ꂽ��
		bool mPressed;
		//�����ꂽ��
		bool mReleased;
};


#endif //__NOTE__HOLD__SPRITE__H__