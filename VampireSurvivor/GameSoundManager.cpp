#include "GameSoundManager.h"

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
    // Callback �Լ�
}

void PlaySoundFile(LPCTSTR soundFilePath) {
    // WAVEFORMATEX ���� (��: 44.1kHz, 16-bit, stereo)
    WAVEFORMATEX wfx;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 2;
    wfx.nSamplesPerSec = 44100;
    wfx.nAvgBytesPerSec = 44100 * 2 * 16 / 8; // (Sample Rate) * (BitsPerSample / 8) * Channels
    wfx.nBlockAlign = 2 * 16 / 8; // (BitsPerSample / 8) * Channels
    wfx.wBitsPerSample = 16;
    wfx.cbSize = 0;

    // waveOutOpen�� ���� ��ġ ����
    HWAVEOUT hWaveOut;
    MMRESULT result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProc, 0, CALLBACK_FUNCTION);
    if (result != MMSYSERR_NOERROR) {
        // ���� ó��
        return;
    }

    // WAVEHDR �ʱ�ȭ
    WAVEHDR waveHeader;
    waveHeader.lpData = (LPSTR)soundFilePath;
    waveHeader.dwBufferLength = 0; // ���� ũ��� ����
    waveHeader.dwBytesRecorded = 0;
    waveHeader.dwUser = 0;
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 0;
    waveHeader.lpNext = NULL;
    waveHeader.reserved = 0;

    // waveOutPrepareHeader�� ���� ��� �غ�
    result = waveOutPrepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR) {
        // ���� ó�� �� ����
        waveOutClose(hWaveOut);
        return;
    }

    // waveOutWrite�� ���� ������ ����
    result = waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR) {
        // ���� ó�� �� ����
        waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
        return;
    }

    // �Ҹ��� ���� ������ ��� (�� �κ��� ���� ���� ���α׷������� �ʿ信 ���� ������ �� ����)
    while (waveHeader.dwFlags & WHDR_DONE == 0) {
        Sleep(100);
    }

    // ����
    waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}