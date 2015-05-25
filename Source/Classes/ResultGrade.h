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


		//レイヤーの登録する
		void setSprite(CCLayer *layer);

		bool updateGrade(float delta);

		void setSpritePosition(SetType type,int pos_y);

	private:
		//Coolの数字画像配列
		CCArray* mCoolArray;
		//Greatの数字画像配列
		CCArray* mGreatArray;
		//Goodの数字画像配列
		CCArray* mGoodArray;
		//Badの数字画像配列
		CCArray* mBadArray;
		//Missの数字画像配列
		CCArray* mMissArray;
		//評価の回数の桁数
		static const int ValuesNumber = 3;

		//スコアの数字画像配列
		CCArray* mGradeArray;
		//スコアの桁数
		static const int GradeNumber = 6;

		//現在のスコア
		int mCurrentGrade;
		//加算されるスコア
		int mCacheGrade;

		//スコア画像を更新する
		void updateNumberSprite();


		//経過時間
		float passedTime;

		SetType mAnimeType;

		bool mAnimeEnded;
};


#endif //__RESULT__GRADE__H__