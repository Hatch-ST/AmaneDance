#ifndef __PAUSE__LAYER__H__
#define __PAUSE__LAYER__H__

#include "cocos2d.h"
#include "GameScene.h"
#include "TitleScene.h"

using namespace cocos2d;

class PauseLayer:public CCLayer{
	public:
		virtual bool init();
		CREATE_FUNC(PauseLayer);

		virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

		void setParentLayer(CCLayer* parent);
		
	private:
		//���C���[�̗D�揇��
		static const int mPauseLayerTouchPriority = kCCMenuHandlerPriority - 1;
		//�|�[�Y���j���[�{�^���̗D�揇��
		static const int mPauseMenuTouchPriority = kCCMenuHandlerPriority - 2;
		//�|�[�Y���C���t�H
		CCSprite* mPauseInfo;
		//�Ó]�p�w�i
		CCSprite* mBlackBack;

		//�߂�{�^�����Ă΂ꂽ�Ƃ�
		void pauseReturnCallback(CCObject* pSender);
		//�ŏ�����{�^�����Ă΂ꂽ�Ƃ�
		void pauseRestartCallback(CCObject* pSender);
		//��߂�{�^�����Ă΂ꂽ�Ƃ�
		void pauseQuitCallback(CCObject* pSender);

		CCLayer* mParent;
};


#endif //__PAUSE__LAYER__H__