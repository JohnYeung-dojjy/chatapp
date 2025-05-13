#include <QDebug>
#include "chatClient.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QHostAddress>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9034

ChatWindow::ChatWindow() {
    // Set up the main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Chat display area
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true); // Make it read-only
    mainLayout->addWidget(chatDisplay);

    // Input area layout
    QHBoxLayout *inputLayout = new QHBoxLayout();

    // Input field
    inputField = new QLineEdit(this);
    inputLayout->addWidget(inputField);
    inputField->setEnabled(false); // Disable input field until connected

    // Send button
    QPushButton *sendButton = new QPushButton("Send", this);
    inputLayout->addWidget(sendButton);

    // Add input layout to the main layout
    mainLayout->addLayout(inputLayout);



    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(SERVER_IP), SERVER_PORT);
    chatDisplay->append(QString("Connecting to server %1 at port %2...").arg(SERVER_IP).arg(SERVER_PORT));
    if (!socket->waitForConnected(3000)) {
        chatDisplay->append("Error: Unable to connect to server.");
        return;
    }
    chatDisplay->append(QString("Success!"));
    inputField->setEnabled(true); // Enable input field after connection is successful

    // Connect signals to slots
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(inputField, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);
    connect(socket, &QTcpSocket::readyRead, this, &ChatWindow::receiveMessage);

    // Set the window title
    setWindowTitle("ChatApp");
}

void ChatWindow::sendMessage() {
    QString message = inputField->text();
    if (message.isEmpty()) {
        return;
    }

    // Send message to server
    socket->write(message.toUtf8());
    socket->flush();

    qDebug() << "Sent message:" << message;

    // Display the message in the chat window
    chatDisplay->append("You: " + message);
    inputField->clear();
}

void ChatWindow::receiveMessage() {
    // Placeholder for receiving messages
    qDebug() << socket->canReadLine();
    QByteArray data = socket->readAll(); // Read all available data
    if (!data.isEmpty()) {
        chatDisplay->append("Server: " + QString(data).trimmed());
        qDebug() << "Received message:" << data;
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ChatWindow window;
    window.resize(400, 600);
    window.show();

    return app.exec();
}

#include "chatClient.moc"