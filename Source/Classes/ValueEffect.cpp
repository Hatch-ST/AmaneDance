#include "ValueEffect.h"
#include "GameScene.h"

ValueEffect::ValueEffect(){
	init();
}


ValueEffect::~ValueEffect(){
	mValuePlayer->release();
}

void ValueEffect::init(){
	mValuePlayer = CCArray::create();
	for(int i=0;i<ArrayNumber;i++){
		ss::Player* player = ss::Player::create();
		player->setData("effect");
		player->retain();
		player->setVisible(false);
		mValuePlayer->addObject(player);
	}
	mValuePlayer->retain();

}

//プレイヤーをレイヤーに登録する
void ValueEffect::setPlayer(CCLayer *layer){
	for(int i=0;i<mValuePlayer->count();i++){
		ss::Player* player = dynamic_cast<ss::Player*>( mValuePlayer->objectAtIndex(i) );
		layer->addChild(player,ValuePriority);
	}
}

//評価ごとに空いているプレイヤーを渡す
ss::Player* ValueEffect::getValuePlayer(Note::Value value){
	//アクティブでないプレイヤーを検索
	for(int i=0;mValuePlayer->count();i++){
		ss::Player* player = dynamic_cast<ss::Player*>( mValuePlayer->objectAtIndex(i) );
		//if(!player->){
			switch(value){		
				case Note::COOL:
					player->play("evaluation_cool_re/anime_1");
					break;
				case Note::GREAT:
					player->play("evaluation_great_re/anime_1");
					break;
				case Note::GOOD:
					player->play("evaluation_good_re/anime_1");
					break;
				case Note::BAD:	
					player->play("evaluation_bad_re/anime_1");
					break;
				case Note::MISS:
					//Missのデータが破損したためBadで代用
					player->play("evaluation_bad_re/anime_1");
					break;
			}
			player->setLoop(1);
			player->setVisible(true);
			return player;

		//}
	}
	return NULL;
}

void ValueEffect::pause(){
	for(int i=0;i<ArrayNumber;i++){
		dynamic_cast<ss::Player*>( mValuePlayer->objectAtIndex(i) )->pause();
	}
}

void ValueEffect::resume(){
	for(int i=0;i<ArrayNumber;i++){
		dynamic_cast<ss::Player*>( mValuePlayer->objectAtIndex(i) )->resume();
	}
}