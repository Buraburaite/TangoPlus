#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include "mainwindow.h"
#include <QVideoWidget>

namespace Ui {
class VideoWidget;
}

class VideoWidget : public QVideoWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent = 0); //when subclassing, remember to include this parameter and to promote in ui file.
    ~VideoWidget();

signals:
    void videoClicked();

private:
    QWidget* parentWidget;
    void mousePressEvent(QMouseEvent *);
    //MainWindow* window;

};

#endif // VIDEOWIDGET_H
