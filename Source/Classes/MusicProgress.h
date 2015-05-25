#ifndef __MUSIC__PROGRESS__H__
#define __MUSIC__PROGRESS__H__

#include "cocos2d.h"
#include "MusicData.h"

using namespace cocos2d;

//�P�t���[�Y�̊y�������N���X
class MusicProgress{
	public:
		MusicProgress(MusicName musicName,CCLayer* layer);
		~MusicProgress();
		void init(CCLayer* layer);
		
		//�A�j���[�V�����J�n
		void startProgressAnim();
		//�A�j���[�V�����ꎞ��~
		void pauseProgressAnim(float currentTime);
		

	private:
		//�Ȃ̍Đ���������
		float mCurrentTime;
		//�Ȃ̒���(�b)
		float mMusicTime;
		//�v���O���X�o�[
		CCSprite* mProgressBar;
		//�v���O���X�A�C�R��
		CCSprite* mProgressIcon;
		//�A�C�R���̍��W
		int mStartPos_x;
		int mStartPos_y;
		int mEndPos_x;
		int mEndPos_y;


};

#endif //__MUSIC__PROGRESS__H__