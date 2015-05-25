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

		//レイヤーの登録する
		void setSprite(CCLayer *layer);

		//座標を登録する
		void setSpritePosition(int pos_x,int pos_y);
	private:
		//桁ごとの数字画像配列
		CCArray* mFigureArray;
		//スコアの桁数
		static const int FigureNumber = 6;
		//ベストスコア
		int mBestGrade;
};


#endif //__GRADE__MANAGER__H__