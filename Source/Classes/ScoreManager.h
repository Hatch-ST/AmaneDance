#ifndef __SCORE__MANAGER__H__
#define __SCORE__MANAGER__H__

#include "cocos2d.h"
#include "ScoreData.h"
#include "Note.h"
#include "NoteTap.h"
#include "NoteTapSprite.h"
//#include "NoteHold.h"
//#include "NoteHoldSprite.h"
#include "OnePhraseScore.h"
#include "ValueEffect.h"
#include "Navigator.h"
#include "GradeManager.h"

using namespace cocos2d;


class ScoreManager:public cocos2d::CCObject{
	public:
		ScoreManager(MusicName musicName,CCLayer *layer);
		~ScoreManager();
		//�X�V����
		virtual bool update(int gameTime,float delta);

		//������
		virtual void init(MusicName musicName,CCLayer *layer);

		//�^�b�v�𔻒肷��
		bool checkTap(int touch_x,int touch_y,GradeManager* gradeMnager);

		//�V�F�C�N�𔻒肷��
		bool checkShake(GradeManager* gradeMnager);

		//�����[�X�𔻒肷��
		bool checkRelease(int touch_x,int touch_y,GradeManager* gradeMnager);

		//�z�[���h�𔻒肷��
		bool checkHold(int touch_x,int touch_y,GradeManager* gradeMnager);

		//�|�[�Y���ɃA�j���[�V�������ꎞ��~����
		void pauseAnimation();
		//�|�[�Y�������ɃA�j���[�V�������ĊJ����
		void resumeAnimation();


	private:
		//BPM
		int m_bpm;
		//���q
		float mRhythm;
		//�P�t���[�Y�̒���
		int mOnePhraseFrame;
		//�y����1�t���[�Y�������ߕ��Ƃ邩
		int mOnePhraseBarNumber;
		float mStep;

		//�y���f�[�^
		ScoreData::NoteType mScoreData[SCORE_MAX_SIZE];

		//�t���[�Y���������邩
		int mScoreMaxNumber;

		//���݂̃t���[�Y
		int mCurrentPhraseNumber;

		//�P�t���[�Y�̊�t���[����
		int mBaseFrame;

		//�P�t���[�Y�̊y��		
		CCArray* mPhraseQueue;
		//�L���[�z��̐�
		int mQueueSize;
		//�L���[�̉��Ԗڂ����ݒn��
		int mCurrentNumberOfQueue;

		//�t���[�Y���ƂɃt���[����i�߂�
		virtual bool updateFrame(OnePhraseScore* phrase,int frame,float delta);

		//�m�[�g�^�b�v�̉摜�Ǘ�
		CCArray* mNoteTapSprite;
		static const int mNoteTapSpriteNumber = 10;

		//�m�[�g�z�[���h�̉摜�Ǘ�
		CCArray* mNoteHoldSprite;
		static const int mNoteHoldSpriteNumber = 4;

		//�m�[�g�����摜�ɓo�^����
		void setNoteData(OnePhraseScore* phrase);

		//�]���摜�Ǘ�
		ValueEffect* mValueEffect;

		//�i�r
		Navigator* mNavi;

		//�i�r�̍X�V
		void setNaviNote();

		//�i�r���L����
		bool mNaviUsed;

};


#endif //__SCORE__MANAGER__H__