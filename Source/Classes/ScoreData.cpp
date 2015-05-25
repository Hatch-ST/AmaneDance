#include "ScoreData.h"

void ScoreData::getScoreData(MusicName musicName,ScoreData::NoteType* scoreData){
	int index=0;
	switch(musicName){
		case ORETA_TSUBASA:
		case ORETA_TSUBASA_OKE:
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = STEP_01_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_01_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = STEP_02_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_02_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = WALK_01_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_01_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = WALK_02_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_02_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = STEP_01_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_01_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = WALK_02_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = POSE_02_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = WALK_01_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_01_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = WALK_02_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_02_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = WALK_01_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_01_A; ++index;
			
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = POSE_02_A; ++index;

			scoreData[index] = END; ++index;
			break;
		case PLASTIC_SKY:
		case PLASTIC_SKY_OKE:
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = STEP_01_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_01_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = STEP_02_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_02_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = WALK_01_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_01_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = WALK_02_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_02_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = STEP_01_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = POSE_01_A; ++index;

			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE01_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE02_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE03_A; ++index;
			scoreData[index] = WALK_02_A; ++index;
			scoreData[index] = DANCE04_A; ++index;
			scoreData[index] = NOTHING; ++index;
			scoreData[index] = DANCE05_A; ++index;
			scoreData[index] = POSE_02_A; ++index;

			scoreData[index] = END; ++index;
			break;
	}
}
