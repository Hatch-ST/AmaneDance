#ifndef __OPTION__LAYER__H__
#define __OPTION__LAYER__H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TitleScene.h"

using namespace cocos2d;
USING_NS_CC_EXT;

class OptionLayer:public CCLayer{
	public:
		virtual bool init();
		CREATE_FUNC(OptionLayer);

		virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

		void setParentLayer(CCLayer* parent);
		
	private:
		//���C���[�̗D�揇��
		static const int mOptionLayerTouchPriority = kCCMenuHandlerPriority - 1;
		//�I�v�V�����{�^���̗D�揇��
		static const int mOptionMenuTouchPriority = kCCMenuHandlerPriority - 2;
		//�w�i�̃t���[��
		CCSprite* mBackFrame;

		//OK�{�^�����Ă΂ꂽ�Ƃ�
		void OKCallback(CCObject* pSender);
		//�L�����Z���{�^�����Ă΂ꂽ�Ƃ�
		void cancelCallback(CCObject* pSender);

		CCLayer* mParent;

		void sliderCallback(CCObject* object, CCControlEvent controlEvent);

		//�I�v�V�����f�[�^��ۑ�����
		void saveOptionData();

		//GUI
		CCControlSlider* mBgmSlider;
		CCControlSlider* mSeSlider;
		CCControlSlider* mShakeSlider;
		CCControlSwitch* mNaviSwitch;
};


#endif //__OPTION__LAYER__H__