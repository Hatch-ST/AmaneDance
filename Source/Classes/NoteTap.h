#ifndef __NOTE__TAP__H__
#define __NOTE__TAP__H__

#include "cocos2d.h" 
#include "Note.h"
#include "NoteTapSprite.h"
#include "SS5Player.h"
using namespace cocos2d;


class NoteTap:public Note{
	typedef Note base;
	public:
		NoteTap();
		NoteTap(CCPoint cood,int correctFrame);
		~NoteTap();
		void init();
     
		//�������W
		int mPos_x;
		int mPos_y;

		//�����̃t���[��
		int mCorrectFrame;

		//�����̃t���[���Ƃ̂���
		int mDeltaFromCorrect;

		//�^�b�v���`�F�b�N���邩�̃t���O
		bool mCheckFlag;

		bool update(int currentFrame,float delta);

		//�����^�C�v���擾����
		base::NoteType getNoteType();

		//�i�r���J�n����
		void naviStart();

		//�i�r���J�n������
		bool isNaviStarted();

		//�^�b�v�𔻒肷��
		base::Value checkTap(int touch_x,int touch_y);

		//�V�F�C�N�𔻒肷��
		base::Value checkShake();

		//�m�[�g�摜��o�^����
		void setNoteTapSprite(NoteTapSprite* ntSprite);

		//�]���摜��o�^����
		void setValuePlayer(ss::Player* valuePlayer);

		//�i�r��\�����邩�̃t���O
		bool mNaviFlag;

	private:
		//����p�~�̔��a
		static const int HitRadiusTap = 48;

		//�t���[���̂���ɂ���ĕ]�����擾����
		Note::Value getValue();

		//�o�^�����摜�̃|�C���^����������
		NoteTapSprite* mNoteTapSprite;
		//�m�[�g�̃A�j���[�V�������I��������
		bool mNoteAnimationEnded;

		//����J�n������
		bool mCheckStarted;


		//�]���摜
		ss::Player* mValuePlayer;
		//�]���摜���A�N�e�B�u��
		bool mValueActive;

};


#endif //__NOTE__TAP__H__