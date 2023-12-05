#include "arrayList.h"
#include "cameraMove.h"
#include "monsterMove.h"
#include <random>
#include "resource.h"
#include "moveExpCrystal.h"
#include "GameSoundManager.h"
#include <cmath>
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;
#define TIMER_MOBCREATE 1 
#define PI 3.14


struct strCharacter {
    short int characterLife;
    short int characterMonsterKill;
    short int characterGatherCrystal;
    bool characterSkill[3];
    DWORD characterInvincibleTime;
};

struct strSavedSkillCategory {
    short int SavedskillDamage;
    short int SavedskillSpeed;
};

strMonster* strMonsterList = NULL;
strSkill* strSkillList = NULL;
strText* strTextList = NULL;
strExpCrystal* strExpCrystalList = NULL;

strCharacter mainCharacter = {50,0 ,0, true , 0};
strSavedSkillCategory skill_0 = {10,3};


static HBITMAP bmpBackGround;
static HBITMAP bmpCharacterRunning;
static HBITMAP bmpMonster_0;
static HBITMAP bmpSkill_0;
static HBITMAP bmpExpCrystal;
static BITMAP InfoBmpSkill_0;
static BITMAP infoBmpBackGround;
static BITMAP infoBmpCharacterRunning;
static POINT posBmpBackGround;
static POINT posBmpCharacter;
static RECT rc;

static short int monsterCount = 0;
static short int ExpCrystalCount = 0;
static short int monsterMaxCount = 0;
static int monsterKill = 0;
static bool keyLayout[256];
static BOOL bGameProgress = false;




void MoveAttackFun(strSkill* head);
void CheckMonsterAttacked(strMonster* head_monster, strSkill* head_skill ,DWORD* pDwTime);
void DamageMonster_Skill(strMonster* cur_monsterNode, short int damage , DWORD* pDwTime);
strMonster* CheckMonsterDeath(strMonster* head_monster);
void CharacterAttackFun(int* pToggleCharAttack);
void CharacterAttack_Num0();
void CheckTextTime(strText* head_text, DWORD* pDwTime);
void CheckCharacterDamaged(strMonster* head_monster, DWORD* pDwTime);
void ReturnToStartFun();

void strMonsterDestroyNode(strMonster* head)
{
    strMonster* destroy = head -> next;
    if (destroy == head) {
        delete destroy;
        destroy = 0;
        return;
    }
    strMonster* temp = destroy;
    while (destroy != NULL) {
        temp = temp->next;
        delete destroy;
        destroy = 0;
        destroy = temp;
        if (destroy == head) {
            delete destroy;
            destroy = 0;
            return;
        }
    }
}



WNDCLASS wc = { };

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
 {
    const wchar_t CLASS_NAME[] = L"VampireSurvivor";

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"VampireSurvivor",
        WS_OVERLAPPEDWINDOW,


        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,

        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    
    
    GetClientRect(hwnd, &rc);

    
    ShowWindow(hwnd, nCmdShow);

    bool peekbool = false;
    MSG msg = { };
    DWORD dwTime = GetTickCount64();
    DWORD* pDwTime = &dwTime;
    int toggleCharAttack = 0;
    int* pToggleCharAttack = &toggleCharAttack;
    
    int halfFrame = 0;

    while (!peekbool)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                peekbool = true;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        if (dwTime + 32 < GetTickCount64() && bGameProgress)
        {
            dwTime = GetTickCount64();
            halfFrame++;
            posBmpBackGround = CharacterMoveFun(keyLayout , posBmpBackGround , infoBmpCharacterRunning ,
                rc , infoBmpBackGround , strMonsterList , strSkillList , strExpCrystalList, strTextList);
            MonsterMoveFun(strMonsterList , posBmpCharacter , infoBmpCharacterRunning);
            CharacterAttackFun(pToggleCharAttack);
            MoveAttackFun(strSkillList);
            CheckMonsterAttacked(strMonsterList,strSkillList,pDwTime);
            strMonsterList = CheckMonsterDeath(strMonsterList);
            CheckTextTime(strTextList , pDwTime);
            CheckCharacterDamaged(strMonsterList , pDwTime);
            strExpCrystalList = CheckExpCrystal_InteractionFun(strExpCrystalList , posBmpCharacter ,
                infoBmpCharacterRunning);
            InvalidateRect(hwnd, NULL, TRUE);
        }

        if (mainCharacter.characterLife <= 0) {
            bGameProgress = false;
            if (MessageBox(hwnd, L"GameOver \n(재시도를 원할 시 OK버튼)", L"GameOver", MB_YESNO) == IDYES) {
                ReturnToStartFun();
                bGameProgress = true;
            }
            else {
                PostQuitMessage(0);
            }
            InvalidateRect(hwnd, NULL,TRUE);
        }

    }


    return 0;
}

