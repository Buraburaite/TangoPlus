#include "customvideo.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QGraphicsTextItem>


CustomVideo::CustomVideo(QWidget *parent)
{
    scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::black);
    setScene(scene);
    videoItem = new QGraphicsVideoItem;
    scene->addItem(videoItem);
    videoItem->setScale(2); //makes video larger than view's default size, resizeEvent then fixes it.

    textItem = new QGraphicsTextItem("Hello");
    textItem->setDefaultTextColor(Qt::cyan);
    textItem->setZValue(1);
    scene->addItem(textItem);

    QGraphicsTextItem* extra = new QGraphicsTextItem("Now");
    extra->setDefaultTextColor(Qt::cyan);
    scene->addItem(extra);

}

void CustomVideo::resizeEvent(QResizeEvent *event)
{
    fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}
    /*There is lag when the video item is large. Specifically, the video starts to trail behind the
    audio. This is a pretty big issue, not equipped to fix it. I made sure that calling resizeEvent
    isn't what causes it (that function really IS only called on resizing, as well as a couple times
    upon start-up). Apparently Qt Creator has something called a Profiler which will help me with
    figuring out how where the lag is happening, and then it's up to me to figure out how to fix it.
    But hell if I know, video rendering is way beyond my ken :/ Can't just make the maximum size
    whatever my comp can handle either. Lag may get worse when adding the label, etc. Ah well.
    Gotta leave this for another day.*/

CustomVideo::~CustomVideo()
{

}

