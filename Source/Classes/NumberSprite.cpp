#include "NumberSprite.h"
#include "GameScene.h"

NumberSprite::NumberSprite(){
	init();
}


NumberSprite::~NumberSprite(){
}

void NumberSprite::init(){
	//番号テクスチャ
	CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage("interface/number.png");

	//番号画像
	mNumberSprite = new CCSprite*[10];
	for(int i=0;i<10;i++){
		CCRect rect = CCRectMake(0 + i*TextureSize, 0 , TextureSize, TextureSize);
		mNumberSprite[i] = CCSprite::createWithTexture(pTexture,rect);
		if(i!=0){
			mNumberSprite[i]->setVisible(false);
		}
	}
}

//レイヤーの登録する
void NumberSprite::setSprite(CCLayer *layer){
	for(int i=0;i<10;i++){
		layer->addChild( mNumberSprite[i],InterfacePriority );
	}
}

//座標をセットする
void NumberSprite::setPosition(int pos_x,int pos_y){
	for(int i=0;i<10;i++){
		mNumberSprite[i]->setPositionX(pos_x);
		mNumberSprite[i]->setPositionY(pos_y);
	}
}

//大きさをセットする
void NumberSprite::setScale(float scale){
	for(int i=0;i<10;i++){
		mNumberSprite[i]->setScale(scale);
	}
}

//指定した番号を表示する
void NumberSprite::setVisible(int number){
	for(int i=0;i<10;i++){
		if(i==number){
			mNumberSprite[i]->setVisible(true);
		}else{
			mNumberSprite[i]->setVisible(false);
		}		
	}
}