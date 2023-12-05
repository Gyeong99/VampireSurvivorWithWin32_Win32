#pragma once
#include "arrayList.h"



POINT CharacterMoveFun(bool pKeyLayout[256], POINT posBmpBackGround, BITMAP infoBmpCharacterRunning, RECT rc, BITMAP infoBmpBackGround,
	strMonster* strMonsterList , strSkill* strSkillList , strExpCrystal* strExpCrystalList , strText* strTextList);
void MoveMonsterWithBackGroundFun(POINT pos, strMonster* head);
void MoveSkillWithBackGroundFun(POINT pos, strSkill* head);
void MoveExpCrystalWithBackGroundFun(POINT pos, strExpCrystal* head);
void MoveTextWithBackGroundFun(POINT pos, strText* head);