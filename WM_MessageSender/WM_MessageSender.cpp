// WM_MessageSender.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "WM_MessageSender.h"
#include <psapi.h>
#include <string>
#include <filesystem>

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス

BOOL CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);
void OnButtonClicked();
BOOL OnFindWindow(HWND hWnd, LPARAM lp);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    hInst = hInstance;

    // ダイアログの名前と、プロシージャを指定してDialogBoxを開く
    DialogBox(hInst, L"WM_MessageSenderMain", NULL, (DLGPROC)MyDlgProc);

    return (int)0;
}

BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDOK:
            EndDialog(hDlg, IDOK);
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        case IDC_BUTTON1:
            OnButtonClicked();
            break;
        }
        return FALSE;
    }
    return FALSE;
}

//std::vector 

void OnButtonClicked()
{
    EnumWindows((WNDENUMPROC)OnFindWindow, 0);
}

BOOL OnFindWindow(HWND hWnd, LPARAM lp)
{
    WCHAR dest[MAX_PATH] = { 0 };

    // プロセスID
    DWORD processID = NULL;
    GetWindowThreadProcessId(hWnd, &processID);

    // プロセスハンドル
    HANDLE hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (!hProcess)
        return false;

    // モジュールハンドル
    HMODULE hModule = NULL;
    DWORD dummy = 0;
    if (!EnumProcessModules(hProcess, &hModule, sizeof(HMODULE), &dummy))
        return false;

    // ファイル名(フルパス)
    memset(dest, 0, MAX_PATH - 1);
    if (!GetModuleFileNameEx(hProcess, hModule, dest, MAX_PATH - 1))
        return false;

    CloseHandle(hProcess);
    // hModuleは自分で閉じちゃいけない

    auto procPath = std::wstring(dest);
    std::filesystem::path filepath = procPath;

    if (filepath.filename() == L"Notepad.exe")
    {
        OutputDebugString(L"あった！");

        PostMessage(hWnd, WM_CLOSE, 0, 0);
    }

    OutputDebugString((procPath + L"\r\n").c_str());

    return true;
}
