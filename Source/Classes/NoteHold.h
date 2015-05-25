#ifndef __NOTE__HOLD__H__
#define __NOTE__HOLD__H__

#include "cocos2d.h" 
#include "Note.h"
#include "NoteHoldSprite.h"
#include "SSPlayer.h"
using namespace cocos2d;


class NoteHold:public Note{
	typedef Note base;
	public:
		NoteHold();
		NoteHold(CCPoint startCood,int startFrame,int endFrame);
		NoteHold(CCPoint startCood,CCPoint endCood,int startFrame,int endFrame);
		~NoteHold();
		void init();
     
		//�z�[���h�J�n���W
		int mStartPos_x;
		int mStartPos_y;

		//�z�[���h�J�n�t���[��
		int mStartFrame;

		//�z�[���h�I�����W
		int mEndPos_x;
		int mEndPos_y;

		//�z�[���h�I���t���[��
		int mEndFrame;

		//���݂̃m�[�g�̍��W
		int mCurrentPos_x;
		int mCurrentPos_y;

		//�J�n�t���[���Ƃ̂���
		int mDeltaFromStart;

		//�I���t���[���Ƃ̂���
		int mDeltaFromEnd;

		//�^�b�v���`�F�b�N���邩�̃t���O
		bool mCheckTapFlag;
		//�z�[���h���`�F�b�N���邩�̃t���O
		bool mCheckHoldFlag;
		//�����[�X���`�F�b�N���邩�̃t���O
		bool mCheckReleaseFlag;

		bool update(int currentFrame,float delta);

		//�����^�C�v���擾����
		base::NoteType getNoteType();

		//�i�r���J�n����
		void naviStart();

		//�i�r���J�n������
		bool isNaviStarted();

		//�^�b�v�ƃ����[�X�𔻒肷��
		base::Value checkHit(int touch_x,int touch_y,bool isStart);
		//�z�[���h�𔻒肷��
		bool checkHold(int touch_x,int touch_y);

		//�m�[�g�摜��o�^����
		void setNoteHoldSprite(NoteHoldSprite* nhSprite);

		//�]���摜��o�^����
		void setValuePlayer(SSPlayer* valuePlayer,bool isStart);

		//�i�r��\�����邩�̃t���O
		bool mNaviFlag;

		//�i�r�p���W���擾����
		CCPoint getNaviCood();

		//�J�n���̃~�X�����ׂ�
		bool isStartMissed();

	private:
		//����p�~�̔��a
		static const int HitRadiusHold = 72;

		//�t���[���̂���ɂ���ĕ]�����擾����
		Note::Value getValue(bool isStart);

		//�o�^�����摜�̃|�C���^����������
		NoteHoldSprite* mNoteHoldSprite;
		//�m�[�g�̃A�j���[�V�������I��������
		bool mNoteAnimationEnded;

		//����J�n������
		bool mCheckTapStarted;
		bool mCheckReleaseStarted;

		//�J�n���̕]���摜
		SSPlayer* mValuePlayerStart;
		//�J�n�]���摜���A�N�e�B�u��
		bool mValueActiveStart;
		//�I�����̕]���摜
		SSPlayer* mValuePlayerEnd;
		//�]���摜���A�N�e�B�u��
		bool mValueActiveEnd;

		//���݂̃m�[�g�̍��W���擾����
		void getCurrentCood(int* current_x,int* current_y, int currentFrame);
};


#endif //__NOTE__HOLD__H__