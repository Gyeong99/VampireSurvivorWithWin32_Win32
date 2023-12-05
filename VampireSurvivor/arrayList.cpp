#include "arrayList.h"
#include <vector>



strMonster* strMonsterInit(strMonster* tmp)
{
    tmp = new strMonster;
    tmp->prev = NULL;
    tmp->next = NULL;
    tmp->monsterCount = 0;
    tmp->monsterCategory = 0;
    tmp->monsterHp = 0;
    tmp->monsterPos = { 10,10 };
    return tmp;
}


strMonster* strMonsterInsert(strMonster* head, short int newMonsterCount, short int newMonsterCategory, POINT newMonsterPos)
{

    short int newMonsterHp = 0;
    switch (newMonsterCategory) {
    case 0:
        newMonsterHp = 30;
        break;
    }
        
    if (head == NULL) {
        head = strMonsterInit(head);
        head->monsterCount = newMonsterCount;
        head->monsterCategory = newMonsterCategory;
        head->monsterPos = newMonsterPos;
        head->monsterHp = newMonsterHp;
        head->next = head;
        head->prev = head;
        return head;
    }

    strMonster* last_node = head->prev;
    strMonster* new_node = NULL;
    new_node = strMonsterInit(new_node);

    last_node->next = new_node;
    new_node->prev = last_node;
    new_node->next = head;
    head->prev = new_node;
    new_node->monsterHp = newMonsterHp;
    new_node->monsterCount = newMonsterCount;
    new_node->monsterCategory = newMonsterCategory;
    new_node->monsterPos = newMonsterPos;


    return head;
}

strMonster* strMonsterRemove(strMonster* head, int index)
{
    strMonster* cur_node = head;
    
    if (cur_node->prev == head && cur_node->next == head) {
        delete cur_node;
        cur_node = 0;
        return NULL;
    }
    while (1) {
        if (cur_node->monsterCount == index)
            break;
        cur_node = cur_node->next;
    }

    cur_node->prev->next = cur_node->next;
    cur_node->next->prev = cur_node->prev;
    if (cur_node == head) {
        head = cur_node->next;
    }
    delete cur_node;
    cur_node = 0;
    return head;
}


strSkill* strSkillInit(strSkill* tmp)
{
    tmp = new strSkill;
    tmp->next = NULL;
    tmp->skillCount = 0;
    tmp->skillCategory = 0;
    tmp->skillDamage = 0;
    tmp->skillSpeed = 0;
    tmp->skillPos = { 10,10 };
    tmp->skillDirection = {0,3};
    return tmp;
}

strSkill* strSkillInsert(strSkill* skillHead, strMonster* monsterHead, short int newSkillCount, short int newSkillCategory, short int newSkillDamage, short int newSkillSpeed, POINT newSkillPos)
{
    if (skillHead == NULL) {
        skillHead = strSkillInit(skillHead);
        skillHead->skillCount = newSkillCount;
        skillHead->skillDamage = newSkillDamage;
        skillHead->skillSpeed = newSkillSpeed;
        skillHead->skillPos = newSkillPos;
        skillHead->next = NULL;
        SetSkillDirection(monsterHead, skillHead);
        return skillHead;
    }
    strSkill* new_node = NULL;
    strSkill* last_node = skillHead;
    new_node = strSkillInit(new_node);
    new_node->skillCount = newSkillCount;
    new_node->skillDamage = newSkillDamage;
    new_node->skillSpeed = newSkillSpeed;
    new_node->skillPos = newSkillPos;
    SetSkillDirection(monsterHead, new_node);
    while (last_node->next != NULL) {
        last_node = last_node->next;
    }
    last_node->next = new_node;
    return skillHead;
}

void SetSkillDirection(strMonster* monster_head, strSkill* skill_newNode) {
    if (skill_newNode == NULL) {
        return;
    }
    if (monster_head == NULL) {
        return;
    }
    double skillInclination = 0;
    strMonster* cur_monsterNode = monster_head;
    strSkill* cur_skillNode = skill_newNode;
    skillInclination = double((skill_newNode->skillPos.y - cur_monsterNode->monsterPos.y)) / double((skill_newNode->skillPos.x - cur_monsterNode->monsterPos.x));
    skillInclination = -skillInclination;
    if (skillInclination >= 2.5 || skillInclination <= -2.5) {
        if (cur_skillNode->skillPos.y > cur_monsterNode->monsterPos.y) {
            cur_skillNode->skillDirection = { 0,-3 };
        }
        else
            cur_skillNode->skillDirection = { 0,3 };
    }
    else if (skillInclination >= 0.5) {
        if (cur_skillNode->skillPos.y > cur_monsterNode->monsterPos.y) {
            cur_skillNode->skillDirection = { 2,-2 };
        }
        else
            cur_skillNode->skillDirection = { -2,2 };
    }
    else if (skillInclination >= -0.5 && skillInclination < 0.5) {
        if (cur_skillNode->skillPos.x > cur_monsterNode->monsterPos.x) {
            cur_skillNode->skillDirection = { -3,0 };
        }
        else
            cur_skillNode->skillDirection = { 3,0 };
    }
    else {
        if (cur_skillNode->skillPos.y > cur_monsterNode->monsterPos.y) {
            cur_skillNode->skillDirection = { -2,-2 };
        }
        else
            cur_skillNode->skillDirection = { 2,2 };
    }

    return;
}

