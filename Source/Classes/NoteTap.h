#ifndef __NOTE__TAP__H__
#define __NOTE__TAP__H__

#include "cocos2d.h" 
#include "Note.h"
#include "NoteTapSprite.h"
#include "SS5Player.h"
using namespace cocos2d;


class NoteTap:public Note{
	typedef Note base;
	public:
		NoteTap();
		NoteTap(CCPoint cood,int correctFrame);
		~NoteTap();
		void init();
     
		//音符座標
		int mPos_x;
		int mPos_y;

		//正解のフレーム
		int mCorrectFrame;

		//正解のフレームとのずれ
		int mDeltaFromCorrect;

		//タップをチェックするかのフラグ
		bool mCheckFlag;

		bool update(int currentFrame,float delta);

		//音符タイプを取得する
		base::NoteType getNoteType();

		//ナビを開始する
		void naviStart();

		//ナビが開始したか
		bool isNaviStarted();

		//タップを判定する
		base::Value checkTap(int touch_x,int touch_y);

		//シェイクを判定する
		base::Value checkShake();

		//ノート画像を登録する
		void setNoteTapSprite(NoteTapSprite* ntSprite);

		//評価画像を登録する
		void setValuePlayer(ss::Player* valuePlayer);

		//ナビを表示するかのフラグ
		bool mNaviFlag;

	private:
		//判定用円の半径
		static const int HitRadiusTap = 48;

		//フレームのずれによって評価を取得する
		Note::Value getValue();

		//登録した画像のポインタを持たせる
		NoteTapSprite* mNoteTapSprite;
		//ノートのアニメーションが終了したか
		bool mNoteAnimationEnded;

		//判定開始したか
		bool mCheckStarted;


		//評価画像
		ss::Player* mValuePlayer;
		//評価画像がアクティブか
		bool mValueActive;

};


#endif //__NOTE__TAP__H__