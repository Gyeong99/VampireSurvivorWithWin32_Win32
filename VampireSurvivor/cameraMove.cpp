#include "cameraMove.h"
#include "arrayList.h"



POINT CharacterMoveFun(bool pKeyLayOut[256], POINT posBmpBackGround, BITMAP infoBmpCharacterRunning, RECT rc, BITMAP infoBmpBackGround,
    strMonster* strMonsterList, strSkill* strSkillList, strExpCrystal* strExpCrystalList, strText* strTextList)
{
    POINT newPoint;
    if (pKeyLayOut[VK_LEFT] == pKeyLayOut[VK_RIGHT])
        newPoint.x = 0;
    else if (pKeyLayOut[VK_LEFT])
        newPoint.x = -7;
    else 
        newPoint.x = +7;


    if (pKeyLayOut[VK_UP] == pKeyLayOut[VK_DOWN])
        newPoint.y = 0;
    else if (pKeyLayOut[VK_UP]) {
        newPoint.y = -7;
        if (pKeyLayOut[VK_LEFT]) {
            newPoint.x = -5;
            newPoint.y = -5;
        }
        else if (pKeyLayOut[VK_RIGHT]) {
            newPoint.x = 5;
            newPoint.y = -5;
        }
    }    
    else {
        newPoint.y = +7;
        if (pKeyLayOut[VK_LEFT]) {
            newPoint.x = -5;
            newPoint.y = 5;
        }
        else if (pKeyLayOut[VK_RIGHT]) {
            newPoint.x = 5;
            newPoint.y = 5;
        }
    }
    if ((posBmpBackGround.x - newPoint.x) + (infoBmpCharacterRunning.bmWidth / 2) > 0)
        return posBmpBackGround;
    else if ((posBmpBackGround.y - newPoint.y) + infoBmpCharacterRunning.bmHeight > 10)
        return posBmpBackGround;
    else if ((posBmpBackGround.x - newPoint.x) - infoBmpCharacterRunning.bmWidth < ((rc.right - rc.left)) - infoBmpBackGround.bmWidth)
        return posBmpBackGround;
    else if ((posBmpBackGround.y - newPoint.y) - infoBmpCharacterRunning.bmHeight < ((rc.bottom - rc.top)) - infoBmpBackGround.bmHeight)
        return posBmpBackGround;
    else {
        posBmpBackGround.x -= newPoint.x;
        posBmpBackGround.y -= newPoint.y;
        MoveMonsterWithBackGroundFun(newPoint , strMonsterList);
        MoveSkillWithBackGroundFun(newPoint, strSkillList);
        MoveExpCrystalWithBackGroundFun(newPoint, strExpCrystalList);
        MoveTextWithBackGroundFun(newPoint, strTextList);
        return posBmpBackGround;
    }
}


void MoveMonsterWithBackGroundFun(POINT pos, strMonster* head)
{
    if (head == NULL) {
        return;
    }
    strMonster* cur_node = head;
    while (1) {
        cur_node->monsterPos.x -= pos.x;
        cur_node->monsterPos.y -= pos.y;
        cur_node = cur_node->next;
        if (cur_node == head) {
            break;
        }
    }
    return;
}

void MoveSkillWithBackGroundFun(POINT pos, strSkill* head)
{
    if (head == NULL) {
        return;
    }
    strSkill* cur_node = head;
    while (1) {
        cur_node->skillPos.x -= pos.x;
        cur_node->skillPos.y -= pos.y;
        cur_node = cur_node->next;
        if (cur_node == NULL) {
            break;
        }
    }
    return;
}

void MoveExpCrystalWithBackGroundFun(POINT pos, strExpCrystal* head)
{
    if (head == NULL) {
        return;
    }
    strExpCrystal* cur_node = head;
    while (1) {
        cur_node->ExpCrystalPos.x -= pos.x;
        cur_node->ExpCrystalPos.y -= pos.y;
        cur_node = cur_node->next;
        if (cur_node == NULL) {
            break;
        }
    }
    return;
}

void MoveTextWithBackGroundFun(POINT pos, strText* head_text)
{
    if (head_text == NULL) {
        return;
    }
    strText* cur_node = head_text;
    while (1) {
        cur_node->textPos.x -= pos.x;
        cur_node->textPos.y -= pos.y;
        cur_node = cur_node->next;
        if (cur_node == NULL) {
            break;
        }
    }
    return;
}
