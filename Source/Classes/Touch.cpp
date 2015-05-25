//�Q�l�\�[�X�R�[�h
//http://gwi.blog.shinobi.jp/cocos2d-x/cocos2d-x%20%E3%83%9E%E3%83%AB%E3%83%81%E3%82%BF%E3%83%83%E3%83%81%E5%AF%BE%E5%BF%9C2

#include "Touch.h"
 
/**
 *�R���X�g���N�^
 */
Touch::Touch(){
    touchFlag = false;
    touchBeganFlag=false;
    touchEndedFlag=false;
	touch_x=0;
	touch_y=0;
	touch_past_x=0;
	touch_past_y=0;
}
 
/**
 *�X�V����
 */
void Touch::update(float delta){
    if(touchFlag==true){
        touch_time += delta;
    }
    touch_past_x = touch_x;
    touch_past_y = touch_y;
    touchBeganFlag=false;
    touchEndedFlag=false;
}
 
/**
 *�^�b�`�J�n���ɌĂ�
 *@param x
 *@param y
 */
void Touch::setTouchBegan(int x,int y){
    touch_x=x;
    touch_y=y;
    touch_began_x=x;
    touch_began_y=y;
    touchBeganFlag = true;
    touchEndedFlag = false;
    touchFlag = true;
    touch_time=0;
}
 
/**
 *�^�b�`�ړ����ɌĂ�
 *@param x
 *@param y
 */
void Touch::setTouchMoved(int x,int y){
    touch_x=x;
    touch_y=y;
    touchFlag = true;
}
 
/**
 *�^�b�`�L�����Z�����ɌĂ�
 *@param x
 *@param y
 */
void Touch::setTouchCancelled(int x,int y){
    touch_x=x;
    touch_y=y;
    touch_ended_x=x;
    touch_ended_y=y;
    touchEndedFlag = true;
    touchFlag = false;
}
 
/**
 *�^�b�`�I�����ɌĂ�
 *@param x
 *@param y
 */
void Touch::setTouchEnded(int x,int y){
    touch_x=x;
    touch_y=y;
    touch_ended_x=x;
    touch_ended_y=y;
    touchEndedFlag = true;
    touchFlag = false;
}