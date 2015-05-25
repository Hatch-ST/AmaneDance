#ifndef __TOUCH__H__
#define __TOUCH__H__

#include "cocos2d.h" 

//タッチクラス
class Touch:public cocos2d::CCObject{
public:
    Touch();
     
    //タッチしているか
    bool touchFlag;
    //タッチが開始されたか
    bool touchBeganFlag;
    //タッチが終了されたか
    bool touchEndedFlag;
     
    //タッチ座標
    int touch_x;
    int touch_y;
    //1フレーム前のタッチ座標
    int touch_past_x;
    int touch_past_y;
    //開始位置
    int touch_began_x;
    int touch_began_y;
    //終了位置
    int touch_ended_x;
    int touch_ended_y;
    //タッチしている時間
    float touch_time;
     
    void update(float delta);
    void setTouchBegan(int x,int y);
    void setTouchMoved(int x,int y);
    void setTouchEnded(int x,int y);
    void setTouchCancelled(int x,int y);
     
};
 
#endif // __TOUCH__H__