#ifndef __SCORE__DATA__H__
#define __SCORE__DATA__H__

#include "MusicData.h"
static const int SCORE_MAX_SIZE = 200;
class ScoreData{
	public:
		
		enum NoteType{
			DANCE01_A,
			DANCE02_A,
			DANCE03_A,
			DANCE04_A,
			DANCE05_A,
			POSE_01_A,
			POSE_02_A,
			STEP_01_A,
			STEP_02_A,
			WALK_01_A,
			WALK_02_A,
			WAIT_A,
			//テスト用
			TAP_TWIN,
			TAP_TRIPLE,
			TAP_DOUBLE,
			TAP_QUARTET,
			HOLD_SINGLE,
			HOLD_DOUBLE,
			HOLD_AND_TAP,
			NOTHING,
			END
		};

		static void getScoreData(MusicName musicName,NoteType* scoreData);
		
};


#endif //__SCORE__DATA__H__