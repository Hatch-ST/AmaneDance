#ifndef __NUMBER__SPRITE__H__
#define __NUMBER__SPRITE__H__

#include "cocos2d.h" 
using namespace cocos2d;


class NumberSprite:public CCObject{
	public:
		NumberSprite();
		~NumberSprite();
		void init();
		CCSprite** mNumberSprite;

		//���C���[�̓o�^����
		void setSprite(CCLayer *layer);

		//���W���Z�b�g����
		void setPosition(int pos_x,int pos_y);

		//�w�肵���ԍ���\������
		void setVisible(int number);
		//�摜�T�C�Y
		static const int TextureSize = 64;

		//�傫�����Z�b�g����
		void setScale(float scale);
};


#endif //__NUMBER__SPRITE__H__