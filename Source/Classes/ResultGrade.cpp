#include "ResultGrade.h"
#include <cmath>
#include <time.h>

#ifdef G_GRADE_DATA
struct GradeData gGradeData;
#else
extern struct GradeData gGradeData;
#endif

ResultGrade::ResultGrade(){
	init();
}


ResultGrade::~ResultGrade(){
	mCoolArray->release();
	mGreatArray->release();
	mGoodArray->release();
	mBadArray->release();
	mMissArray->release();
	mGradeArray->release();
}

void ResultGrade::init(){
	srand((unsigned int)time(NULL));

	//画像配列を用意
	mCoolArray = CCArray::create();
	for(int i=0;i<ValuesNumber;i++){
		NumberSprite* numberSprite = new NumberSprite();
		mCoolArray->addObject(numberSprite);
		numberSprite->setVisible(-1);
	}
	mCoolArray->retain();

	mGreatArray = CCArray::create();
	for(int i=0;i<ValuesNumber;i++){
		NumberSprite* numberSprite = new NumberSprite();
		mGreatArray->addObject(numberSprite);
		numberSprite->setVisible(-1);
	}
	mGreatArray->retain();

	mGoodArray = CCArray::create();
	for(int i=0;i<ValuesNumber;i++){
		NumberSprite* numberSprite = new NumberSprite();
		mGoodArray->addObject(numberSprite);
		numberSprite->setVisible(-1);
	}
	mGoodArray->retain();

	mBadArray = CCArray::create();
	for(int i=0;i<ValuesNumber;i++){
		NumberSprite* numberSprite = new NumberSprite();
		mBadArray->addObject(numberSprite);
		numberSprite->setVisible(-1);
	}
	mBadArray->retain();

	mMissArray = CCArray::create();
	for(int i=0;i<ValuesNumber;i++){
		NumberSprite* numberSprite = new NumberSprite();
		mMissArray->addObject(numberSprite);
		numberSprite->setVisible(-1);
	}
	mMissArray->retain();

	mGradeArray = CCArray::create();
	for(int i=0;i<GradeNumber;i++){
		NumberSprite* numberSprite = new NumberSprite();
		mGradeArray->addObject(numberSprite);
		numberSprite->setScale(1.5f);
		numberSprite->setVisible(-1);
	}
	mGradeArray->retain();

	mCurrentGrade = 0;
	mCacheGrade = 0;

	passedTime = 0;

	mAnimeType = SET_COOL;
	mAnimeEnded = false;
}


//レイヤーの登録する
void ResultGrade::setSprite(CCLayer *layer){
	for(int i=0;i<mCoolArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mCoolArray->objectAtIndex(i) );
		numberSprite->setSprite(layer);
	}
	for(int i=0;i<mGreatArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGreatArray->objectAtIndex(i) );
		numberSprite->setSprite(layer);
	}
	for(int i=0;i<mGoodArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGoodArray->objectAtIndex(i) );
		numberSprite->setSprite(layer);
	}
	for(int i=0;i<mBadArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mBadArray->objectAtIndex(i) );
		numberSprite->setSprite(layer);
	}
	for(int i=0;i<mMissArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mMissArray->objectAtIndex(i) );
		numberSprite->setSprite(layer);
	}
	for(int i=0;i<mGradeArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(i) );
		numberSprite->setSprite(layer);
	}

}

void ResultGrade::setSpritePosition(SetType type,int pos_y){
	CCPoint screenSize = CCDirector::sharedDirector()->getWinSize();
	switch(type){
		case SET_COOL:
			for(int i=0;i<mCoolArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mCoolArray->objectAtIndex(i) );
				int pos_x = screenSize.x * 0.5f - (i - 1) *NumberSprite::TextureSize + (i - 1) * 16;
				numberSprite->setPosition(pos_x,pos_y);
			}
			break;
		case SET_GREAT:
			for(int i=0;i<mGreatArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGreatArray->objectAtIndex(i) );
				int pos_x = screenSize.x * 0.5f - (i - 1) *NumberSprite::TextureSize + (i - 1) * 16;
				numberSprite->setPosition(pos_x,pos_y);
			}
			break;
		case SET_GOOD:
			for(int i=0;i<mGoodArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGoodArray->objectAtIndex(i) );
				int pos_x = screenSize.x * 0.5f - (i - 1) *NumberSprite::TextureSize + (i - 1) * 16;
				numberSprite->setPosition(pos_x,pos_y);
			}
			break;
		case SET_BAD:
			for(int i=0;i<mBadArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mBadArray->objectAtIndex(i) );
				int pos_x = screenSize.x * 0.5f - (i - 1) *NumberSprite::TextureSize + (i - 1) * 16;
				numberSprite->setPosition(pos_x,pos_y);
			}
			break;
		case SET_MISS:
			for(int i=0;i<mMissArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mMissArray->objectAtIndex(i) );
				int pos_x = screenSize.x * 0.5f - (i - 1) *NumberSprite::TextureSize + (i - 1) * 16;
				numberSprite->setPosition(pos_x,pos_y);
			}
			break;
		case SET_GRADE:
			for(int i=0;i<mGradeArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(i) );
				int pos_x = screenSize.x - 30 - i * NumberSprite::TextureSize * 1.5f + i * 24 - NumberSprite::TextureSize * 0.75f;
				numberSprite->setPosition(pos_x,pos_y);
			}
			break;
	}
}

