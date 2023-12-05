#pragma once
#include "arrayList.h"
#include <mmsystem.h>
#include <Digitalv.h>

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
void PlaySoundFile(LPCTSTR soundFilePath);