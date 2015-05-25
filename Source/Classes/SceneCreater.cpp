#include "SceneCreater.h"

bool SceneCreater::init()
{
	srand((unsigned int)time(NULL));
	if ( !CCScene::init() )
	{
		return false;
	}

	return true;

}

void SceneCreater::setSceneName(SceneName sceneName){
	mSceneName = sceneName;
}

SceneCreater::SceneName SceneCreater::getSceneName(){
	return mSceneName;
}