void ReturnToStartFun() {
    if (strMonsterList == NULL) {
        return;
    }
    posBmpBackGround = { -450,-300 };
    strMonsterDestroyNode(strMonsterList);
    mainCharacter = { 50,0 , 0 , true , 0 };
    skill_0 = { 10,3 };
    strMonsterList = NULL;
    strSkillList = NULL;
    strExpCrystalList = NULL;
}

void CheckCharacterDamaged(strMonster* head_monster , DWORD* pDwTime) {
    if (head_monster == NULL) {
        return;
    }
    
    RECT rectMonster, rectCharacter, rectTmp;
    rectCharacter = { posBmpCharacter.x , posBmpCharacter.y , posBmpCharacter.x + infoBmpCharacterRunning.bmWidth , posBmpCharacter.y + infoBmpCharacterRunning.bmHeight };
    strMonster* cur_monsterNode = head_monster;
        while (1) {
            rectMonster = { cur_monsterNode->monsterPos.x , cur_monsterNode->monsterPos.y , cur_monsterNode->monsterPos.x + infoBmpCharacterRunning.bmWidth - 10 , cur_monsterNode->monsterPos.y + infoBmpCharacterRunning.bmHeight - 10 };
            if (IntersectRect(&rectTmp, &rectMonster, &rectCharacter)) {
                if (mainCharacter.characterInvincibleTime + 100 < *pDwTime) {
                    mainCharacter.characterInvincibleTime = *pDwTime;
                    mainCharacter.characterLife -= 1;
                }
             }
            cur_monsterNode = cur_monsterNode->next;
            if (cur_monsterNode == head_monster) {
                break;
            }

        }
    return;
}

void CheckTextTime(strText* head_text , DWORD* pDwTime) {
    if (head_text == NULL) {
        return;
    }
    strText* cur_node = head_text;
    while (1) {
        if (cur_node->textTime + 1000 < *pDwTime) {
            strTextList = strTextRemove(cur_node, cur_node->textCount);
            cur_node = strTextList;
            if (cur_node == NULL) {
                break;
            }
        }
        cur_node = cur_node->next;
        if (cur_node == NULL) {
            break;
        }
    }
    return;
}

strMonster* CheckMonsterDeath(strMonster* head_monster) {
    if (head_monster == NULL) {
        return NULL;
    }
    strMonster* cur_node = head_monster;
    strMonster* next_node = NULL;
    while (1) {
        if (cur_node->monsterHp <= 0) {
            next_node = cur_node->next;
            mainCharacter.characterMonsterKill++; 
            strExpCrystalList = strExpCrystalInsert(strExpCrystalList, ExpCrystalCount, 10, cur_node->monsterPos);
            ExpCrystalCount++;
            head_monster = strMonsterRemove(head_monster, cur_node->monsterCount);
            monsterMaxCount--;
            
            
        }
        if (head_monster == NULL) {
            return NULL;
        }
        
        if (next_node == NULL) {
            cur_node = cur_node->next;
        }
        else
            cur_node = next_node;

        if (cur_node == head_monster) {
            break;
        }
        next_node = NULL;
    }
    return head_monster;;
}

