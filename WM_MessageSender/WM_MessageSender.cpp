// WM_MessageSender.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "WM_MessageSender.h"

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス

BOOL CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);

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
            MessageBox(NULL, L"", L"", MB_OK);
            break;
        }
        return FALSE;
    }
    return FALSE;
}
