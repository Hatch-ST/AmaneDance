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

		//レイヤーの登録する
		void setSprite(CCLayer *layer);

		//座標をセットする
		void setPosition(int pos_x,int pos_y);

		//指定した番号を表示する
		void setVisible(int number);
		//画像サイズ
		static const int TextureSize = 64;

		//大きさをセットする
		void setScale(float scale);
};


#endif //__NUMBER__SPRITE__H__