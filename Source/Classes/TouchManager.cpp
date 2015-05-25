//�Q�l�\�[�X�R�[�h
//http://gwi.blog.shinobi.jp/cocos2d-x/cocos2d-x%20%E3%83%9E%E3%83%AB%E3%83%81%E3%82%BF%E3%83%83%E3%83%81%E5%AF%BE%E5%BF%9C2

#include "TouchManager.h"

/**
 *�R���X�g���N�^
 *@param touch_num �^�b�`�ł���� 1-5�܂�
 */
TouchManager::TouchManager(int touch_num){
	if(touch_num<1){
		touch_num=1;
	}else if(touch_num>5){
		touch_num=5;
	}
	touchData = CCArray::create();
	touchData->retain();
	for(int i=0;i<touch_num;i++){
		Touch* touch = new Touch();
		touchData->addObject(touch);
	}
	mEffectVisible = false;
}

/**
 *�R���X�g���N�^
 *@param touch_num �^�b�`�ł���� 1-5�܂�
 */
TouchManager::TouchManager(int touch_num,CCLayer* layer){
	if(touch_num<1){
		touch_num=1;
	}else if(touch_num>5){
		touch_num=5;
	}
	touchData = CCArray::create();
	touchData->retain();
	for(int i=0;i<touch_num;i++){
		Touch* touch = new Touch();
		touchData->addObject(touch);
	}


	//�^�b�`�G�t�F�N�g�̉摜
	mTouchEffect = CCArray::create();
	mTouchEffect->retain();
	for(int i=0;i<mTouchEffectNumber;i++){
		TouchEffect* touchEffect = new TouchEffect();
		mTouchEffect->addObject(touchEffect);
		touchEffect->setPlayer(layer);
	}
	mEffectVisible = true;
}

/**
 *�f�X�g���N�^
 */
TouchManager::~TouchManager(){
	touchData->release();
}
 
/**
 *�X�V����
 */
void TouchManager::update(float delta){
	CCObject* it;
    CCARRAY_FOREACH (touchData,it){
		Touch* touch = dynamic_cast<Touch*>(it);
		if(mEffectVisible){
			//�^�b�`�J�n���ɃG�t�F�N�g���o��
			if(touch->touchBeganFlag){
				for(int i=0;i<mTouchEffectNumber;i++){
					TouchEffect* touchEffect = dynamic_cast<TouchEffect*>(mTouchEffect->objectAtIndex(i));
					if(!touchEffect->isActive()){
						touchEffect->setPosition(touch->touch_began_x,touch->touch_began_y);
						break;
					}
				}
			}
		}
		touch->update(delta);
    }
	if(mEffectVisible){
		//�^�b�`�G�t�F�N�g�̍X�V
		CCARRAY_FOREACH (mTouchEffect,it){
			TouchEffect* touchEffect = dynamic_cast<TouchEffect*>(it);
			touchEffect->update(delta);
		}
	}
}
 
/**
 *�^�b�`�J�n���ɌĂ�
 *@param index �Ǘ���̔ԍ�
 *@param x
 *@param y
 */
void TouchManager::setTouchBegan(int index,int x,int y){
	if( index >= touchData->count() ) return;
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    touch->setTouchBegan(x, y);
}
 
/**
 *�^�b�`�ړ����ɌĂ�
 *@param index �Ǘ���̔ԍ�
 *@param x
 *@param y
 */
void TouchManager::setTouchMoved(int index,int x,int y){
	if( index >= touchData->count() ) return;
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    touch->setTouchMoved(x, y);
}
 
/**
 *�^�b�`�L�����Z�����ɌĂ�
 *@param index �Ǘ���̔ԍ�
 *@param x
 *@param y
 */
void TouchManager::setTouchCancelled(int index,int x,int y){
	if( index >= touchData->count() ) return;
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    touch->setTouchCancelled(x, y);
}
 
/**
 *�^�b�`�I�����ɌĂ�
 *@param index �Ǘ���̔ԍ�
 *@param x
 *@param y
 */
void TouchManager::setTouchEnded(int index,int x,int y){
	if( index >= touchData->count() ) return;
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    touch->setTouchEnded(x, y);
}
/**
 *�^�b�`�N���X�̎擾
 *@param index �Ǘ���̔ԍ�
 *@return Touch
 */
Touch* TouchManager::getTouchObject(int index){
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    return touch;
}
/**
 *�^�b�`�ʒu�̎擾
 *@param index �Ǘ���̔ԍ�
 *@return CCPoint postion
 */
CCPoint TouchManager::getTouchPoint(int index){
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
	CCPoint pos = CCPoint(touch->touch_x, touch->touch_y);
    return pos;
}
/**
 *Debug�p �擾����Touch�ʒu�̕������Ԃ�
 *@return touch�ʒu�̑S�擾����������
 */
CCString* TouchManager::getAllTouchPointString(){
    std::string str;
    str.clear();
     
	for(int i=0;i<touchData->count();i++){
        char cstr[256];
        Touch* touch;
        touch = dynamic_cast<Touch*>( touchData->objectAtIndex(i) );
         
        sprintf(cstr, "%d:(%d,%d)",i,touch->touch_x,touch->touch_y);
        str.append(cstr);
        str.append("\n");
    }
	CCString* ccstr;
	ccstr = CCString::createWithFormat("%s",str.c_str());
     
    return ccstr;
}
