//
// Created by zhongyang219 on 2026/7/7.
//

#ifndef DEVELOPERTOOLS_COLORCONVERT_H
#define DEVELOPERTOOLS_COLORCONVERT_H
#include <QString>
#include <QColor>

class ColorConvert
{
public:
    static unsigned int RGB2Hex(unsigned char R, unsigned char G, unsigned char B);	//RGB值转换成颜色十六进制值
    static unsigned char Hex2R(unsigned int hex);	//从颜色十六进制获取RGB中的红色值
    static unsigned char Hex2G(unsigned int hex);	//从颜色十六进制获取RGB中的绿色值
    static unsigned char Hex2B(unsigned int hex);	//从颜色十六进制获取RGB中的蓝色值
    static unsigned int GetColorrefValue(const QColor& color);
    static QColor ColorrefToColor(unsigned int colorref);

    static QString GetRgbString(const QColor& color);
};


#endif //DEVELOPERTOOLS_COLORCONVERT_H
