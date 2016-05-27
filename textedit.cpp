#include "textedit.h"
#include <QTextCursor>
#include <QDebug>

#include <QStringList>

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent)
{
    setReadOnly(true); //so they can't edit it

    kana = ("あいうえおかきくけこがぎぐげごさしすせそざじずぜぞまみむめもなにぬねのはひふへほばび"
            "ぶべぼぱぴぷぺぽたちつてとだぢづでどらりるれろわをやゆよんゃょゅアイウエオカキクケコガギ"
            "グゲゴサシスセソザジズゼゾマミムメモナニヌネノハヒフヘホバビブベボパピプペポタチツテトダ"
            "ヂヅデドラリルレロワヲヤユヨンャュョ");
    englishKeyboard = ("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXC"
                       "VBNM,;[]'/.0123456789/*-+!@#$%^&*()_+-=<>:{}\"?`~");

}

TextEdit::~TextEdit()
{
}

void TextEdit::updateDefins(QNetworkReply *reply)
{
    page = QString::fromUtf8(reply->readAll());
    query = reply->url().toString().remove("http://tangorin.com/general/");

    this->setHtml(findQuery(query, page));

}

QString& TextEdit::findQuery(QString& query, QString& page)
{

    if (page.contains("Sorry, no results.")){
        query = "Nothing found for " + query + ",<br>perhaps it is a name?";
        return query;
    }
    else {
        start = page.indexOf("w:") - 1; //pre-processing of page
        end = page.indexOf("results-footer");
        page = page.mid(start, end - start);

        wChunks = page.split("w:'");

    }

    defins.clear(); //don't need list of lists, just put spacing in-between
    for (int i = 1; i < wChunks.length(); i++){
        if (!wChunks[i - 1].endsWith("	")){ //ignore definitions for individual kanji
            continue;
        }
        end = wChunks[i].indexOf("k:") - 5;
        kanji = wChunks[i].left(end); //I miss python slicing
        end += 8;
        hira = wChunks[i].mid(end, wChunks[i].indexOf("'", end) - end);
        defins += "<b>" + hira + "</b>(" + kanji + ")<br><br>"; //note: including any html tags makes entire thing html
        end = 0;
        for (int q = 0; q < wChunks[i].count("<b>"); q++){
            start = wChunks[i].indexOf("<b>", end) + 3;
            defin = wChunks[i].mid(start, wChunks[i].indexOf("</b>", start) - start);
            if (englishKeyboard.contains(defin[0])){ //NOT a complete solution
                defins += defin + ",";
            }
            end = start + defin.length();

        }
        defins[defins.length() - 1].chop(1);
        defins += "<br>------------------<br><br>";
    }
    defins.removeLast();
    query = defins.join(" ");

    return query;

}

void TextEdit::mouseReleaseEvent(QMouseEvent *event) //just some tech
{
    c = cursorForPosition(event->pos()); //Cursors are confusing. Have to be made and set each time.
    c.select(QTextCursor::WordUnderCursor);
    setTextCursor(c);
    qDebug() << c.selectedText();
}
