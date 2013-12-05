#ifndef KEERRORTEXT_H
#define KEERRORTEXT_H

#include <QTextEdit>


enum ErrorMsgType{
    EMsg_Info,
    EMsg_Warning,
    EMsg_Error
};


class KeErrorText : public QTextEdit
{
    Q_OBJECT
public:
    explicit KeErrorText(QWidget *parent = 0);

signals:

public slots:
    void showErrorMessage(const QString &str, int error);
};

#endif // KEERRORTEXT_H
