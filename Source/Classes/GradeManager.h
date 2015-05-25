#ifndef __GRADE__MANAGER__H__
#define __GRADE__MANAGER__H__

#include "cocos2d.h"
#include "NumberSprite.h"
#include "Note.h"
using namespace cocos2d;

struct GradeData{
	int totalGrade;
	int coolNum;
	int greatNum;
	int goodNum;
	int badNum;
	int missNum;
};

class GradeManager:public CCObject{
	public:
		GradeManager();
		~GradeManager();
		void init();


		//���C���[�̓o�^����
		void setSprite(CCLayer *layer);

		void update();

		//�]�����Ƃɉ��Z�X�R�A��ǉ�����
		void addCacheGrade(Note::Value value);

		//�z�[���h���̉��Z�X�R�A��ǉ�����
		void addCacheGradeByHold();

		//�g�[�^���X�R�A���擾����
		void getTotalGrade();

	private:
		//�����Ƃ̐����摜�z��
		CCArray* mFigureArray;
		//�X�R�A�̌���
		static const int FigureNumber = 6;

		//���݂̃X�R�A
		int mCurrentGrade;
		//���Z�����X�R�A
		int mCacheGrade;
		
		//�e�ʂ̐������擾����
		void getGradeNumbers(int gradeNumber[]);

		//�X�R�A�摜���X�V����
		void updateNumberSprite(int gradeNumber[]);

		//���݂̃X�R�A�����Z����
		void addCurrentGrade();

		//�P�T�C�N�����Ƃɉ��Z����X�R�A
		static const int AddGradeNumberByFrame = 10;
};


#endif //__GRADE__MANAGER__H__