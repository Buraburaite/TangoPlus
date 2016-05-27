#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QMouseEvent>
#include <QNetworkReply>
#include <QTextCursor>

namespace Ui {
class TextEdit;
}

class TextEdit : public QTextEdit
{
    Q_OBJECT

    public:
        TextEdit(QWidget *parent = 0);
        ~TextEdit();

    signals:
        void clickedWord(const QString&);

    public slots:
        void updateDefins(QNetworkReply* reply);

    private:
        QString& findQuery(QString& query, QString& page); //should return QList<QVariant>; update: (results? Not sure why this is here...)
        QString page;
        QString query;
        QList<QVariant> results;

        QStringList wChunks;
        QStringList defins;

        int start;
        int end;
        bool allKana;
        QString kanji;
        QString hira;
        QString defin;
        QString tango;
        QString kana;
        QString englishKeyboard;

        QTextCursor c;

    protected:
        virtual void mouseReleaseEvent(QMouseEvent *event);
};


#endif // TEXTEDIT_H
