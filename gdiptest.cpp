#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <gdiplus.h>
using namespace Gdiplus;

BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDOK:
    case IDCANCEL:
        EndDialog(hwnd, id);
        break;
    }
}

void OnDraw(HWND hwnd, HDC hdc)
{
    Graphics graphics(hdc);

    Pen redPen(Color(255, 0, 0), 1);

    PointF pt1(100.5, 101.5);
    PointF pt2(500.5, 101.5);
    graphics.DrawLine(&redPen, pt1, pt2);

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
    SelectObject(hdc, hPen);

    MoveToEx(hdc, 100.5, 100.5, NULL);
    LineTo(hdc, 500.5, 100.5);

    WCHAR string[256];
    wcscpy(string, L"Sample Text");

    LOGFONTW lf = { -16 };
    lstrcpyW(lf.lfFaceName, L"Tahoma");
    lf.lfQuality = NONANTIALIASED_QUALITY;
    HFONT hFont = CreateFontIndirectW(&lf);
    SelectObject(hdc, hFont);

    Font myFont(hdc, &lf);
    PointF point(200.5, 200.5);
    SolidBrush blackBrush(Color(255, 0, 0, 0));

    StringFormat sf(0, LANG_JAPANESE);

    graphics.SetTextRenderingHint(TextRenderingHintSystemDefault);
    graphics.DrawString(
        string,
        -1,
        &myFont,
        point,
        &sf,
        &blackBrush);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 0, 0));
    SetTextCharacterExtra(hdc, 0);
    TextOutW(hdc, 200.5, 200.5, string, lstrlenW(string));
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    if (HDC hdc = BeginPaint(hwnd, &ps))
    {
        OnDraw(hwnd, hdc);
        EndPaint(hwnd, &ps);
    }
}

INT_PTR CALLBACK
DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
    }
    return 0;
}

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
    InitCommonControls();

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    {
        DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, DialogProc);
    }
    GdiplusShutdown(gdiplusToken);
    return 0;
}
