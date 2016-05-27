#ifndef DOUBTITLES_H
#define DOUBTITLES_H

#include <QLabel>
#include <QHash>


namespace Ui {
class Doubtitles;
}


class Doubtitles : public QLabel
{
    Q_OBJECT

public:
    Doubtitles(QWidget* parent = 0);
    void loadSource(QString filename, int* mode);
    QHash<int, QString> slides;
    QList<int> marks;
    int nextSlideNum;
    void nextSlide();
    void previousSlide();
    QString* prevWord;
    QString* target;

    int* userMode;

    ~Doubtitles();

signals:
    void selectionChanged(QString& newWord);

private:
    int toMs(const QStringRef r);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // DOUBTITLES_H
