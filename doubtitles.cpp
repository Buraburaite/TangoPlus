#include "doubtitles.h"
#include <QFile>
#include <QTextStream>

Doubtitles::Doubtitles(QWidget *parent)
{

}

void Doubtitles::loadSource(QString filename, int* mode)
{

    userMode = mode;
    QString text;
    QFile sFile(filename);
    if (sFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       QTextStream in(&sFile);
       while (!in.atEnd())
       {
          text.append(in.readLine() + "\n");
       }
       sFile.close();
    }

    text.remove("<b>").remove("</b>").remove("<i>").remove("</i>");
    QStringList subs = text.split("\n");
    subs.removeAll("");    //there are variables to delete around here.
    slides = QHash<int, QString>();
    slides.insert(0, "");// make empty string
    for (int i = 1; i < subs.size(); i++) {
        if (subs.at(i).contains("-->")) {
            slides.insert(toMs(subs.at(i).midRef(0,12)), subs[i + 1]);
            slides.insert(toMs(subs.at(i).midRef(17,12)), "");
        }
    }
    slides.insert(std::numeric_limits<int>::max(), "");

    marks = slides.keys();
    qSort(marks);
    nextSlideNum = 0;
    prevWord = new QString;
    target = new QString;

}

int Doubtitles::toMs(const QStringRef r)
{
   QString s = r.toString();
   s = s.remove(",");
   QStringList l = s.split(":");

   int p = l[0].toInt() * 3600000;
   p += l[1].toInt() * 60000;
   p += l[2].toInt();

   return p - 500; //to better average starting point (positionChanged called apprx. once per second when playing)
}

void Doubtitles::nextSlide()
{
    nextSlideNum += 1;
    if (nextSlideNum == marks.size()){
        nextSlideNum = 0 ;
    }
}

void Doubtitles::previousSlide()
{
    nextSlideNum -= 1;
    if (nextSlideNum < 0){
        nextSlideNum = marks.size() - 1;
    }

}

void Doubtitles::mouseReleaseEvent(QMouseEvent *event)
{
    *target = this->selectedText(); //For Study mode
    if (*userMode == 0 && *target != *prevWord && *target != ""){
        emit selectionChanged(*target);
    }
    this->setSelection(0,0);

}

Doubtitles::~Doubtitles()
{

}

