#include <QDebug>
#include "chatClient.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QHostAddress>
#include <ctime> // for using time() as seed for random number generation
#include <unistd.h> // for using getpid() as seed for random number generation

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9034

std::string gen_random_username() {
    // Generate a random username on client start
    static const int USERNAME_LEN = 10;
    static const std::string prefix = "User_";
    static const char alphanum[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(USERNAME_LEN);

    for (int i = 0; i < USERNAME_LEN; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return prefix+tmp_s;
}

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

    // Get username
    userName = new QString(QString::fromStdString(gen_random_username()));

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

    QString formattedMessage = userName + QString(": ") + message;

    // Send message to server
    socket->write(formattedMessage.toUtf8());
    socket->flush();

    qDebug() << "Sent message:" << message;

    // Display the message in the chat window
    chatDisplay->append("You: " + message);
    inputField->clear();
}

void ChatWindow::receiveMessage() {
    // Placeholder for receiving messages
    QByteArray data = socket->readAll(); // Read all available data
    if (!data.isEmpty()) {
        chatDisplay->append(QString(data).trimmed());
    }
}

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL) * getpid()); // Seed the random number generator with current time and process ID
    QApplication app(argc, argv);

    ChatWindow window;
    window.resize(400, 600);
    window.show();

    return app.exec();
}

#include "chatClient.moc"