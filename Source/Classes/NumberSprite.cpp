#include "NumberSprite.h"
#include "GameScene.h"

NumberSprite::NumberSprite(){
	init();
}


NumberSprite::~NumberSprite(){
}

void NumberSprite::init(){
	//�ԍ��e�N�X�`��
	CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage("interface/number.png");

	//�ԍ��摜
	mNumberSprite = new CCSprite*[10];
	for(int i=0;i<10;i++){
		CCRect rect = CCRectMake(0 + i*TextureSize, 0 , TextureSize, TextureSize);
		mNumberSprite[i] = CCSprite::createWithTexture(pTexture,rect);
		if(i!=0){
			mNumberSprite[i]->setVisible(false);
		}
	}
}

//���C���[�̓o�^����
void NumberSprite::setSprite(CCLayer *layer){
	for(int i=0;i<10;i++){
		layer->addChild( mNumberSprite[i],InterfacePriority );
	}
}

//���W���Z�b�g����
void NumberSprite::setPosition(int pos_x,int pos_y){
	for(int i=0;i<10;i++){
		mNumberSprite[i]->setPositionX(pos_x);
		mNumberSprite[i]->setPositionY(pos_y);
	}
}

//�傫�����Z�b�g����
void NumberSprite::setScale(float scale){
	for(int i=0;i<10;i++){
		mNumberSprite[i]->setScale(scale);
	}
}

//�w�肵���ԍ���\������
void NumberSprite::setVisible(int number){
	for(int i=0;i<10;i++){
		if(i==number){
			mNumberSprite[i]->setVisible(true);
		}else{
			mNumberSprite[i]->setVisible(false);
		}		
	}
}