bool ResultGrade::updateGrade(float delta){
	//画像更新
	updateNumberSprite();

	passedTime += delta;

	return mAnimeEnded;
}

//スコア画像を更新する
void ResultGrade::updateNumberSprite(){
	if( mAnimeEnded ){
		return;
	}
	int mNumbers[GradeNumber] = {0,};
	switch(mAnimeType){
	case SET_COOL:
		if(passedTime >= 1.5f){
			for(int i=0;i<mCoolArray->count();i++){
				int num = gGradeData.coolNum / (int) pow(10.0,i);
				mNumbers[i] = num % 10;
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mCoolArray->objectAtIndex(i) );
				numberSprite->setVisible(mNumbers[i]);
			}
			passedTime = 0;
			mAnimeType = SET_GREAT;
		}
		break;
	case SET_GREAT:
		if(passedTime >= 0.7f){
			for(int i=0;i<mGreatArray->count();i++){
				int num = gGradeData.greatNum / (int) pow(10.0,i);
				mNumbers[i] = num % 10;
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGreatArray->objectAtIndex(i) );
				numberSprite->setVisible(mNumbers[i]);
			}
			passedTime = 0;
			mAnimeType = SET_GOOD;
		}
		break;
	case SET_GOOD:
		if(passedTime >= 0.7f){
			for(int i=0;i<mGoodArray->count();i++){
				int num = gGradeData.goodNum / (int) pow(10.0,i);
				mNumbers[i] = num % 10;
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGoodArray->objectAtIndex(i) );
				numberSprite->setVisible(mNumbers[i]);
			}
			passedTime = 0;
			mAnimeType = SET_BAD;
		}
		break;
	case SET_BAD:
		if(passedTime >= 0.7f){
			for(int i=0;i<mBadArray->count();i++){
				int num = gGradeData.badNum / (int) pow(10.0,i);
				mNumbers[i] = num % 10;
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mBadArray->objectAtIndex(i) );
				numberSprite->setVisible(mNumbers[i]);
			}
			passedTime = 0;
			mAnimeType = SET_MISS;
		}
		break;
	case SET_MISS:
		if(passedTime >= 0.7f){
			for(int i=0;i<mMissArray->count();i++){
				int num = gGradeData.missNum / (int) pow(10.0,i);
				mNumbers[i] = num % 10;
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mMissArray->objectAtIndex(i) );
				numberSprite->setVisible(mNumbers[i]);
			}
			passedTime = 0;
			mAnimeType = SET_GRADE;
		}
		break;
	case SET_GRADE:
		if(passedTime < 1.2f){
			//なにもしない
		}else if(passedTime <= 2.0f){
			//乱数で表示
			for(int i=0;i<mGradeArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(i) );
				numberSprite->setVisible( rand() % 9 );
			}
		}else if(passedTime <= 2.5f){
			int drawNum = 1;
			//1の位を表示
			NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(drawNum-1) );
			int num = ( gGradeData.totalGrade / (int) pow(10.0,drawNum-1) ) % 10;
			numberSprite->setVisible( num );
			
			//乱数で表示
			for(int i=drawNum;i<mGradeArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(i) );
				numberSprite->setVisible( rand() % 9 );
			}
		}else if(passedTime <= 3.0f){
			int drawNum = 2;
			//十の位を表示
			NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(drawNum-1) );
			int num = ( gGradeData.totalGrade / (int) pow(10.0,drawNum-1) ) % 10;
			numberSprite->setVisible( num );
			
			//乱数で表示
			for(int i=drawNum;i<mGradeArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(i) );
				numberSprite->setVisible( rand() % 9 );
			}
		}else if(passedTime <= 3.5f){
			int drawNum = 3;
			//百の位を表示
			NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(drawNum-1) );
			int num = ( gGradeData.totalGrade / (int) pow(10.0,drawNum-1) ) % 10;
			numberSprite->setVisible( num );
			
			//乱数で表示
			for(int i=drawNum;i<mGradeArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(i) );
				numberSprite->setVisible( rand() % 9 );
			}
		}else if(passedTime <= 4.0f){
			int drawNum = 4;
			//千の位を表示
			NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(drawNum-1) );
			int num = ( gGradeData.totalGrade / (int) pow(10.0,drawNum-1) ) % 10;
			numberSprite->setVisible( num );
			
			//乱数で表示
			for(int i=drawNum;i<mGradeArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(i) );
				numberSprite->setVisible( rand() % 9 );
			}
		}else if(passedTime <= 4.5f){
			int drawNum = 5;
			//一万の位を表示
			NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(drawNum-1) );
			int num = ( gGradeData.totalGrade / (int) pow(10.0,drawNum-1) ) % 10;
			numberSprite->setVisible( num );
			
			//乱数で表示
			for(int i=drawNum;i<mGradeArray->count();i++){
				NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(i) );
				numberSprite->setVisible( rand() % 9 );
			}
		}else if(passedTime <= 5.0f){
			int drawNum = 6;
			//十万の位を表示
			NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mGradeArray->objectAtIndex(drawNum-1) );
			int num = ( gGradeData.totalGrade / (int) pow(10.0,drawNum-1) ) % 10;
			numberSprite->setVisible( num );
			mAnimeEnded = true;
		}
		break;
	}	
}

