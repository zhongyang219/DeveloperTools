#include "ImageLabel.h"
#include <QPaintEvent>
#include <QPainter>

CImageLabel::CImageLabel(QWidget* pParent)
{
}

void CImageLabel::SetImage(const QPixmap& image)
{
	m_image = image;
    m_rect = QRect();
    update();
}

QRect CImageLabel::GetImageViewRect() const
{
    return m_imageRect;
}

void CImageLabel::SetRect(const QRect &rect)
{
    m_rect = rect;
    update();
}

QRect CImageLabel::RealRectToDisplayRect(const QRect &rect)
{
    QRect rectDisplay;
    rectDisplay.setX(m_imageRect.x() + (rect.x() * m_imageRect.width() / m_image.width()));
    rectDisplay.setY(m_imageRect.y() + (rect.y() * m_imageRect.height() / m_image.height()));
    rectDisplay.setWidth(rect.width() * m_imageRect.width() / m_image.width());
    rectDisplay.setHeight(rect.height() * m_imageRect.height() / m_image.height());
    return rectDisplay;
}

QRect CImageLabel::DisplayRectToReadRect(const QRect &rectDisplay)
{
    QRect rectReal;
    rectReal.setX(m_imageRect.width() * (rectDisplay.x() - m_imageRect.x()));
    rectReal.setY(m_imageRect.height() * (rectDisplay.x() - m_imageRect.y()));
    rectReal.setWidth(m_image.width() * rectDisplay.width() / m_imageRect.width());
    rectReal.setHeight(m_image.height() * rectDisplay.height() / m_imageRect.height());
    return rectReal;
}

QSize CImageLabel::GetImageSize() const
{
    return m_image.size();
}

void CImageLabel::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    //绘制图像
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

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
    painter.drawPixmap(m_imageRect, m_image, QRect(0, 0, m_image.width(), m_image.height()));

    //绘制矩形
    if(!m_rect.isEmpty())
    {
        //计算矩形实际显示的大小
        QRect rectDisplay = RealRectToDisplayRect(m_rect);

        //绘制矩形
        QPen mypen;
        mypen.setWidth(4);
        mypen.setColor(QColor(220, 76, 64));
        painter.setPen(mypen);

        painter.drawRect(rectDisplay);

        //QLabel::update();
    }


    QWidget::paintEvent(e);
}
