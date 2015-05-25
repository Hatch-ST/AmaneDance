#include "OptionLayer.h"

bool OptionLayer::init()
{
	srand((unsigned int)time(NULL));
	if ( !CCLayer::init() )
	{
		return false;
	}

	this->setTouchPriority(mOptionLayerTouchPriority);
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

	CCPoint screenSize = CCDirector::sharedDirector()->getWinSize();

	//フレーム
	mBackFrame = CCSprite::create("option_frame.png");
	mBackFrame->setColor(ccc3(100,100,100));
	mBackFrame->setOpacity(150);
	mBackFrame->setPosition( ccp(screenSize.x * 0.5f,screenSize.y * 0.5f) );
	this->addChild(mBackFrame, 1);

	//オプションデータを読み込む
	CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();

	float pos_y;
	//OPTIONラベル
	CCSprite* optionLabel = CCSprite::create("option_label.png");
	pos_y = screenSize.y - 25 - 30 - optionLabel->getContentSize().height * 0.5f;
	optionLabel->setPosition(ccp(screenSize.x * 0.5f,pos_y ) );
	this->addChild(optionLabel,2);

	//BGMラベル
	CCSprite* bgmLabel = CCSprite::create("bgm_label.png");
	pos_y += - optionLabel->getContentSize().height * 0.5f - 15 -  bgmLabel->getContentSize().height * 0.5f;
	bgmLabel->setPosition(ccp(screenSize.x * 0.5f, pos_y) );
	this->addChild(bgmLabel,2);

	//BGMスライダー
	mBgmSlider = CCControlSlider::create(
		"extensions/sliderTrack.png",
		"extensions/sliderProgress.png",
		"extensions/sliderThumb.png");
	mBgmSlider->setMinimumValue(0.0f);
	mBgmSlider->setMaximumValue(1.0f);
	mBgmSlider->setValue(userDefault->getFloatForKey("bgmVolume",0.8f));
	pos_y += - bgmLabel->getContentSize().height * 0.5f - 10 - mBgmSlider->getContentSize().height * 0.5f;
	mBgmSlider->setPosition( ccp(screenSize.x * 0.5f,pos_y) );
	mBgmSlider->setScale(2.0f);
	mBgmSlider->addTargetWithActionForControlEvents(
		this,
		cccontrol_selector(OptionLayer::sliderCallback),
		CCControlEventValueChanged);
	mBgmSlider->setTouchPriority(mOptionLayerTouchPriority);
	this->addChild(mBgmSlider,2);

	//SEラベル
	CCSprite* seLabel = CCSprite::create("se_label.png");
	pos_y += - mBgmSlider->getContentSize().height * 0.5f - 15 -  seLabel->getContentSize().height * 0.5f;
	seLabel->setPosition(ccp(screenSize.x * 0.5f, pos_y) );
	this->addChild(seLabel,2);

	//SEスライダー
	mSeSlider = CCControlSlider::create(
		"extensions/sliderTrack.png",
		"extensions/sliderProgress.png",
		"extensions/sliderThumb.png");
	mSeSlider->setMinimumValue(0.0f);
	mSeSlider->setMaximumValue(1.0f);
	mSeSlider->setValue(userDefault->getFloatForKey("seVolume",0.8f));
	pos_y += - seLabel->getContentSize().height * 0.5f - 10 - mSeSlider->getContentSize().height * 0.5f;
	mSeSlider->setPosition( ccp(screenSize.x * 0.5f,pos_y) );
	mSeSlider->setScale(2.0f);
	mSeSlider->addTargetWithActionForControlEvents(
		this,
		cccontrol_selector(OptionLayer::sliderCallback),
		CCControlEventValueChanged);
	mSeSlider->setTouchPriority(mOptionLayerTouchPriority);
	this->addChild(mSeSlider,2);

	//SHAKEラベル
	CCSprite* shakeLabel = CCSprite::create("shake_label.png");
	pos_y += - mSeSlider->getContentSize().height * 0.5f - 15 -  seLabel->getContentSize().height * 0.5f;
	shakeLabel->setPosition(ccp(screenSize.x * 0.5f, pos_y) );
	this->addChild(shakeLabel,2);

	//SHAKEスライダー
	mShakeSlider = CCControlSlider::create(
		"extensions/sliderTrack.png",
		"extensions/sliderProgress.png",
		"extensions/sliderThumb.png");
	mShakeSlider->setMinimumValue(2.0f);
	mShakeSlider->setMaximumValue(4.0f);
	mShakeSlider->setValue(userDefault->getFloatForKey("shakeLevel",3.0f));
	pos_y += - shakeLabel->getContentSize().height * 0.5f - 10 - mShakeSlider->getContentSize().height * 0.5f;
	mShakeSlider->setPosition( ccp(screenSize.x * 0.5f,pos_y) );
	mShakeSlider->setScale(2.0f);
	mShakeSlider->addTargetWithActionForControlEvents(
		this,
		cccontrol_selector(OptionLayer::sliderCallback),
		CCControlEventValueChanged);
	mShakeSlider->setTouchPriority(mOptionLayerTouchPriority);
	this->addChild(mShakeSlider,2);

	//NAVIGATORラベル
	CCSprite* navigatorLabel = CCSprite::create("navigator_label.png");
	pos_y += - mShakeSlider->getContentSize().height * 0.5f - 15 -  navigatorLabel->getContentSize().height * 0.5f;
	//pos_y += - 225 - 15 -  navigatorLabel->getContentSize().height * 0.5f;
	navigatorLabel->setPosition(ccp(screenSize.x * 0.5f, pos_y) );
	this->addChild(navigatorLabel,2);

	//NAVIスイッチ
	CCSprite* maskSprite = CCSprite::create("extensions/switch-mask.png");
	CCSprite* onSprite = CCSprite::create("extensions/switch-on.png");
	CCSprite* offSprite = CCSprite::create("extensions/switch-off.png");
	CCSprite* thumbSprite = CCSprite::create("extensions/switch-thumb.png");
	CCLabelTTF* onLabel = CCLabelTTF::create("ON","Arial-BoldMT",20);
	CCLabelTTF* offLabel = CCLabelTTF::create("OFF","Arial-BoldMT",20);
	mNaviSwitch	 = CCControlSwitch::create(
		maskSprite,onSprite,offSprite,thumbSprite,onLabel,offLabel);
	pos_y += - navigatorLabel->getContentSize().height * 0.5f - 10 - mNaviSwitch->getContentSize().height * 0.5f;
	mNaviSwitch->setPosition( ccp(screenSize.x * 0.5f,pos_y) );
	mNaviSwitch->setScale(2.0f);
	mNaviSwitch->addTargetWithActionForControlEvents(
		this,
		cccontrol_selector(OptionLayer::sliderCallback),
		CCControlEventValueChanged);
	mNaviSwitch->setTouchPriority(mOptionLayerTouchPriority);
	mNaviSwitch->setOn(userDefault->getBoolForKey("naviUsed",true));
	this->addChild(mNaviSwitch,2);

	//メニュー
	//OK
	CCSprite* okButton = CCSprite::create("ok_button.png");
	CCSprite* okButtonSelected = CCSprite::create("ok_button.png");
	okButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pOkItem = CCMenuItemSprite::create(
		okButton,
		okButtonSelected,
		this,
		menu_selector(OptionLayer::OKCallback));
	pOkItem->setPosition(ccp(screenSize.x * 0.5f - pOkItem->getContentSize().width * 0.5f -10, 25 + 30 + pOkItem->getContentSize().height * 0.5f));

	//キャンセル
	CCSprite* cancelButton = CCSprite::create("cancel_button.png");
	CCSprite* cancelButtonSelected = CCSprite::create("cancel_button.png");
	cancelButtonSelected->setColor(ccc3(100,100,100));
	CCMenuItemSprite *pCancelItem = CCMenuItemSprite::create(
		cancelButton,
		cancelButtonSelected,
		this,
		menu_selector(OptionLayer::cancelCallback));
	pCancelItem->setPosition(ccp(screenSize.x * 0.5f + pCancelItem->getContentSize().width * 0.5f +10,25 + 30 + pCancelItem->getContentSize().height * 0.5f));

	CCMenu* pMenu = CCMenu::create(pOkItem,pCancelItem, NULL);
	pMenu->setPosition(CCPointZero);
	pMenu->setTouchPriority(mOptionMenuTouchPriority);
	this->addChild(pMenu,2);

	return true;

}

