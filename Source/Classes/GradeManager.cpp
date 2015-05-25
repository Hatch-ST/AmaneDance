#include "GradeManager.h"
#include <cmath>

#ifdef G_GRADE_DATA
struct GradeData gGradeData;
#else
extern struct GradeData gGradeData;
#endif

GradeManager::GradeManager(){
	init();
}


GradeManager::~GradeManager(){
	mFigureArray->release();
}

void GradeManager::init(){
	CCPoint screenSize = CCDirector::sharedDirector()->getWinSize();
	//画像配列を用意
	mFigureArray = CCArray::create();
	for(int i=0;i<FigureNumber;i++){
		NumberSprite* numberSprite = new NumberSprite();
		mFigureArray->addObject(numberSprite);
		int pos_x = screenSize.x - 40 - NumberSprite::TextureSize/2 - i*NumberSprite::TextureSize;
		int pos_y = screenSize.y - 50 ;
		numberSprite->setPosition(pos_x,pos_y);
	}
	mFigureArray->retain();
	mCurrentGrade = 0;
	mCacheGrade = 0;
}


//レイヤーの登録する
void GradeManager::setSprite(CCLayer *layer){
	for(int i=0;i<mFigureArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mFigureArray->objectAtIndex(i) );
		numberSprite->setSprite(layer);
	}
}

void GradeManager::update(){
	//各桁の数字を取得
	int gradeNumbers[FigureNumber];
	getGradeNumbers(gradeNumbers);

	//画像更新
	updateNumberSprite(gradeNumbers);
	//スコアを加算
	addCurrentGrade();
}

//各位の数字を取得する
void GradeManager::getGradeNumbers(int gradeNumber[]){
	for(int i=0;i<FigureNumber;i++){
		int num = mCurrentGrade / (int) pow(10.0,i);
		gradeNumber[i] = num % 10;
	}
}

//スコア画像を更新する
void GradeManager::updateNumberSprite(int gradeNumber[]){
	//0 → 1の位, 1 → 10の位 …
	for(int i=0;i<mFigureArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mFigureArray->objectAtIndex(i) );
		numberSprite->setVisible(gradeNumber[i]);
	}
}

//現在のスコアを加算する
void GradeManager::addCurrentGrade(){
	if(mCacheGrade >= AddGradeNumberByFrame){
		mCurrentGrade += AddGradeNumberByFrame;
		mCacheGrade -= AddGradeNumberByFrame;
	}else{
		mCurrentGrade += mCacheGrade;
		mCacheGrade = 0;
	}
}

//評価ごとに加算スコアを追加する
void GradeManager::addCacheGrade(Note::Value value){
	switch(value){
		case Note::COOL:
			mCacheGrade += 500;
			gGradeData.coolNum++;
			break;
		case Note::GREAT:
			mCacheGrade += 250;
			gGradeData.greatNum++;
			break;
		case Note::GOOD:
			mCacheGrade += 150;
			gGradeData.goodNum++;
			break;
		case Note::BAD:
			mCacheGrade += 50;
			gGradeData.badNum++;
			break;
	}
}

//ホールド時の加算スコアを追加する
void GradeManager::addCacheGradeByHold(){
	int holdPoint = 5;
	mCacheGrade += holdPoint;
}

//トータルスコアを取得する
void GradeManager::getTotalGrade(){
	gGradeData.totalGrade = mCurrentGrade + mCacheGrade;
}