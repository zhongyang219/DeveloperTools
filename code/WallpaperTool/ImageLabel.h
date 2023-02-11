#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QLabel>
#include <QPixmap>

//用于显示图像的窗口，继承于QLabel控件
class CImageLabel : public QLabel
{
    Q_OBJECT
public:
    CImageLabel(QWidget* pParent = nullptr);
	void SetImage(const QPixmap& image);
    QRect GetImageViewRect() const;		//获取图片显示的大小

    QSize GetImageSize() const;

private:
    QPixmap m_image;

    // QObject interface
public:
    virtual void paintEvent(QPaintEvent *) override;

private:
    QRect m_imageRect;
};

#endif // VIDEOLABEL_H
