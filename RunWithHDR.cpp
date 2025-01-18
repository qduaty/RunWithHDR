#include <Windows.h>
#include <tchar.h>
#include <string>
#include <HDRController.h>

using tstring = std::basic_string<TCHAR>;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    core::SetGlobalHDR(true);
    tstring cmdLine(lpCmdLine);

    size_t pos = cmdLine.find(' ');
    tstring programPath = cmdLine.substr(0, pos);
    tstring programArgs = (pos == tstring::npos) ? _T("") : cmdLine.substr(pos + 1);

    STARTUPINFO si = { sizeof(si) };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    si.dwFlags = 0;
    si.wShowWindow = 0;

    PROCESS_INFORMATION pi;

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
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        MessageBox(nullptr, fullCmdLine.c_str(), _T("Nie dało się uruchomić programu"), MB_ICONERROR | MB_OK);
    }
    core::SetGlobalHDR(false);
    return 0;
}
