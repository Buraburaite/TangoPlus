#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "doubtitles.h"
#include "videowidget.h"
#include "textedit.h"
#include "playwidget.h"
#include "customvideo.h"

#include <QUrl>
#include <QDebug>
#include <QFileDialog>
#include <QShortcut>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <typeinfo>
#include <QGraphicsVideoItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),                  //these variables are being initialized by the syntax below
    ui(new Ui::MainWindow)                //ThingToInit(ValueIWant), DoIt(Again)
{
    ui->setupUi(this);
    resize(800, 500);
    //this->setStyleSheet("background-color: rgb(255,127,0)");

    staticPath = "C:\\Users\\Javier\\Desktop\\TangoPlus\\testing\\TangoPlus\\static\\";
    //Icons
    rewindIcon          = new QIcon(QPixmap(staticPath + "Icons\\Rewind.png"));
    previousIcon        = new QIcon(QPixmap(staticPath + "Icons\\Previous.png"));
    playIcon            = new QIcon(QPixmap(staticPath + "Icons\\Play.png"));
    pauseIcon           = new QIcon(QPixmap(staticPath + "Icons\\Pause.png"));
    nextIcon            = new QIcon(QPixmap(staticPath + "Icons\\Next.png"));
    volMuteIcon         = new QIcon(QPixmap(staticPath + "Icons\\Volume Mute.png"));
    vol33Icon           = new QIcon(QPixmap(staticPath + "Icons\\Volume 33.png"));
    vol66Icon           = new QIcon(QPixmap(staticPath + "Icons\\Volume 66.png"));
    volFullIcon         = new QIcon(QPixmap(staticPath + "Icons\\Volume Full.png"));

    //Shortcuts
    playPauseShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this, SLOT(on_spaceShortcut));//parentheses issue annoying
    connect(playPauseShortcut, SIGNAL(activated()), this, SLOT(on_spaceShortcut()));

    player = new QMediaPlayer;
    videoWidget = ui->videoWidget; //keep address here for connection later
    player->setVideoOutput(videoWidget);
    player->setVolume(ui->volumeSldr->value());

    doubs = ui->doubsLbl;
    doubs->setFont(QFont("Kochi Gothic", 18));

    net = new QNetworkAccessManager(this);

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
    connect(videoWidget, SIGNAL(videoClicked()), this, SLOT(playPause()));

    connect(doubs, SIGNAL(selectionChanged(QString&)), this, SLOT(makeRequest(QString&)));
    connect(net, SIGNAL(finished(QNetworkReply*)), ui->textEditWidget, SLOT(updateDefins(QNetworkReply*)));
    //Okay, so the signal will be sent whenever a response is completed.
    //Therefore, updateDefins should really be everything that happens after that.
    //So, selectionChanged (signal) causes MainWindow::newRequest(),
    //which causes &QNetworkAccessManager::finished, which causes
    //updateDefins. response->deleteLater in updateDefins

    ui->rewindBtn->setIcon(*rewindIcon);
    ui->previousBtn->setIcon(*previousIcon);
    ui->playPauseBtn->setIcon(*playIcon);
    ui->nextBtn->setIcon(*nextIcon);
    ui->volumeBtn->setIcon(*vol66Icon);

    userMode = 0; //0 = Study, 1 = Play
    userModeP = &userMode;

    this->show();

    //on_actionOpen_triggered(); //AUTOPLAY (assuming "filename" set inside the function)

}

void MainWindow::mousePressEvent(QMouseEvent *)
{
    playPause();
}

void MainWindow::playPause()
{
    if (player->state() != 1) {
        player->play();
        ui->playPauseBtn->setIcon(*pauseIcon);
    }
    else {
        player->pause();
        ui->playPauseBtn->setIcon(*playIcon);
    }
}

void MainWindow::studyPlay() //will be expanded with extra modes, this structure is in anticipation of that
{
    if (userMode == 0){
        ui->textEditWidget->show();
    }
    else if (userMode == 1){
        ui->textEditWidget->hide();
    }
}

void MainWindow::setNextSlide()
{
    int p = player->position();
    for (int i = 0; i < doubs->marks.size(); i++){
        if (p >= doubs->marks[i] && // >= needed so that dragging to far left works
            p < doubs->marks[i + 1]){
            doubs->nextSlideNum = i;
        }
    }
}

