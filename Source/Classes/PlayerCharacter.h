#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "SSPlayer.h"
#include "SS5Player.h"
#include "MotionData.h"

//�v���C���[�Ǘ��N���X
class PlayerCharacter{
	public:
		//������
		virtual void init(MusicName musicName);

		//�R���X�g���N�^
		PlayerCharacter(MusicName musicName);

		//�X�V����
		virtual bool update(int gameTime,float delta);

		//SSPlayer�̃Z�b�g
		virtual void setSSPlayer(cocos2d::CCLayer *layer);

		//�~�X������
		bool mIsMissed;
		//�~�X���[�V����
		MotionData::PlayerMotion mMissMotion;

		//�|�[�Y
		void pause();

		//�|�[�Y����
		void resume();
	private:
		//�v���C���[�̍��W
		//cocos2d::CCPoint* mPlayerPos;
		//�C���[�W���X�g
		//SSImageList*	mImageList;
		//SS�v���C���[
		ss::Player*		mSSPlayer;

		//�o�b�N���C�g�C���[�W
		//SSImageList*	mBackImage;
		//�o�b�N���C�gSS�v���C���[
		ss::Player*	mBackSSPlayer;

		//�`��ԍ�
		int mDrawNumber;
		//BPM
		int m_bpm;
		//���q
		float mRhythm;
		//�P�t���[�Y�̒���
		int mOnePhraseFrame;
		//�_���X��1�t���[�Y�������ߕ��Ƃ邩
		int mOnePhraseBarNumber;

		//���[�V�����f�[�^
		MotionData::PlayerMotion mMotionData[MOTION_MAX_SIZE];
		//�P�t���[���O�̃��[�V����
		int mBeforeMotion;
		//�ȏI���܂ł̃��[�V������
		int mMotionNumber;

		//���[�V������ύX����
		virtual void changeMotion(MotionData::PlayerMotion motion,int gameTime);

		//���[�V�������ύX���ꂽ��
		bool mMotionChanged;

};

#endif // __PLAYER_H__