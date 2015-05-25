#ifndef __TOUCH__H__
#define __TOUCH__H__

#include "cocos2d.h" 

//�^�b�`�N���X
class Touch:public cocos2d::CCObject{
public:
    Touch();
     
    //�^�b�`���Ă��邩
    bool touchFlag;
    //�^�b�`���J�n���ꂽ��
    bool touchBeganFlag;
    //�^�b�`���I�����ꂽ��
    bool touchEndedFlag;
     
    //�^�b�`���W
    int touch_x;
    int touch_y;
    //1�t���[���O�̃^�b�`���W
    int touch_past_x;
    int touch_past_y;
    //�J�n�ʒu
    int touch_began_x;
    int touch_began_y;
    //�I���ʒu
    int touch_ended_x;
    int touch_ended_y;
    //�^�b�`���Ă��鎞��
    float touch_time;
     
    void update(float delta);
    void setTouchBegan(int x,int y);
    void setTouchMoved(int x,int y);
    void setTouchEnded(int x,int y);
    void setTouchCancelled(int x,int y);
     
};
 
#endif // __TOUCH__H__