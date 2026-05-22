#include "ScpiClient.hpp"
#include "TcpTransport.hpp"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

#include <QString>

#include <memory>
#include <string>
#include <exception>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("SCPI C++ Instrument Demo");

    auto *layout = new QVBoxLayout(&window);

    // =====================================================
    // Host Input
    // =====================================================

    auto *hostLabel = new QLabel("Host");
    auto *hostInput = new QLineEdit("localhost");

    // =====================================================
    // Port Input
    // =====================================================

    auto *portLabel = new QLabel("Port");
    auto *portInput = new QLineEdit("5025");

    // =====================================================
    // SCPI Command Input
    // =====================================================

    auto *commandLabel = new QLabel("SCPI Command");
    auto *commandInput = new QLineEdit("*IDN?");

    // =====================================================
    // Send Button
    // =====================================================

    auto *sendButton = new QPushButton("Send Command");

    // =====================================================
    // Response Output
    // =====================================================

    auto *responseLabel = new QLabel("Response");

    auto *responseBox = new QTextEdit();
    responseBox->setReadOnly(true);

    // =====================================================
    // Add Widgets To Layout
    // =====================================================

    layout->addWidget(hostLabel);
    layout->addWidget(hostInput);

    layout->addWidget(portLabel);
    layout->addWidget(portInput);

    layout->addWidget(commandLabel);
    layout->addWidget(commandInput);

    layout->addWidget(sendButton);

    layout->addWidget(responseLabel);
    layout->addWidget(responseBox);

    // =====================================================
    // Button Click Event
    // =====================================================

    QObject::connect(
        sendButton,
        &QPushButton::clicked,
        [&]()
        {
            try
            {
                std::string host =
                    hostInput->text().toStdString();

                int port =
                    portInput->text().toInt();

                std::string command =
                    commandInput->text().toStdString();

                auto transport =
                    std::make_unique<TcpTransport>(host, port);

                ScpiClient client(std::move(transport));

                std::string response =
                    client.query(command);

                responseBox->setText(
                    QString::fromStdString(response));
            }
            catch (const std::exception& ex)
            {
                responseBox->setText(
                    QString("ERROR: ") + ex.what());
            }
        });

    // =====================================================
    // Show Window
    // =====================================================

    window.resize(500, 350);

    window.show();

    return app.exec();
}