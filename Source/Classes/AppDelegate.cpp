#include "AppDelegate.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "MusicData.h"
#include "SceneCreater.h"
#include "MusicSelectScene.h"

#define G_MUSIC_NAME

#ifdef G_MUSIC_NAME
MusicName gMusicName;
#else
extern MusicName gMusicName;
#endif

#define G_GRADE_DATA

#ifdef G_GRADE_DATA
struct GradeData gGradeData;
#else
extern struct GradeData gGradeData;
#endif

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

	//追加
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(720,1280,kResolutionShowAll);
	MusicData::preloadMusic();
	
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	CCScene *pScene = Title::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

//最小化されたとき
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCScene* scene = pDirector->getRunningScene();
	SceneCreater* sceneCreater = (SceneCreater*)scene;
	//ゲーム画面ならポーズをかける
	if(sceneCreater->getSceneName() == SceneCreater::GAME){
		Game* gameLayer = (Game*)scene->getChildByTag(0);
		gameLayer->pauseOpenedCallback();
	}


    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

//最小化から復帰したとき
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
