#include "ImageLabel.h"
#include <QPaintEvent>
#include <QPainter>

CImageLabel::CImageLabel(QWidget* pParent)
{
}

void CImageLabel::SetImage(const QPixmap& image)
{
	m_image = image;
    update();
}

QRect CImageLabel::GetImageViewRect() const
{
    return m_imageRect;
}

QSize CImageLabel::GetImageSize() const
{
    return m_image.size();
}

void CImageLabel::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    ////绘制图像
    //painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    //使背景图片在主窗口中保持设定的比例，在这里计算背景图片的大小
    int x = 0, y = 0, w = 0, h = 0;
    const double picWidthHeightRatio = static_cast<double>(m_image.width()) / m_image.height();
    double widthHeightRatio = static_cast<double>(size().width()) / size().height();      //当前窗口的宽高比
	//如果图片比窗口小，则图片在窗口中居中显示
	if (m_image.width() <= size().width() && m_image.height() <= size().height())
	{
		x = (size().width() - m_image.width()) / 2;
		y = (size().height() - m_image.height()) / 2;
		w = m_image.width();
		h = m_image.height();
	}
	//否则计算图片的大小
	else
	{
		if (picWidthHeightRatio > widthHeightRatio)       //如果背景图片的宽高比大于窗口的宽高比
		{
			w = size().width();
			h = m_image.height() * w / m_image.width();
			x = 0;
			y = (size().height() - h) / 2;
		}
		else               //如果背景图片的宽高比小于窗口的宽高比
		{
			h = size().height();
			w = m_image.width() * h / m_image.height();
			y = 0;
			x = (size().width() - w) / 2;
		}
	}
    m_imageRect = QRect(x, y, w, h);
    QPixmap drawImage = m_image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(m_imageRect, drawImage, QRect(0, 0, w, h));


    QWidget::paintEvent(e);
}
