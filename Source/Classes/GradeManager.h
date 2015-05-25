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


		//レイヤーの登録する
		void setSprite(CCLayer *layer);

		void update();

		//評価ごとに加算スコアを追加する
		void addCacheGrade(Note::Value value);

		//ホールド時の加算スコアを追加する
		void addCacheGradeByHold();

		//トータルスコアを取得する
		void getTotalGrade();

	private:
		//桁ごとの数字画像配列
		CCArray* mFigureArray;
		//スコアの桁数
		static const int FigureNumber = 6;

		//現在のスコア
		int mCurrentGrade;
		//加算されるスコア
		int mCacheGrade;
		
		//各位の数字を取得する
		void getGradeNumbers(int gradeNumber[]);

		//スコア画像を更新する
		void updateNumberSprite(int gradeNumber[]);

		//現在のスコアを加算する
		void addCurrentGrade();

		//１サイクルごとに加算するスコア
		static const int AddGradeNumberByFrame = 10;
};


#endif //__GRADE__MANAGER__H__