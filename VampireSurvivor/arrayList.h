#pragma once
#include <Windows.h>
#include<queue>

using namespace std;

struct strMonster
{
    strMonster* next;       //몬스터 연결리스트 다음 주소
    strMonster* prev;       //몬스터 연결리스트 이전 주소
    short int monsterCount;  // 몬스터 식별 번호 
    short int monsterCategory;      //몬스터 종류 번호 (무슨 비트맵을 사용하는가 , 어떤 움직임을 보이는가)
    short int monsterHp;        //몬스터 체력
    POINT monsterPos;          // 몬스터 좌표
    queue<int> queueMove;       // 몬스터 이동 방향 (8방향 POS 벡터)
    queue <short int> damagedSkillCount;    // 어떤 스킬에 맞았는가(스킬의 중복 데미지 제한)
};

struct strSkill
{
    strSkill* next;
    short int skillCount;
    short int skillCategory;
    short int skillDamage;
    short int skillSpeed;
    POINT skillPos;
    POINT skillDirection;
};

struct strExpCrystal
{
    strExpCrystal* next;
    short int ExpCrystalCount;
    short int ExpCrystalAmount;
    bool ExpCrystalInteraction;
    POINT ExpCrystalPos;
};

struct strText
{
    strText* next;
    short int textDamage;
    short int textCount;
    DWORD textTime;
    POINT textPos;
};




strMonster* strMonsterInit(strMonster* tmp);
strMonster* strMonsterInsert(strMonster* head, short int newMonsterCount, short int newMonsterCategory, POINT newMonsterPos);
strMonster* strMonsterRemove(strMonster* head, int index);

strSkill* strSkillInit(strSkill* tmp);
strSkill* strSkillInsert(strSkill* skillHead, strMonster* monsterHead, short int newSkillCount, short int newSkillCategory, short int newSkillDamage, short int newSkillSpeed, POINT newSkillPos);
void SetSkillDirection(strMonster* monster_head, strSkill* skill_newNode);
strSkill* strSkillRemove(strSkill* head, int index);

strExpCrystal* strExpCrystalInit(strExpCrystal* tmp);
strExpCrystal* strExpCrystalInsert(strExpCrystal* ExpCrystalHead , short int newExpCrystalCount , short int newExpCrystalAmount , POINT newExpCrystalPos);
strExpCrystal* strExpCrystalRemove(strExpCrystal* head , int index);

strText* strTextInit(strText* tmp);
strText* strTextInsert(strText* head, short int newTextDamage, short int newTextCount, POINT newTextPos , DWORD newTextTime);
strText* strTextRemove(strText* head, short int index);



