#ifndef __RESULT__GRADE__H__
#define __RESULT__GRADE__H__

#include "cocos2d.h"
#include "NumberSprite.h"
#include "GradeManager.h"
using namespace cocos2d;

class ResultGrade:public CCObject{
	public:
		enum SetType{
			SET_COOL,
			SET_GREAT,
			SET_GOOD,
			SET_BAD,
			SET_MISS,
			SET_GRADE
		};
		ResultGrade();
		~ResultGrade();
		void init();


		//���C���[�̓o�^����
		void setSprite(CCLayer *layer);

		bool updateGrade(float delta);

		void setSpritePosition(SetType type,int pos_y);

	private:
		//Cool�̐����摜�z��
		CCArray* mCoolArray;
		//Great�̐����摜�z��
		CCArray* mGreatArray;
		//Good�̐����摜�z��
		CCArray* mGoodArray;
		//Bad�̐����摜�z��
		CCArray* mBadArray;
		//Miss�̐����摜�z��
		CCArray* mMissArray;
		//�]���̉񐔂̌���
		static const int ValuesNumber = 3;

		//�X�R�A�̐����摜�z��
		CCArray* mGradeArray;
		//�X�R�A�̌���
		static const int GradeNumber = 6;

		//���݂̃X�R�A
		int mCurrentGrade;
		//���Z�����X�R�A
		int mCacheGrade;

		//�X�R�A�摜���X�V����
		void updateNumberSprite();


		//�o�ߎ���
		float passedTime;

		SetType mAnimeType;

		bool mAnimeEnded;
};


#endif //__RESULT__GRADE__H__