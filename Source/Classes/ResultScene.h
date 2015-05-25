#ifndef __RESULT_SCENE_H__
#define __RESULT_SCENE_H__

#include "cocos2d.h"
#include "SSPlayer.h"
#include "TouchManager.h"
#include "MusicData.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneCreater.h"
#include "ResultGrade.h"

using namespace cocos2d;
class Result : public CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(Result);

	virtual void update(float delta);

	//�V�[�����n�܂������ɌĂяo����郁�\�b�h
	virtual void onEnter();
	
	//�}���`�^�b�`
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);

	//again�{�^�����Ă΂ꂽ�Ƃ�
	void againCallback(CCObject* pSender);
	//quit�{�^�����Ă΂ꂽ�Ƃ�
	void quitCallback(CCObject* pSender);

private:
	TouchManager* mTouchManager;

	ResultGrade* mResultGrade;

	CCMenu* mMenu;
};

#endif // __RESULT_SCENE_H__
