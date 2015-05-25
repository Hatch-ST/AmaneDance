#include "MotionData.h"

void MotionData::getMotionData(MusicName musicName,MotionData::PlayerMotion* motion){
	int index=0;
	switch(musicName){			
		case ORETA_TSUBASA:
		case ORETA_TSUBASA_OKE:
			motion[index] = WAIT; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = STEP_01; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = JUMP; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_01; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = STEP_02; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_01; ++index;			
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_01; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_02; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = STEP_01; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_01; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_02; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_01; ++index;			
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_01; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_02; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_01; ++index;			
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_01; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_02; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_01; ++index;			
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_01; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_02; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = DANCE_01; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = END;
			break;
		case PLASTIC_SKY:
		case PLASTIC_SKY_OKE:
			motion[index] = WAIT; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = STEP_01; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = JUMP; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_01; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = STEP_02; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_01; ++index;			
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_01; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_02; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = STEP_01; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_01; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_02; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_01; ++index;			
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_01; ++index;

			motion[index] = WAIT; ++index;
			motion[index] = DANCE_01; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_02; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_03; ++index;
			motion[index] = WALK_02; ++index;
			motion[index] = DANCE_04; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = DANCE_05; ++index;
			motion[index] = WAIT; ++index;
			motion[index] = POSE_02; ++index;

			motion[index] = END;
			break;
	}
}
