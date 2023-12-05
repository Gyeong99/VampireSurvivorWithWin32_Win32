#include "monsterMove.h"

void MonsterMoveFun(strMonster* head , POINT localePosBmpCharacter , BITMAP localeInfoBmpCharacterRunning)
{
    if (head == NULL) {
        return;
    }
    strMonster* cur_node = head;
    POINT monsterMovePos[8] = { { 0,-3 }, { 2,-2 }, { 3,0 }, { 2,2 }, { 0,3 } , {-2,2} , {-3,0} , {-2,-2} };
    double radius;
    while (1) {

        radius = sqrt(pow((cur_node->monsterPos.x - localePosBmpCharacter.x), 2) + pow((cur_node->monsterPos.y - localePosBmpCharacter.y), 2));

        SetMonsterDirection(cur_node, radius, localePosBmpCharacter);


        if (cur_node->queueMove.empty() == false) {
            while (CheckCrashObstacleFun(cur_node,
                { cur_node->monsterPos.x + monsterMovePos[cur_node->queueMove.front()].x ,
                cur_node->monsterPos.y + monsterMovePos[cur_node->queueMove.front()].y } ,
                localeInfoBmpCharacterRunning) == TRUE)
            {
                cur_node->queueMove.pop();
                if (cur_node->queueMove.size() == 1) {

                    break;
                }
            }
            cur_node->monsterPos.x += monsterMovePos[cur_node->queueMove.front()].x;
            cur_node->monsterPos.y += monsterMovePos[cur_node->queueMove.front()].y;
            cur_node->queueMove = queue<int>();
        }
        cur_node = cur_node->next;
        if (cur_node == head) {
            break;
        }

    }
    return;
}

void SetMonsterDirection(strMonster* cur_node, double radius , POINT localePosBmpCharacter) {
    double bottomlineLen = double(localePosBmpCharacter.x) - cur_node->monsterPos.x;
    double cosAngle = bottomlineLen / radius;
    if (cur_node->monsterPos.y >= localePosBmpCharacter.y) {
        if (cosAngle <= 1 && cosAngle > 0.75) {
            SetWeightDirection(cur_node, 2, 0);
        }
        else if (cosAngle <= 0.75 && cosAngle > 0.60) {
            SetWeightDirection(cur_node, 1, 1);
        }
        else if (cosAngle <= 0.60 && cosAngle > 0.38) {
            SetWeightDirection(cur_node, 1, 0);
        }
        else if (cosAngle <= 0.38 && cosAngle > 0.0) {
            SetWeightDirection(cur_node, 0, 1);
        }
        else if (cosAngle <= 0.0 && cosAngle > -0.38) {
            SetWeightDirection(cur_node, 0, 0);
        }
        else if (cosAngle <= -0.38 && cosAngle > -0.70) {
            SetWeightDirection(cur_node, 7, 1);
        }
        else if (cosAngle <= -0.70 && cosAngle > -0.85) {
            SetWeightDirection(cur_node, 7, 0);
        }
        else if (cosAngle <= -0.85 && cosAngle >= -1.0) {
            SetWeightDirection(cur_node, 6, 1);
        }

    }
    else {
        if (cosAngle <= 1 && cosAngle > 0.75) {
            SetWeightDirection(cur_node, 2, 1);
        }
        else if (cosAngle <= 0.75 && cosAngle > 0.60) {
            SetWeightDirection(cur_node, 3, 0);
        }
        else if (cosAngle <= 0.60 && cosAngle > 0.44) {
            SetWeightDirection(cur_node, 3, 1);
        }
        else if (cosAngle <= 0.44 && cosAngle > 0.0) {
            SetWeightDirection(cur_node, 4, 0);
        }
        else if (cosAngle <= 0.0 && cosAngle > -0.44) {
            SetWeightDirection(cur_node, 4, 1);
        }
        else if (cosAngle <= -0.44 && cosAngle > -0.70) {
            SetWeightDirection(cur_node, 5, 0);
        }
        else if (cosAngle <= -0.70 && cosAngle > -0.85) {
            SetWeightDirection(cur_node, 5, 1);
        }
        else if (cosAngle <= -0.85 && cosAngle >= -1.0) {
            SetWeightDirection(cur_node, 6, 0);
        }
    }

    return;
}

void SetWeightDirection(strMonster* cur_node, int firstDirection, bool leftOrRight) {
    int saveFirstDirection = firstDirection;
    int i = 1;
    bool pmChange = true;
    cur_node->queueMove.push(firstDirection);
    if (leftOrRight == 0) {             // 벡터 기준 왼쪽에 있을 경우
        while (i <= 7) {
            if (pmChange == true) {
                firstDirection -= i;
            }
            else {
                firstDirection += i;
            }
            if (firstDirection < 0)
                firstDirection += 8;
            if (firstDirection > 7)
                firstDirection -= 8;
            cur_node->queueMove.push(firstDirection);
            pmChange = !pmChange;
            i += 1;
        }
    }
    else {                          // 벡터 기준 오른쪽에 있을 경우
        while (i <= 7) {
            if (pmChange == true) {
                firstDirection += i;
            }
            else {
                firstDirection -= i;
            }
            if (firstDirection < 0)
                firstDirection += 8;
            if (firstDirection > 7)
                firstDirection -= 8;
            cur_node->queueMove.push(firstDirection);
            pmChange = !pmChange;
            i += 1;
        }
    }

    return;

}

bool CheckCrashObstacleFun(strMonster* cur_node, POINT newMonsterMovePos, BITMAP localeInfoBmpCharacterRunning) {
    if (cur_node->next == cur_node)
        return 0;
    strMonster* checking_node = cur_node;
    double radius = (localeInfoBmpCharacterRunning.bmWidth / 2);
    double distance = 0;
    double afterdistance = 0;
    while (1) {
        cur_node = cur_node->next;

        distance = sqrt(pow(((checking_node->monsterPos.x + radius) -
            (cur_node->monsterPos.x + radius)), 2) +
            pow((checking_node->monsterPos.y + radius) -
                (cur_node->monsterPos.y + radius), 2));

        afterdistance = sqrt(pow(((newMonsterMovePos.x + radius) -
            (cur_node->monsterPos.x + radius)), 2) +
            pow((newMonsterMovePos.y + radius) -
                (cur_node->monsterPos.y + radius), 2));

        if (distance < radius) {
            if (afterdistance < distance) {
                return 1;           
            }
        }


        if (cur_node->next == checking_node) {
            break;
        }
    }
    return 0;
}