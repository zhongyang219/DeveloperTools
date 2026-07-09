#include "common/Common.h"
#include <QtGlobal>

#ifdef Q_OS_WIN
#include <dwmapi.h>
#pragma comment(lib,"Dwmapi.lib")
#endif

QColor Common::GetSystemThemeColor()
{
#ifdef Q_OS_WIN
    DWORD crColorization;
    BOOL fOpaqueBlend;
    QColor theme_color{};
    HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
    if (result == S_OK)
    {
        BYTE r, g, b;
        r = (crColorization >> 16) % 256;
        g = (crColorization >> 8) % 256;
        b = crColorization % 256;
        theme_color = QColor(r, g, b);
    }
    return theme_color;

#endif
    return QColor();
}
