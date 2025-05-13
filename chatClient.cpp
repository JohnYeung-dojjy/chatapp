#include "chatClient.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>

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

    // Send button
    QPushButton *sendButton = new QPushButton("Send", this);
    inputLayout->addWidget(sendButton);

    // Add input layout to the main layout
    mainLayout->addLayout(inputLayout);

    // Connect signals to slots
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(inputField, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);

    // Set the window title
    setWindowTitle("ChatApp");
}

void ChatWindow::sendMessage() {
    QString message = inputField->text();
    if (message.isEmpty()) {
        return;
    }

    // Display the message in the chat window
    chatDisplay->append("You: " + message);
    inputField->clear();
}

void ChatWindow::receiveMessage() {
    // Placeholder for receiving messages
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ChatWindow window;
    window.resize(400, 600);
    window.show();

    return app.exec();
}

#include "chatClient.moc"