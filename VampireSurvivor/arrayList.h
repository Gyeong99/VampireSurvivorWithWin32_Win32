#pragma once
#include <Windows.h>
#include<queue>

using namespace std;

struct strMonster
{
    strMonster* next;       //���� ���Ḯ��Ʈ ���� �ּ�
    strMonster* prev;       //���� ���Ḯ��Ʈ ���� �ּ�
    short int monsterCount;  // ���� �ĺ� ��ȣ 
    short int monsterCategory;      //���� ���� ��ȣ (���� ��Ʈ���� ����ϴ°� , � �������� ���̴°�)
    short int monsterHp;        //���� ü��
    POINT monsterPos;          // ���� ��ǥ
    queue<int> queueMove;       // ���� �̵� ���� (8���� POS ����)
    queue <short int> damagedSkillCount;    // � ��ų�� �¾Ҵ°�(��ų�� �ߺ� ������ ����)
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



