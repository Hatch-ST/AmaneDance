
#include "BestGrade.h"
#include <cmath>

BestGrade::BestGrade(int bestGrade){
	mBestGrade = bestGrade;
	init();
}

BestGrade::~BestGrade(){
	mFigureArray->release();
}

void BestGrade::init(){
	CCPoint screenSize = CCDirector::sharedDirector()->getWinSize();
	//画像配列を用意
	mFigureArray = CCArray::create();
	for(int i=0;i<FigureNumber;i++){
		NumberSprite* numberSprite = new NumberSprite();
		mFigureArray->addObject(numberSprite);
	}
	mFigureArray->retain();

	int gradeNumber[FigureNumber];
	//表示する数字を決定する
	for(int i=0;i<FigureNumber;i++){
		int num = mBestGrade / (int) pow(10.0,i);
		gradeNumber[i] = num % 10;
	}
	//0 → 1の位, 1 → 10の位 …
	for(int i=0;i<mFigureArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mFigureArray->objectAtIndex(i) );
		numberSprite->setVisible(gradeNumber[i]);
	}
}


//レイヤーを登録する
void BestGrade::setSprite(CCLayer *layer){
	for(int i=0;i<mFigureArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mFigureArray->objectAtIndex(i) );
		numberSprite->setSprite(layer);
	}
}

//座標を登録する
void BestGrade::setSpritePosition(int pos_x,int pos_y){
	for(int i=0;i<mFigureArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mFigureArray->objectAtIndex(i) );
		numberSprite->setPosition(pos_x - NumberSprite::TextureSize * 0.5f - i * (NumberSprite::TextureSize -16) , pos_y );
	}
}