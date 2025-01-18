#include <Windows.h>
#include <tchar.h>
#include <string>
#include <HDRController.h>

using tstring = std::basic_string<TCHAR>;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    core::SetGlobalHDR(true);
    // Podziel lpCmdLine na argumenty
    tstring cmdLine(lpCmdLine);

    // Znajdź pierwszą spację jako rozdzielenie ścieżki programu potomnego i jego argumentów
    size_t pos = cmdLine.find(' ');
    tstring programPath = cmdLine.substr(0, pos);
    tstring programArgs = (pos == tstring::npos) ? _T("") : cmdLine.substr(pos + 1);

    // Struktura STARTUPINFO
    STARTUPINFO si = { sizeof(si) };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW; // Ukrywa konsolę
    si.dwFlags = 0;
    si.wShowWindow = 0;

    // Struktura PROCESS_INFORMATION
    PROCESS_INFORMATION pi;

    // Sklej ścieżkę programu potomnego z argumentami
    //tstring fullCmdLine = _T("\"") + programPath + _T("\" ") + programArgs;
    tstring fullCmdLine = programPath + _T(" ") + programArgs;

    // Uruchomienie nowego procesu
    BOOL success = CreateProcess(
        NULL, // Nazwa programu (null, ponieważ jest podana w linii poleceń)
        const_cast<LPTSTR>(fullCmdLine.c_str()), // Argumenty programu
        NULL, // Atrybuty bezpieczeństwa procesu
        NULL, // Atrybuty bezpieczeństwa wątku
        FALSE, // Dziedziczenie uchwytów
        0, // Flagi tworzenia
        NULL, // Zmienna środowiskowa
        NULL, // Katalog roboczy
        &si, // Informacje startowe
        &pi // Informacje procesu
    );

    if (success) {
        // Zamknij uchwyty do procesu i wątku
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        // Obsługa błędu
        MessageBox(nullptr, fullCmdLine.c_str(), _T("Nie dało się uruchomić programu"), MB_ICONERROR | MB_OK);
    }
    core::SetGlobalHDR(false);
    return 0;
}