void CheckMonsterAttacked(strMonster* head_monster, strSkill* head_skill, DWORD* pDwTime) {
    if (head_monster == NULL) {
        return;
    }
    if (head_skill == NULL) {
        return;
    }
    queue<short int> queueSkillSave;
    short int i = -1;
    bool bpass = 0;
    RECT rectMonster, rectSkill, rectTmp;

    strMonster* cur_monsterNode = head_monster;
    strSkill* cur_skillNode = head_skill;
    while (1) {
        rectSkill = { cur_skillNode->skillPos.x , cur_skillNode->skillPos.y , cur_skillNode->skillPos.x + infoBmpCharacterRunning.bmWidth , cur_skillNode->skillPos.y + infoBmpCharacterRunning.bmHeight };
        while (1) {
            rectMonster = { cur_monsterNode->monsterPos.x , cur_monsterNode->monsterPos.y , cur_monsterNode->monsterPos.x + infoBmpCharacterRunning.bmWidth , cur_monsterNode->monsterPos.y + infoBmpCharacterRunning.bmHeight };
            while (IntersectRect(&rectTmp, &rectMonster, &rectSkill)) {
                bpass = 0;
                if (cur_monsterNode->damagedSkillCount.empty() == true) {
                    DamageMonster_Skill(cur_monsterNode , cur_skillNode->skillDamage , pDwTime);
                    cur_monsterNode->damagedSkillCount.push(cur_skillNode->skillCount);
                    break;
                }
                while (cur_monsterNode->damagedSkillCount.empty() == false) {
                    i = cur_monsterNode->damagedSkillCount.front();
                    queueSkillSave.push(i);
                    cur_monsterNode->damagedSkillCount.pop();
                    if (i == cur_skillNode->skillCount) {
                        bpass = 1;
                        break;
                    }
                    
                }
                if (bpass) {
                    bpass = 0;
                    break;
                }
                if (cur_monsterNode->damagedSkillCount.empty() == true) {
                    DamageMonster_Skill(cur_monsterNode, cur_skillNode->skillDamage , pDwTime);
                    cur_monsterNode->damagedSkillCount.push(cur_skillNode->skillCount);
                    break;
                }
            }

            while (queueSkillSave.empty() == false) {
                cur_monsterNode->damagedSkillCount.push(queueSkillSave.front());
                queueSkillSave.pop();
            }
            cur_monsterNode = cur_monsterNode->next;
            if (cur_monsterNode == head_monster) {
                break;
            }

        }
        cur_skillNode = cur_skillNode->next;
        if (cur_skillNode == NULL) {
            break;
        }
    }
    

    return;
}

strExpCrystal* CheckExpCrystal_InteractionFun(strExpCrystal* strCrystalHead, POINT CharacterPos, BITMAP localeInfoBmpCharacterRunning) {
    if (strCrystalHead == NULL)
        return 0;
    strExpCrystal* return_head = strCrystalHead;
    strExpCrystal* cur_node = strCrystalHead;
    int halfBmWidth = localeInfoBmpCharacterRunning.bmWidth / 2;
    int halfBmHeight = localeInfoBmpCharacterRunning.bmHeight / 2;
    int crystalRemoveNum = 0;
    double radius = 30.0;
    double distance = 0.0;
    while (1) {
        distance = sqrt(pow((CharacterPos.x + halfBmWidth) - (cur_node->ExpCrystalPos.x + halfBmWidth), 2) + pow((CharacterPos.y + halfBmWidth) - (cur_node->ExpCrystalPos.y + halfBmHeight), 2));

        if (distance < radius) {
            cur_node->ExpCrystalInteraction = true;
            if (cur_node->ExpCrystalInteraction == true) {
                crystalRemoveNum = cur_node->ExpCrystalCount;
                cur_node = cur_node->next;
                return_head = strExpCrystalRemove(return_head, crystalRemoveNum);
                mainCharacter.characterGatherCrystal++;
                if((mainCharacter.characterGatherCrystal % 2 == 0) && (mainCharacter.characterLife < 50))
                    mainCharacter.characterLife++;
                if (cur_node == NULL) {
                    break;
                }
                continue;
            }

        }
        if (cur_node->next == NULL) {
            break;
        }
        cur_node = cur_node->next;
    }
    return return_head;
}


void CharacterAttackFun(int* pToggleCharAttack) {
    *pToggleCharAttack += 1;
    if (*pToggleCharAttack % 30 == 0 && mainCharacter.characterSkill[0] == true) {
        CharacterAttack_Num0();


    }
}

void CharacterAttack_Num0() {
    static int skillCount = 0;
    //PlaySound(TEXT(SOUND_FIRESOUND), wc.hInstance, SND_ASYNC);
    //PlaySoundFile(L"C:\\SDE_FireSound.wav");


    strSkillList = strSkillInsert(strSkillList, strMonsterList, skillCount, 0, skill_0.SavedskillDamage, skill_0.SavedskillSpeed, { posBmpCharacter.x + 6 , posBmpCharacter.y + 12 });
    skillCount += 1;
}

 

void DamageMonster_Skill(strMonster* cur_monsterNode , short int damage , DWORD* pDwTime) {
    static int staticTextCount = 0;
    cur_monsterNode->monsterHp -= damage;
    strTextList = strTextInsert(strTextList, damage, staticTextCount, cur_monsterNode->monsterPos, *pDwTime);
    staticTextCount++;
    PlaySound(TEXT("SDE_HitSound.wav"), wc.hInstance, SND_ASYNC);
}

