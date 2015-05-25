LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
				   ../../Classes/BestGrade.cpp \
				   ../../Classes/GameScene.cpp \
				   ../../Classes/GradeManager.cpp \
				   ../../Classes/MotionData.cpp \
				   ../../Classes/MusicData.cpp \
				   ../../Classes/MusicProgress.cpp \
				   ../../Classes/MusicSelectScene.cpp \
				   ../../Classes/Navigator.cpp \
				   ../../Classes/Note.cpp \
				   ../../Classes/NoteHold.cpp \
				   ../../Classes/NoteHoldSprite.cpp \
				   ../../Classes/NoteTap.cpp \
				   ../../Classes/NoteTapSprite.cpp \
				   ../../Classes/NumberSprite.cpp \
				   ../../Classes/OnePhraseScore.cpp \
				   ../../Classes/OptionLayer.cpp \
				   ../../Classes/PauseLayer.cpp \
				   ../../Classes/PlayerCharacter.cpp \
				   ../../Classes/ResultGrade.cpp \
				   ../../Classes/ResultScene.cpp \
				   ../../Classes/SceneCreater.cpp \
				   ../../Classes/ScoreData.cpp \
				   ../../Classes/ScoreManager.cpp \
				   ../../Classes/SEData.cpp \
   				   ../../Classes/SSPlayer.cpp \
				   ../../Classes/SS5Player.cpp \
				   ../../Classes/TitleScene.cpp \
				   ../../Classes/Touch.cpp \
				   ../../Classes/TouchManager.cpp \
				   ../../Classes/TouchEffect.cpp \
				   ../../Classes/ValueEffect.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
