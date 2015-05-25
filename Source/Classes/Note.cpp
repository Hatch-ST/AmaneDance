#include "Note.h"

Note::NoteType Note::getNoteType(){
	return mNoteType;
}

bool Note::isNaviStarted(){
	return mNaviStarted;
}

void Note::naviStart(){
	mNaviStarted = true;
}