void OptionLayer::sliderCallback(CCObject* object, CCControlEvent controlEvent){
	
}

//下レイヤーのタッチを無効化する
bool OptionLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

    return true;
     
}

//OKボタンが呼ばれたとき
void OptionLayer::OKCallback(CCObject* pSender){
	saveOptionData();
	Title* title = (Title*) mParent;
	title->closeOption();
	this->removeFromParentAndCleanup(true);
}
//キャンセルボタンが呼ばれたとき
void OptionLayer::cancelCallback(CCObject* pSender){
	Title* title = (Title*) mParent;
	title->closeOption();
	this->removeFromParentAndCleanup(true);
}

void OptionLayer::setParentLayer(CCLayer* parent){
	mParent = parent;
}

void OptionLayer::saveOptionData(){
	CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
	//BGMの音量
	float bgmVolume = mBgmSlider->getValue();
	userDefault->setFloatForKey("bgmVolume",bgmVolume);
	//SEの音量
	float seVolume = mSeSlider->getValue();
	userDefault->setFloatForKey("seVolume",seVolume);
	//シェイクの感度
	float shakeLevel  = mShakeSlider->getValue();
	userDefault->setFloatForKey("shakeLevel",shakeLevel);
	//ナビのオンオフ
	bool naviUsed = mNaviSwitch->isOn();
	userDefault->setBoolForKey("naviUsed",naviUsed);

	userDefault->flush();
}