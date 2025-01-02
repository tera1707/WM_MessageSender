// WM_MessageSender.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "WM_MessageSender.h"
#include <psapi.h>
#include <string>
#include <filesystem>

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
HWND g_hDlg;

BOOL CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);
void OnButtonClicked();
void OnSendButtonClicked();
BOOL OnFindWindow(HWND hWnd, LPARAM lp);

struct WindowData
{
    WindowData(HWND hwnd, BOOL isVisible, std::wstring title, std::wstring className)
    {
        hWnd = hwnd;
        IsVisible = isVisible;
        Title = title;
        ClassName = className;
    }

    HWND hWnd;
    BOOL IsVisible;
    std::wstring Title;
    std::wstring ClassName;
};

std::vector<WindowData> WindowDataList;

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
    case WM_INITDIALOG:
        g_hDlg = hDlg;
        break;
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
        case IDC_SEND_MSG:
            OnSendButtonClicked();
            break;
        }
        return FALSE;
    }
    return FALSE;
}

//std::vector 

void OnButtonClicked()
{
    auto listBox = GetDlgItem(g_hDlg, IDC_LIST1);
    int pos = (int)SendMessage(listBox, LB_RESETCONTENT, 0, (LPARAM)0);
    WindowDataList.clear();

    EnumWindows((WNDENUMPROC)OnFindWindow, 0);
}

BOOL OnFindWindow(HWND hWnd, LPARAM lp)
{
    WCHAR dest[MAX_PATH] = { 0 };

    auto bufferSize = GetWindowTextLength(hWnd) + 1;

    auto title = new wchar_t[bufferSize];	// newで長さlen + 1のTCHAR配列バッファを用意.

    if (GetWindowText(hWnd, title, bufferSize) == 0)
    {
        auto err = GetLastError();
        return true;
    }

    auto className = new wchar_t[256];	// newで長さlen + 1のTCHAR配列バッファを用意.

    if (GetClassName(hWnd, className, 256) == 0)
    {
        auto err = GetLastError();
        return true;
    }

    // 表示したい情報を揃える
    auto isVisible = IsWindowVisible(hWnd);
    auto titleWstr = std::wstring(title);
    auto classNameWstr = std::wstring(className);
    auto txt = ((isVisible == 0 ? L"×, " : L"〇, ") + titleWstr + L", " + classNameWstr + L"\r\n");
    OutputDebugString(txt.c_str());

    // ------------------------------------------
    // 表示＆保存処理
    // ------------------------------------------ 

    // 不可視ウインドウを表示するかどうかのチェックを取得
    auto chk = GetDlgItem(g_hDlg, IDC_CHECK1);
    BOOL showInvisible = (int)SendMessage(chk, BM_GETCHECK, 0, (LPARAM)txt.c_str());

    if (isVisible == TRUE
        || showInvisible == TRUE)
    {
        // 値の保存＆表示
        auto data = WindowData(hWnd, isVisible, titleWstr, classNameWstr);

        WindowDataList.push_back(data);

        auto listBox = GetDlgItem(g_hDlg, IDC_LIST1);
        //int count = (int)SendMessage(listBox, LB_GETCOUNT, 0, (LPARAM)0);
        int pos = (int)SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)txt.c_str());
    }

    return true;
}

void OnSendButtonClicked()
{
    auto listBox = GetDlgItem(g_hDlg, IDC_LIST1);
    int pos = (int)SendMessage(listBox, LB_GETCURSEL, 0, (LPARAM)0);

    if (pos == LB_ERR)
    {
        OutputDebugString(L"ウインドウが選択されてません。");
    }

    auto selectedWindow = WindowDataList[pos];

    auto ret = SendMessage(selectedWindow.hWnd, WM_CLOSE, 0, 0);
    auto retStr = std::to_wstring(ret);

    MessageBox(NULL, retStr.c_str(), L"応答", MB_OK);
}
