#include "videowidget.h"
#include "mainwindow.h"

VideoWidget::VideoWidget(QWidget *parent)
{
    parentWidget = parent;
}

VideoWidget::~VideoWidget()
{

}

void VideoWidget::mousePressEvent(QMouseEvent *)
{
    emit videoClicked();
}

