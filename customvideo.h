#ifndef CUSTOMVIDEO_H
#define CUSTOMVIDEO_H
#include <QGraphicsView>

class QResizeEvent;
class QGraphicsScene;
class QGraphicsVideoItem;
class QGraphicsTextItem;


class CustomVideo : public QGraphicsView
{
public:
    CustomVideo(QWidget *parent);

    void resizeEvent(QResizeEvent* event);

    QGraphicsScene* scene;
    QGraphicsVideoItem* videoItem;
    QGraphicsTextItem* textItem;

    ~CustomVideo();

};

#endif // CUSTOMVIDEO_H