strSkill* strSkillRemove(strSkill* head, int index)
{
    strSkill* cur_node = head;
    strSkill* previous_node = NULL;
    if (cur_node->next == NULL) {
        delete cur_node;
        cur_node = 0;
        return NULL;
    }
    if (cur_node->skillCount == index) {
        head = head->next;
        delete cur_node;
        cur_node = 0;
        return head;
    }
    while (1) {
        if (cur_node->skillCount == index)
            break;
        previous_node  = cur_node;
        if (cur_node->next == NULL)
            return head;
        cur_node = cur_node->next;
    }
    previous_node->next = cur_node->next;
    delete cur_node;
    cur_node = 0;
    return head;
}

strExpCrystal* strExpCrystalInit(strExpCrystal* tmp)
{
    tmp = new strExpCrystal;
    tmp->next = NULL;
    tmp->ExpCrystalAmount = 0;
    tmp->ExpCrystalCount = 0;
    tmp->ExpCrystalPos = {0,0};
    tmp->ExpCrystalInteraction = false;
    return tmp;
}

strExpCrystal* strExpCrystalInsert(strExpCrystal* ExpCrystalHead, short int newExpCrystalCount, short int newExpCrystalAmount, POINT newExpCrystalPos)
{
    if (ExpCrystalHead == NULL) {
        ExpCrystalHead = strExpCrystalInit(ExpCrystalHead);
        ExpCrystalHead->ExpCrystalCount = newExpCrystalCount;
        ExpCrystalHead->ExpCrystalAmount = newExpCrystalAmount;
        ExpCrystalHead->ExpCrystalPos = newExpCrystalPos;
        ExpCrystalHead->ExpCrystalInteraction = false;
        ExpCrystalHead->next = NULL;
        return ExpCrystalHead;
    }
    strExpCrystal* new_node = NULL;
    strExpCrystal* last_node = ExpCrystalHead;
    new_node = strExpCrystalInit(ExpCrystalHead);
    new_node->ExpCrystalCount = newExpCrystalCount;
    new_node->ExpCrystalAmount = newExpCrystalAmount;
    new_node->ExpCrystalPos = newExpCrystalPos;
    new_node->ExpCrystalInteraction = false;
    new_node->next = NULL;
    
    while (last_node->next != NULL) {
        last_node = last_node->next;
    }
    last_node->next = new_node;
    return ExpCrystalHead;
}

strExpCrystal* strExpCrystalRemove(strExpCrystal* head, int index)
{
    strExpCrystal* cur_node = head;
    strExpCrystal* previous_node = NULL;
    if (cur_node->next == NULL) {
        delete cur_node;
        cur_node = 0;
        return NULL;
    }
    if (cur_node->ExpCrystalCount == index) {
        head = head->next;
        delete cur_node;
        cur_node = 0;
        return head;
    }
    while (1) {
        if (cur_node->ExpCrystalCount == index)
            break;
        previous_node = cur_node;
        if (cur_node->next == NULL)
            return head;
        cur_node = cur_node->next;
    }
    previous_node->next = cur_node->next;
    delete cur_node;
    cur_node = 0;
    return head;
}

strText* strTextInit(strText* tmp)
{
    tmp = new strText;
    tmp->next = NULL;
    tmp->textDamage = 0;
    tmp->textCount = 0;
    tmp->textPos = { 0,0 };
    tmp->textTime = 0;
    return tmp;
}

strText* strTextInsert(strText* head, short int newTextDamage, short int newTextCount, POINT newTextPos , DWORD newTextTime)
{
    if (head == NULL) {
        head = strTextInit(head);
        head->next = NULL;
        head->textDamage = newTextDamage;
        head->textCount = newTextCount;
        head->textPos = newTextPos;
        head->textTime = newTextTime;
        return head;
    }
    strText* new_node = NULL;
    strText* last_node = head;
    new_node = strTextInit(new_node);
    new_node->textDamage = newTextDamage;
    new_node->textPos = newTextPos;
    new_node->textCount = newTextCount;
    new_node->textTime = newTextTime;
    while (last_node->next != NULL) {
        last_node = last_node->next;
    }
    last_node->next = new_node;
    return head;
}

strText* strTextRemove(strText* head, short int index)
{
    strText* cur_node = head;
    strText* previous_node = NULL;
    if (cur_node->next == NULL) {
        delete cur_node;
        cur_node = 0;
        return NULL;
    }
    if (cur_node->textCount == index) {
        head = head->next;
        delete cur_node;
        cur_node = 0;
        return head;
    }
    while (1) {
        if (cur_node->textCount == index)
            break;
        previous_node = cur_node;
        if (cur_node->next == NULL)
            return head;
        cur_node = cur_node->next;
    }
    previous_node->next = cur_node->next;
    delete cur_node;
    cur_node = 0;
    return head;
}