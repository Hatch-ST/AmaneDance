#ifndef __VALUE__EFFECT__H__
#define __VALUE__EFFECT__H__

#include "cocos2d.h" 
#include "SS5Player.h"
#include "Note.h"
using namespace cocos2d;


class ValueEffect:public CCObject{
	public:
		ValueEffect();
		~ValueEffect();
		void init();

		//�]�������v���C���[
		CCArray* mValuePlayer;

		void setPlayer(CCLayer *layer);

		//�]�����Ƃɋ󂢂Ă���v���C���[��n��
		ss::Player* getValuePlayer(Note::Value value);

		//�|�[�Y
		void pause();

		//�|�[�Y����
		void resume();

	private:
		static const int ArrayNumber = 10;
};


#endif //__VALUE__EFFECT__H__