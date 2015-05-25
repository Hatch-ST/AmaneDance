//参考ソースコード
//http://gwi.blog.shinobi.jp/cocos2d-x/cocos2d-x%20%E3%83%9E%E3%83%AB%E3%83%81%E3%82%BF%E3%83%83%E3%83%81%E5%AF%BE%E5%BF%9C2

#include "TouchManager.h"

/**
 *コンストラクタ
 *@param touch_num タッチできる個数 1-5まで
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
 *コンストラクタ
 *@param touch_num タッチできる個数 1-5まで
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


	//タッチエフェクトの画像
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
 *デストラクタ
 */
TouchManager::~TouchManager(){
	touchData->release();
}
 
/**
 *更新処理
 */
void TouchManager::update(float delta){
	CCObject* it;
    CCARRAY_FOREACH (touchData,it){
		Touch* touch = dynamic_cast<Touch*>(it);
		if(mEffectVisible){
			//タッチ開始時にエフェクトを出す
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
		//タッチエフェクトの更新
		CCARRAY_FOREACH (mTouchEffect,it){
			TouchEffect* touchEffect = dynamic_cast<TouchEffect*>(it);
			touchEffect->update(delta);
		}
	}
}
 
/**
 *タッチ開始時に呼ぶ
 *@param index 管理上の番号
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
 *タッチ移動時に呼ぶ
 *@param index 管理上の番号
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
 *タッチキャンセル時に呼ぶ
 *@param index 管理上の番号
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
 *タッチ終了時に呼ぶ
 *@param index 管理上の番号
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
 *タッチクラスの取得
 *@param index 管理上の番号
 *@return Touch
 */
Touch* TouchManager::getTouchObject(int index){
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    return touch;
}
/**
 *タッチ位置の取得
 *@param index 管理上の番号
 *@return CCPoint postion
 */
CCPoint TouchManager::getTouchPoint(int index){
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
	CCPoint pos = CCPoint(touch->touch_x, touch->touch_y);
    return pos;
}
/**
 *Debug用 取得したTouch位置の文字列を返す
 *@return touch位置の全取得した文字列
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
