#include "KeErrorText.h"
#include <QTime>
#include <QScrollBar>
#include <QTextCursor>
KeErrorText::KeErrorText(QWidget *parent) :
    QTextEdit(parent)
{
}



void stringToHtmlFilter(QString &str)
{
    //注意这几行代码的顺序不能乱，否则会造成多次替换
    str.replace("&","&amp;");
    str.replace(">","&gt;");
    str.replace("<","&lt;");
    str.replace("\"","&quot;");
    str.replace("\'","&#39;");
    str.replace(" ","&nbsp;");
    str.replace("\n","<br>");
    str.replace("\r","<br>");
}


void stringToHtml(QString &str,QColor crl)
{
    QByteArray array;
    array.append(crl.red());
    array.append(crl.green());
    array.append(crl.blue());
    QString strC(array.toHex());

    str =  QString("<span  style='color:#%1;'>%2</span>").arg(strC).arg(str);
}

void KeErrorText::showErrorMessage(const QString &str, int error)
{
    QString htmlStr =  QTime::currentTime().toString() + " : " + str;
    stringToHtmlFilter(htmlStr);
    switch (error){
    case EMsg_Info:
        stringToHtml(htmlStr,QColor(Qt::black));
        break;
    case EMsg_Warning:
        stringToHtml(htmlStr,QColor(Qt::darkMagenta));
        break;
    case EMsg_Error:
        stringToHtml(htmlStr,QColor(Qt::red));
        break;
    }
    QTextCursor cursor =  this->textCursor();
    cursor.movePosition(QTextCursor::End);
    this->setTextCursor(cursor);

    this->insertHtml(htmlStr);
    this->insertHtml("<br>");

    QScrollBar* scrollbar = this->verticalScrollBar();
    scrollbar->setSliderPosition(scrollbar->maximum());

}