void MoveAttackFun(strSkill* head) {
    if (head == NULL) {
        return;
    }
    strSkill* cur_node = head;
    while (1) {
        if ((cur_node->skillPos.x >= (rc.right - rc.left)) || ((cur_node->skillPos.x + InfoBmpSkill_0.bmWidth) <= 0) || cur_node->skillPos.y >= (rc.bottom - rc.top) || (cur_node->skillPos.y + InfoBmpSkill_0.bmHeight) <= 0) {
            strSkill* next_node = cur_node->next;
            strSkillList = strSkillRemove(strSkillList, cur_node->skillCount);
            if (next_node == NULL) {
                break;
            }
            else {
                cur_node = next_node;
                continue;
            }
        }
        cur_node->skillPos.x += cur_node->skillSpeed * cur_node->skillDirection.x;
        cur_node->skillPos.y += cur_node->skillSpeed * cur_node->skillDirection.y;
        cur_node = cur_node->next;
        if (cur_node == NULL) {
            break;
        }
    }
    return;
}


void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
    HDC MemDC;
    HBITMAP OldBitmap;
    int bx, by;
    BITMAP bit;

    MemDC = CreateCompatibleDC(hdc);
    OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

    GetObject(hBit, sizeof(BITMAP), &bit);
    bx = bit.bmWidth;
    by = bit.bmHeight;

    TransparentBlt(hdc, x, y, bx, by, MemDC, 0, 0, bx, by, RGB(0, 255, 0));
    

    SelectObject(MemDC, OldBitmap);
    DeleteDC(MemDC);
}

void DrawStrMonster(HDC hdc, strMonster* head) {
    if (head == NULL) {
        return;
    }
    strMonster* cur_node = head;
    while (1) {
        switch (cur_node->monsterCategory) {
        case 0: {
            DrawBitmap(hdc, cur_node->monsterPos.x, cur_node->monsterPos.y, bmpMonster_0);
            break;
        }
        }
        cur_node = cur_node->next;
        if (cur_node == head) {
            break;
        }
    }
    return;
}

void DrawStrSkill(HDC hdc, strSkill* head) {
    if (head == NULL) {
        return;
    }
    strSkill* cur_node = head;
    while (1) {
        switch (cur_node->skillCategory) {
        case 0: {
            DrawBitmap(hdc, cur_node->skillPos.x, cur_node->skillPos.y, bmpSkill_0);
            break;
        }
        }
        cur_node = cur_node->next;
        if (cur_node == NULL) {
            break;
        }
    }
    return;
}

void DrawStrExpCrystal(HDC hdc, strExpCrystal* head) {
    if (head == NULL) {
        return;
    }
    strExpCrystal* cur_node = head;
    while (1) {
        DrawBitmap(hdc, cur_node->ExpCrystalPos.x, cur_node->ExpCrystalPos.y, bmpExpCrystal);
        cur_node = cur_node->next;
        if (cur_node == NULL) {
            break;
        }
    }
    return;
}

void DrawStrText(HDC hdc, strText* head_text) {
    TCHAR lpOut[100];
    if (head_text == NULL) {
        return;
    }
    strText* cur_node = head_text;
    while (1) {
        wsprintf(lpOut, TEXT("%d"),cur_node->textDamage);
        TextOut(hdc, cur_node->textPos.x, cur_node->textPos.y, lpOut, lstrlen(lpOut));
        cur_node = cur_node->next;
        if (cur_node == NULL) {
            break;
        }
    }
    return;
}

