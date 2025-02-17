#include "HideExplorerSearchBar.h"

HWND FindChildWindow(
    HWND hwndParent,
    wchar_t* lpszClass
)
{
    HWND hwnd = FindWindowEx(
        hwndParent,
        NULL,
        lpszClass,
        NULL
    );
    if (hwnd == NULL)
    {
        HWND hwndChild = FindWindowEx(
            hwndParent,
            NULL,
            NULL,
            NULL
        );
        while (hwndChild != NULL && hwnd == NULL)
        {
            hwnd = FindChildWindow(
                hwndChild,
                lpszClass
            );
            if (hwnd == NULL)
            {
                hwndChild = FindWindowEx(
                    hwndParent,
                    hwndChild,
                    NULL,
                    NULL
                );
            }
        }
    }
    return hwnd;
}

VOID HideExplorerSearchBar(HWND hWnd)
{
    HWND band = NULL, rebar = NULL;
    band = FindChildWindow(
        hWnd,
        L"TravelBand"
    );
    if (!band)
    {
        return;
    }
    rebar = GetParent(band);
    if (!rebar)
    {
        return;
    }
    int idx = 0;
    idx = (int)SendMessage(
        rebar,
        RB_IDTOINDEX,
        4,
        0
    );
    if (idx >= 0)
    {
        SendMessage(
            rebar,
            RB_SHOWBAND,
            idx,
            FALSE
        );
    }
    idx = (int)SendMessage(
        rebar,
        RB_IDTOINDEX,
        5,
        0
    );
    if (idx >= 0)
    {
        SendMessage(
            rebar,
            RB_SHOWBAND,
            idx,
            TRUE
        );
    }
    RedrawWindow(
        rebar,
        NULL,
        NULL,
        RDW_UPDATENOW | RDW_ALLCHILDREN
    );
}

LRESULT CALLBACK HideExplorerSearchBarSubClass(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    UINT_PTR uIdSubclass,
    DWORD_PTR dwRefData
)
{
    if (uMsg == WM_SIZE || uMsg == WM_PARENTNOTIFY)
    {
        if (uMsg == WM_SIZE && IsWindows11Version22H2OrHigher()) HideExplorerSearchBar(hWnd);
        else if (uMsg == WM_PARENTNOTIFY && (WORD)wParam == 1) HideExplorerSearchBar(hWnd);
    }
    else if (uMsg == WM_DESTROY)
    {
        RemoveWindowSubclass(hWnd, HideExplorerSearchBarSubClass, HideExplorerSearchBarSubClass);
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
