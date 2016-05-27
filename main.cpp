#include "mainwindow.h"
#include "doubtitles.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])//I think the second one is an array of /pointers/, that all point to /chars/, named argv
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();


    return app.exec();
}
