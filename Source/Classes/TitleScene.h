#ifndef __TITLE__SCENE__H__
#define __TITLE__SCENE__H__

#include "cocos2d.h"
#include "SS5Player.h"
#include "TouchManager.h"
#include "MusicData.h"
#include "MusicSelectScene.h"
#include "SceneCreater.h"
#include "OptionLayer.h"

using namespace cocos2d;
class Title : public CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(Title);

	virtual void update(float delta);

	//�V�[�����n�܂������ɌĂяo����郁�\�b�h
	virtual void onEnter();
	
	//�}���`�^�b�`
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	//android �o�b�N�L�[
	void keyBackClicked();
	//android ���j���[�L�[
	void keyMenuClicked();

	//�Q�[���X�^�[�g�������ꂽ�Ƃ�
	void gameStartCallback(CCObject* pSender);

	//�I�v�V�����������ꂽ�Ƃ�
	void optionCallback(CCObject* pSender);

	//�I���{�^���������ꂽ�Ƃ�
	void exitCallback(CCObject* pSender);

	//�I�v�V�����������Ƃ�
	void closeOption();

private:
	TouchManager* mTouchManager;

	//�Q�[���I��
	void exitApp();

	ss::Player* mSSPlayer;
	ss::Player* mTitleBGPlayer;

	bool mOptionOpened;

	static const int mOptionTagNum = 101;

	//�I�v�V�������J���ꂽ�Ƃ�
	void openOption();
};

#endif // __TITLE__SCENE__H__
