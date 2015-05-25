#ifndef __SCENE__CREATER__H__
#define __SCENE__CREATER__H__

#include "cocos2d.h"

using namespace cocos2d;

class SceneCreater:public CCScene{
	public:
		enum SceneName{
			TITLE,
			MUSIC_SELECT,
			GAME,
			RESULT
		};
		virtual bool init();
		CREATE_FUNC(SceneCreater);


		void setSceneName(SceneName sceneName);
		SceneName getSceneName();
	private:
		SceneName mSceneName;
};


#endif //__SCENE__CREATER__H__