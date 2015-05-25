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
	//�摜�z���p��
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


//���C���[�̓o�^����
void GradeManager::setSprite(CCLayer *layer){
	for(int i=0;i<mFigureArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mFigureArray->objectAtIndex(i) );
		numberSprite->setSprite(layer);
	}
}

void GradeManager::update(){
	//�e���̐������擾
	int gradeNumbers[FigureNumber];
	getGradeNumbers(gradeNumbers);

	//�摜�X�V
	updateNumberSprite(gradeNumbers);
	//�X�R�A�����Z
	addCurrentGrade();
}

//�e�ʂ̐������擾����
void GradeManager::getGradeNumbers(int gradeNumber[]){
	for(int i=0;i<FigureNumber;i++){
		int num = mCurrentGrade / (int) pow(10.0,i);
		gradeNumber[i] = num % 10;
	}
}

//�X�R�A�摜���X�V����
void GradeManager::updateNumberSprite(int gradeNumber[]){
	//0 �� 1�̈�, 1 �� 10�̈� �c
	for(int i=0;i<mFigureArray->count();i++){
		NumberSprite* numberSprite = dynamic_cast<NumberSprite*>( mFigureArray->objectAtIndex(i) );
		numberSprite->setVisible(gradeNumber[i]);
	}
}

//���݂̃X�R�A�����Z����
void GradeManager::addCurrentGrade(){
	if(mCacheGrade >= AddGradeNumberByFrame){
		mCurrentGrade += AddGradeNumberByFrame;
		mCacheGrade -= AddGradeNumberByFrame;
	}else{
		mCurrentGrade += mCacheGrade;
		mCacheGrade = 0;
	}
}

//�]�����Ƃɉ��Z�X�R�A��ǉ�����
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

//�z�[���h���̉��Z�X�R�A��ǉ�����
void GradeManager::addCacheGradeByHold(){
	int holdPoint = 5;
	mCacheGrade += holdPoint;
}

//�g�[�^���X�R�A���擾����
void GradeManager::getTotalGrade(){
	gGradeData.totalGrade = mCurrentGrade + mCacheGrade;
}