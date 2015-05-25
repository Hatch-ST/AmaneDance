#ifndef __MUSIC__SELECT__SCENE__H__
#define __MUSIC__SELECT__SCENE__H__

#include "cocos2d.h"
#include "SSPlayer.h"
#include "TouchManager.h"
#include "MusicData.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SceneCreater.h"
#include "BestGrade.h"

using namespace cocos2d;
using namespace std;
class MusicSelect : public CCLayer
{
public:
	enum MovingDirection{
		Stop,
		Left,
		Right
	};

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(MusicSelect);

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

	//�o�b�N�{�^���������ꂽ�Ƃ�
	void backButtonCallback(CCObject* pSender);

	//�X�^�[�g�{�^���������ꂽ�Ƃ�
	void startButtonCallback(CCObject* pSender);

	//���{�^���������ꂽ�Ƃ�
	void leftButtonCallback(CCObject* pSender);

	//���{�^���������ꂽ�Ƃ�
	void rightButtonCallback(CCObject* pSender);

private:
	TouchManager* mTouchManager;

	//�^�C�g���ɖ߂�
	void backToTitle();

	//�I������Ă���Ȃ̃C���f�b�N�X
	int mSelectedMusicIndex;

	//���݂̋ȃ��X�g�̃y�[�W��
	int mCurrentPageIndex;

	//�ȃ��X�g�̃y�[�W���ړ�����
	void movePage(bool isLeft);

	//�r���[�p���C���[
	CCLayer* mViewLayer;

	//���C���[�̃y�[�W��
	static const int LayerPageNum = 2;
	//���X�g�̏c�Ɉ�x�ɕ\�����鐔
	static const int ListNum_y = 5;

	//�y�[�W�ړ��A�N�V��������
	MovingDirection mPageMoving;

	//�y�[�W�ړ��A�N�V������ɌĂ�
	void pageMovedCallback(CCNode* sender);

	//���{�^��
	CCMenuItemSprite *mLeftItem;

	//���{�^��
	CCMenuItemSprite *mRightItem;

	//�ȃ��X�g�̃^�b�v���`�F�b�N����
	void checkTapMusic();

	//�ȑI���t���[��
	CCSprite* mSelectedFrame;

	//�ȃ��X�g�̃X���C�v���`�F�b�N����
	void checkSwipeMusic();
	//�X���C�v������
	bool mSwiped;
	CCString* getMusicName(int index);
};

#endif // __TITLE_SCENE_H__
