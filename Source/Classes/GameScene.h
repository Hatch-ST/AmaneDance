#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "SS5Player.h"
#include "PlayerCharacter.h"
#include "TouchManager.h"
#include "MusicData.h"
#include "SEData.h"
#include "ScoreManager.h"
#include "Note.h"
#include "ResultScene.h"
#include "MusicProgress.h"
#include "PauseLayer.h"
#include "SceneCreater.h"
#include "GradeManager.h"

enum SpritePriority{
	BackgroundPriority = 0,
	CharacterPriority,
	NotePriority,
	CirclePriority,
	NavigatorPriority,
	ValuePriority,
	TouchEffectPriority,
	BlackPriority,
	InfoPriority,
	InterfacePriority,
	ButtonPriority,
	PausePriority,
};

class Game : public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(Game);

	virtual void update(float delta);

	//�V�[�����n�܂������ɌĂяo����郁�\�b�h
	virtual void onEnter();
	//�V�[�����I��������ɌĂяo����郁�\�b�h
	virtual void onExit();

	//�����x���擾����
	virtual void didAccelerate(CCAcceleration* pAccel);
	//�V���O���^�b�`
	//virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	
	//�}���`�^�b�`
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	// ��ʃT�C�Y�̎擾
	static int screenWidth();
	static int screenHeight();

	//�|�[�Y�������ꂽ�Ƃ�
	void pauseMenuCallback(CCObject* pSender);
	//�|�[�Y���J�n���ꂽ�Ƃ�
	void pauseOpenedCallback();
	//�|�[�Y���������ꂽ�Ƃ�
	void pauseClosedCallback();

	//android �o�b�N�L�[
	void keyBackClicked();
	//android ���j���[�L�[
	void keyMenuClicked();
private:
	
	float mGameSecond;	//�o�ߎ���(�b)
	int mGameFrame;		//�o�ߎ���(�t���[��)
	PlayerCharacter* mPlayer;
	ScoreManager* mScoreManager;

	TouchManager* mTouchManager;

	//�Q�[�����J�n������
	bool mGameStarted;
	//�Q�[�����I��������
	bool mGameEnded;

	//�v���O���X
	MusicProgress* mMusicProgress;

	//���_�o�[
	CCSprite* mGradeBar;
	
	//�Q�[���O�C���t�H
	CCSprite* mTouchToStart;
	//�Ó]�p�w�i
	CCSprite* mBlackBack;

	//�X�e�[�W
	SSPlayer* mStagePlayer;
	SSPlayer* mStageEffect;

	//�|�[�Y���j���[�{�^��
	CCMenu* mMenu;

	//�|�[�Y����
	bool mPaused;

	//�|�[�Y���C���[�^�O
	static const int mPauseTagNum = 100; 

	//���_�Ǘ�
	GradeManager* mGradeManager;

	//�Ȃ̒���
	float mMusicTime;

	//�����x�̐��l
	float mAccel_x;
	float mAccel_y;
	float mAccel_z;

	//�V�F�C�N���ꂽ��
	bool mShaked;
	//�V�F�C�N�J�n����
	float mShakeStartTime;
	//1��̃V�F�C�N����
	float mOneShakeTime;

	//�V�F�C�N���ꂽ�����ׂ�
	bool checkShake();

	//�V�F�C�N�̊��x
	float mShakeLevel;

};

#endif // __GAME_SCENE_H__
