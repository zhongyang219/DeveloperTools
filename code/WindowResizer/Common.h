#pragma once

class CCommon
{
public:
    static void SetWindowAlwaysOnTop(HWND hWnd, bool top);      //���ô��������ö�
    static bool IsWindowAlwaysOnTop(HWND hWnd);                 //�жϴ����Ƿ������ö�

    static void ShowWindowInTaskBar(HWND hWnd, bool show);      //���ô�����ʾ��������
    static bool IsWindowShownInTaskBar(HWND hWnd);              //�жϴ����Ƿ���ʾ��������
};

