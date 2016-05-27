#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>

namespace Ui {
class MainWindow;
}

class Doubtitles;
class CustomVideo;
class VideoWidget;
class PlayWidget;
class TextEdit;

class QMediaPlayer;
class QMediaPlaylist;
class QShortcut;
class QNetworkAccessManager;
class QNetworkReply;

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void setNextSlide(); //sets dubs->nextSlide to correspond to player->position (iterative so slower than increments)
    void updateSubs(); //sets subs to dubs->nextSlide slide
    //before calling updateSubs, you must change dubs->nextSlide, either by calling setNextSlide or dubs->nextSlide or dubs->previousSlide

    int* userModeP;
    CustomVideo* view;

    ~MainWindow();

private slots:
    void makeRequest(QString& newWord);

    void playPause();

    void studyPlay();

    void on_playPauseBtn_released();

    void on_progressSldr_sliderMoved(int position);

    void on_progressSldr_sliderPressed();

    void on_progressSldr_sliderReleased();

    void on_positionChanged(qint64 position);

    void on_durationChanged(qint64 duration); //i.e. the length of the video

    void on_spaceShortcut();

    void on_volumeSldr_valueChanged(int value);

    void on_volumeBtn_clicked();

    void on_actionOpen_triggered();

    void on_rewindBtn_pressed();

    void on_rewindBtn_released();

    void on_nextBtn_pressed();

    void on_nextBtn_released();

    void on_previousBtn_pressed();

    void on_previousBtn_released();

    void on_transBtn_released();

private:
    Ui::MainWindow* ui;
    QMediaPlayer* player;
    QVideoWidget* videoWidget;
    Doubtitles* doubs;
    QNetworkAccessManager* net;
    QNetworkReply* response;
    QString filename;
    QString staticPath;
    int userMode;
    int slideNum; //do I still need this?
    int muteVol; //volume when player last muted
    bool wasPlaying; //for use if progressSldr_Released
    const QShortcut *playPauseShortcut;

    //Icons
    const QIcon *rewindIcon;
    const QIcon *previousIcon;
    const QIcon *playIcon;
    const QIcon *pauseIcon;
    const QIcon *nextIcon;
    const QIcon *volMuteIcon;
    const QIcon *vol33Icon;
    const QIcon *vol66Icon;
    const QIcon *volFullIcon;
};

#endif // MAINWINDOW_H