void MainWindow::updateSubs()
{
    doubs->setText(doubs->slides[doubs->marks[doubs->nextSlideNum]]);
//    QPoint cursorPosition(this->mapFromGlobal(QCursor::pos()));
//    int textCursorPosition;
//    textCursorPosition = ui->doubsLbl->
//    ui->transBtn->setText();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeRequest(QString& newWord)
{
    net->get(QNetworkRequest(QUrl("http://tangorin.com/general/" + newWord)));
}

void MainWindow::on_playPauseBtn_released()
{
    playPause();
}

void MainWindow::on_progressSldr_sliderMoved(int position)
{
    player->setPosition(position);
    //Causes image to update
    player->play();
    player->pause();
}

void MainWindow::on_progressSldr_sliderPressed()
{
    if (player->state() == 1){
        wasPlaying = true;
    }
    player->pause();
    doubs->setText("");
}

void MainWindow::on_progressSldr_sliderReleased()
{
    player->setPosition(ui->progressSldr->value());
    setNextSlide();
    updateSubs();
    if (wasPlaying == true){
        player->play();
    }
    wasPlaying = false;
}

void MainWindow::on_positionChanged(qint64 position) //see notes on this function
{
    ui->progressSldr->setValue(position);
    //ui->transBtn->setText(QString::number(position) + "-" + QString::number(doubs->nextSlideNum));
    if (player->state() == 1 && position >= doubs->marks[doubs->nextSlideNum]){
        updateSubs();
        doubs->nextSlide();
    }
}

void MainWindow::on_durationChanged(qint64 duration) //this function could be sublimated by actionOpen instead
{
    ui->progressSldr->setMaximum(duration);
    ui->progressSldr->setTickInterval(duration / 10);
}

void MainWindow::on_spaceShortcut()
{
    playPause();
}

void MainWindow::on_volumeSldr_valueChanged(int value)
{
    player->setVolume(value);
    if (value == 0){//setMuted(true) here unnecessary
        ui->volumeBtn->setIcon(*volMuteIcon);
    }
    else if (value > 66){
        player->setMuted(false);
        ui->volumeBtn->setIcon(*volFullIcon);
    }
    else if (value < 33){
        player->setMuted(false);
        ui->volumeBtn->setIcon(*vol33Icon);
    }
    else{
        player->setMuted(false);
        ui->volumeBtn->setIcon(*vol66Icon);
    }
}

void MainWindow::on_volumeBtn_clicked()
{
    if (player->isMuted() == false){
        muteVol = player->volume();
        player->setMuted(true);
        ui->volumeBtn->setIcon(*volMuteIcon);
        ui->volumeSldr->setValue(0);
    }
    else{
        player->setMuted(false);
        ui->volumeSldr->setValue(muteVol);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (player->state() == 1){
        playPause();
    }
//    filename = QFileDialog::getOpenFileName( //fix this so that you can open and look without losing current video
//                this,
//                tr("Select video"),staticPath,//QDir::currentPath(),
//                "Video (*.mp4 *.mov *.flv *.wmv *.avi *.mkv);;All Files (*.*)");
    filename = staticPath + "Hanzawa Naoki - Ep 2.mkv";
    player->setMedia(QMediaContent(QUrl::fromLocalFile(filename))); //Make it so that if you just exit out nothing happens
    doubs->setText("");
    wasPlaying = false;

    filename = staticPath + "Hanzawa Naoki - Ep 2.srt";
//    filename = QFileDialog::getOpenFileName(
//                this,
//                tr("Select subtitles in the original language"),staticPath + "for demo",//QDir::currentPath(),
//                "Subtitles (*.srt);;All Files (*.*)");
    doubs->loadSource(filename, userModeP);
    playPause();

}

void MainWindow::on_rewindBtn_pressed() //yeah, I would put the two functions together in "clicked"
{                                       //but for some reason, then it doesn't rewind *burns laptop*
    if (player->state() == 1){
        playPause();
        wasPlaying = true;
    }
    player->setPosition(player->position() - 6000); //already forces val >= 0, neat!
    setNextSlide();
    updateSubs();

}

void MainWindow::on_rewindBtn_released()
{
    if (wasPlaying == true){
        playPause();
        wasPlaying = false;
    }
}

void MainWindow::on_nextBtn_pressed()
{
    if (player->state() == 1){
        player->pause();
        wasPlaying = true;
    }
}

void MainWindow::on_nextBtn_released()
{
    while (doubs->slides[doubs->marks[doubs->nextSlideNum]] == ""){ doubs->nextSlide(); }
    updateSubs();
    player->setPosition(doubs->marks[doubs->nextSlideNum] + 500);
    doubs->nextSlide();
    if (wasPlaying == true){
        ui->playPauseBtn->setIcon(*pauseIcon);
        player->play();
    }
    wasPlaying = false;

}

void MainWindow::on_previousBtn_pressed()
{
    if (player->state() == 1){
        player->pause();
        wasPlaying = true;
    }

}

void MainWindow::on_previousBtn_released()
{
    if (player->position() - doubs->marks[doubs->nextSlideNum - 1] < 4000){ //4 secs should be good, right?
        doubs->previousSlide();
    }
    while (doubs->slides[doubs->marks[doubs->nextSlideNum - 1]] == ""){ doubs->previousSlide(); }
    doubs->previousSlide();
    updateSubs();
    player->setPosition(doubs->marks[doubs->nextSlideNum] + 500);
    if (wasPlaying == true){
        ui->playPauseBtn->setIcon(*pauseIcon);
        player->play();
    }
    wasPlaying = false;

}

void MainWindow::on_transBtn_released()
{
    if (userMode == 0) {
        userMode++;
        ui->transBtn->setText("PLAY");
    }
    else {
        userMode = 0;
        ui->transBtn->setText("STUDY");
    }
    studyPlay();
}
