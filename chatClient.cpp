#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

class ChatWindow : public QWidget {
public:
    ChatWindow() {
        // Set up the main layout
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // Chat display area
        QTextEdit *chatDisplay = new QTextEdit(this);
        chatDisplay->setReadOnly(true); // Make it read-only
        mainLayout->addWidget(chatDisplay);

        // Input area layout
        QHBoxLayout *inputLayout = new QHBoxLayout();

        // Input field
        QLineEdit *inputField = new QLineEdit(this);
        inputLayout->addWidget(inputField);

        // Send button
        QPushButton *sendButton = new QPushButton("Send", this);
        inputLayout->addWidget(sendButton);

        // Add input layout to the main layout
        mainLayout->addLayout(inputLayout);

        // Connect the send button to a lambda function to append text
        connect(sendButton, &QPushButton::clicked, [=]() {
            QString message = inputField->text();
            if (!message.isEmpty()) {
                chatDisplay->append("You: " + message);
                inputField->clear();
            }
        });

        // Connect the input field to send message on Enter key press
        connect(inputField, &QLineEdit::returnPressed, [=]() {
            QString message = inputField->text();
            if (!message.isEmpty()) {
                chatDisplay->append("You: " + message);
                inputField->clear();
            }
        });

        // Set the window title
        setWindowTitle("ChatApp");
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ChatWindow window;
    window.resize(400, 600);
    window.show();

    return app.exec();
}