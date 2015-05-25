
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
	//�摜�z���p��
	mFigureArray = CCArray::create();
	for(int i=0;i<FigureNumber;i++){
		NumberSprite* numberSprite = new NumberSprite();
		mFigureArray->addObject(numberSprite);
	}
	mFigureArray->retain();

	int gradeNumber[FigureNumber];
	//�\�����鐔�������肷��
	for(int i=0;i<FigureNumber;i++){
		int num = mBestGrade / (int) pow(10.0,i);
		gradeNumber[i] = num % 10;
	}
	//0 �� 1�̈�, 1 �� 10�̈� �c
	for(int i=0;i<mFigureArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mFigureArray->objectAtIndex(i) );
		numberSprite->setVisible(gradeNumber[i]);
	}
}


//���C���[��o�^����
void BestGrade::setSprite(CCLayer *layer){
	for(int i=0;i<mFigureArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mFigureArray->objectAtIndex(i) );
		numberSprite->setSprite(layer);
	}
}

//���W��o�^����
void BestGrade::setSpritePosition(int pos_x,int pos_y){
	for(int i=0;i<mFigureArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mFigureArray->objectAtIndex(i) );
		numberSprite->setPosition(pos_x - NumberSprite::TextureSize * 0.5f - i * (NumberSprite::TextureSize -16) , pos_y );
	}
}