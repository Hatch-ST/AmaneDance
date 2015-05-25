#ifndef __NAVIGATOR__H__
#define __NAVIGATOR__H__

#include "cocos2d.h" 
#include "Note.h"
#include "NoteTap.h"
#include "NoteTapSprite.h"
#include "NoteHold.h"
#include "NoteHoldSprite.h"

using namespace cocos2d;

class Navigator{
	public:
		Navigator(CCLayer* layer);
		~Navigator();
		void init(CCLayer* layer);

		//ナビ画像
		CCSprite* mNaviSprite;
		//ナビ画像（同時押しの２つ目）
		CCSprite* mNaviSpriteDouble;

		void setPosition(int x,int y, bool isDouble);

		void setNoteData(Note* note);

		int getNoteArrayCount();

		void update();
	private:
		//次の２つのノートを保持しておく
		CCArray* mNoteArray;

		//同時押しか
		bool mDouble;
		
		//ノートが更新されたか
		bool mNoteChanged;

		//ナビの描画フラグが立ったかチェックする
		bool checkNaviFlag();

		//ナビ画像を表示する
		void setDraw( bool isDouble);

		//同時押しか調べる
		bool checkDouble();

		//1つ目がホールドであるか
		bool mIsHoldFirst;
		//2つ目がホールドであるか
		bool mIsHoldSecond;

		//ホールド時の更新
		void updateHoldNavi(bool isDouble);

};


#endif //__NAVIGATOR__H__