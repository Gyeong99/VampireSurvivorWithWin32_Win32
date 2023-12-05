#pragma once
#include "arrayList.h"

void MonsterMoveFun(strMonster* head, POINT localePosBmpCharacter, BITMAP localeInfoBmpCharacterRunning);
bool CheckCrashObstacleFun(strMonster* cur_node, POINT newMonsterMovePos , BITMAP localeInfoBmpCharacterRunning);
void SetMonsterDirection(strMonster* tmp, double radius , POINT localePosBmpCharacter);
void SetWeightDirection(strMonster* cur_node, int firstDirection, bool leftOrRight);