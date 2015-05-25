#ifndef __MOTION__DATA__H__
#define __MOTION__DATA__H__

#include "MusicData.h"
static const int MOTION_MAX_SIZE = 200;
class MotionData{
	public:
		
		enum PlayerMotion{
			DANCE_01,
			DANCE_02,
			DANCE_03,
			DANCE_04,
			DANCE_05,
			POSE_01,
			POSE_02,
			STEP_01,
			STEP_02,
			WALK_01,
			WALK_02,
			JUMP,
			WAIT,
			DAMEGE_01,
			DAMEGE_02,
			DEF,
			END
		};

		static void getMotionData(MusicName musicName,PlayerMotion* motion);
		
};


#endif //__MOTION__DATA__H__