void DrawStatusText(HDC hdc) {
    TCHAR lpOut[100];
    wsprintf(lpOut, TEXT("CharacterLife : %d"), mainCharacter.characterLife);
    TextOut(hdc, rc.right - rc.left - 150, 30, lpOut, lstrlen(lpOut));
    wsprintf(lpOut, TEXT("Score : %d"), mainCharacter.characterGatherCrystal);
    TextOut(hdc, rc.right - rc.left - 150, 60, lpOut, lstrlen(lpOut));
    return;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    
    PAINTSTRUCT ps;
    HWND btn1_button , btn2_button;
    int timerFrame = 0;
    switch (uMsg)
    {
    case WM_CREATE:
    {
        btn1_button = CreateWindow(L"button", L"리셋", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 100, 25, hwnd, (HMENU)0x1001, wc.hInstance, NULL);
        btn2_button = CreateWindow(L"button", L"일시정지", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 50, 100, 25, hwnd, (HMENU)0x1002, wc.hInstance, NULL);
        bmpBackGround = LoadBitmap(wc.hInstance, MAKEINTRESOURCE(IDB_BMPBACKGROUND2));
        bmpCharacterRunning = LoadBitmap(wc.hInstance, MAKEINTRESOURCE(IDB_BMPCHARACTER1));
        bmpMonster_0 = LoadBitmap(wc.hInstance, MAKEINTRESOURCE(IDB_BMPMONSTER1));
        bmpSkill_0 = LoadBitmap(wc.hInstance, MAKEINTRESOURCE(IDB_BMPSKILL0));
        bmpExpCrystal = LoadBitmap(wc.hInstance, MAKEINTRESOURCE(IDB_BMPEXPCRYSTAL));
        GetObject(bmpBackGround , sizeof(BITMAP), &infoBmpBackGround);
        GetObject(bmpCharacterRunning, sizeof(BITMAP), &infoBmpCharacterRunning);
        GetObject(bmpSkill_0, sizeof(BITMAP), &InfoBmpSkill_0);
        posBmpBackGround = {-450,-300};
        posBmpCharacter = {300,200 };
        SetTimer(hwnd, TIMER_MOBCREATE, 250, NULL);
        break;
    }
    case WM_DESTROY:
    {
        KillTimer(hwnd,TIMER_MOBCREATE);
        strMonsterDestroyNode(strMonsterList);
        PostQuitMessage(0);
        break;
    }
        
        
    case WM_PAINT:
    {
        static HDC hdc, MemDC;
        static HBITMAP BackBit, oldBackBit;
        static RECT bufferRT;
        MemDC = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &bufferRT);
        hdc = CreateCompatibleDC(MemDC);
        BackBit = CreateCompatibleBitmap(MemDC, bufferRT.right, bufferRT.bottom);
        oldBackBit = (HBITMAP)SelectObject(hdc, BackBit);
        PatBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);
        DrawBitmap(hdc, posBmpBackGround.x, posBmpBackGround.y, bmpBackGround);
        DrawBitmap(hdc, posBmpCharacter.x, posBmpCharacter.y, bmpCharacterRunning);
        DrawStrMonster(hdc, strMonsterList);
        DrawStrSkill(hdc, strSkillList);
        DrawStrText(hdc, strTextList);
        DrawStrExpCrystal(hdc, strExpCrystalList);
        DrawStatusText(hdc);
        GetClientRect(hwnd, &bufferRT);
        BitBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, hdc, 0, 0, SRCCOPY);
        SelectObject(hdc, oldBackBit);
        DeleteObject(BackBit);
        DeleteDC(hdc);
        EndPaint(hwnd, &ps);

        
        break;
    }

    case WM_TIMER:{
        switch (wParam) {
        case TIMER_MOBCREATE: {
            
            short int monsterCategory = 0;
            POINT monsterPos = { 0,0 };
            static std::random_device rd;               // 랜덤 디바이스 시드값 얻는 함수 (시간이 많이 걸리는편이라 static 선언함)
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> disDirection(0, 3);
            std::uniform_int_distribution<int> disPosx(-20,(rc.right - rc.left));
            std::uniform_int_distribution<int> disPosy(-20,(rc.bottom - rc.top));
            switch (disDirection(gen)) {
            case 0 : {
                    monsterPos.x = 20;
                    monsterPos.y = disPosy(gen);
                    break;
                }
            case 1: {
                monsterPos.x = disPosx(gen);
                monsterPos.y = 20;
                break;
            }
            case 2: {
                monsterPos.x = (rc.right - rc.left) - 20;
                monsterPos.y = disPosy(gen);
                break;
            }
            case 3: {
                monsterPos.x = disPosx(gen);
                monsterPos.y = (rc.bottom - rc.top) - 20;
                break;
            }

            }
            if (monsterMaxCount < 1000) {
                if (bGameProgress) {
                    strMonsterList = strMonsterInsert(strMonsterList, monsterCount, monsterCategory, { monsterPos.x, monsterPos.y });
                    monsterCount++;
                    monsterMaxCount++;
               }
            }
            break;
        }
            
        }
    }
    case WM_KEYDOWN:
    {
        keyLayout[wParam] = 1;
        
        break;
    }

    case WM_KEYUP:
    {
        keyLayout[wParam] = 0;
        break;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case 0x1001: {
            ReturnToStartFun();
            SetFocus(hwnd);
            break;
        }
        case 0x1002: {
            if (bGameProgress == true) 
                bGameProgress = !bGameProgress;
            else
                bGameProgress = !bGameProgress;
            SetFocus(hwnd);
            break;
        }
        }
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}