//
// Created by zhongyang219 on 2026/7/7.
//

#include "ColorConvert.h"
unsigned int ColorConvert::RGB2Hex(unsigned char R, unsigned char G, unsigned char B)
{
	return static_cast<unsigned int>(R) << 16 | static_cast<unsigned short>(G) << 8 | B;
}

unsigned char ColorConvert::Hex2R(unsigned int hex)
{
	return hex / 256 / 256;
}

unsigned char ColorConvert::Hex2G(unsigned int hex)
{
	return hex / 256 % 256;
}

unsigned char ColorConvert::Hex2B(unsigned int hex)
{
	return hex % 256;
}

QString ColorConvert::GetRgbString(const QColor& color)
{
	QString strRgb = QString("%1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue());
	return strRgb;
}
