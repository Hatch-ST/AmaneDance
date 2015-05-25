#ifndef __BEST__GRADE__H__
#define __BEST__GRADE__H__

#include "cocos2d.h"
#include "NumberSprite.h"

using namespace cocos2d;

class BestGrade:public CCObject{
	public:
		BestGrade(int bestGrade);
		~BestGrade();
		void init();

		//���C���[�̓o�^����
		void setSprite(CCLayer *layer);

		//���W��o�^����
		void setSpritePosition(int pos_x,int pos_y);
	private:
		//�����Ƃ̐����摜�z��
		CCArray* mFigureArray;
		//�X�R�A�̌���
		static const int FigureNumber = 6;
		//�x�X�g�X�R�A
		int mBestGrade;
};


#endif //__GRADE__MANAGER__H__