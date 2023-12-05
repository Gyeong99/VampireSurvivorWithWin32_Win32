#include "GameSoundManager.h"

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
    // Callback 함수
}

void PlaySoundFile(LPCTSTR soundFilePath) {
    // WAVEFORMATEX 설정 (예: 44.1kHz, 16-bit, stereo)
    WAVEFORMATEX wfx;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 2;
    wfx.nSamplesPerSec = 44100;
    wfx.nAvgBytesPerSec = 44100 * 2 * 16 / 8; // (Sample Rate) * (BitsPerSample / 8) * Channels
    wfx.nBlockAlign = 2 * 16 / 8; // (BitsPerSample / 8) * Channels
    wfx.wBitsPerSample = 16;
    wfx.cbSize = 0;

    // waveOutOpen을 통해 장치 열기
    HWAVEOUT hWaveOut;
    MMRESULT result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProc, 0, CALLBACK_FUNCTION);
    if (result != MMSYSERR_NOERROR) {
        // 오류 처리
        return;
    }

    // WAVEHDR 초기화
    WAVEHDR waveHeader;
    waveHeader.lpData = (LPSTR)soundFilePath;
    waveHeader.dwBufferLength = 0; // 파일 크기로 설정
    waveHeader.dwBytesRecorded = 0;
    waveHeader.dwUser = 0;
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 0;
    waveHeader.lpNext = NULL;
    waveHeader.reserved = 0;

    // waveOutPrepareHeader을 통해 헤더 준비
    result = waveOutPrepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR) {
        // 오류 처리 및 정리
        waveOutClose(hWaveOut);
        return;
    }

    // waveOutWrite를 통해 데이터 전송
    result = waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR) {
        // 오류 처리 및 정리
        waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
        return;
    }

    // 소리가 끝날 때까지 대기 (이 부분은 실제 응용 프로그램에서는 필요에 따라 조절할 수 있음)
    while (waveHeader.dwFlags & WHDR_DONE == 0) {
        Sleep(100);
    }

    // 정리
    waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}