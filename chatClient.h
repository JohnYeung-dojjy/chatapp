#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>

class ChatWindow : public QWidget {
    Q_OBJECT

private:
    QTextEdit *chatDisplay;
    QLineEdit *inputField;

public:
    ChatWindow();
private slots:
    void sendMessage();
    void receiveMessage();
};

#endif // CHATCLIENT_H