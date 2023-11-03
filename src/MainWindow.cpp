#include "MainWindow.h"

MainWindow::MainWindow(TCPServer *server, QWidget *parent) :
        QMainWindow(parent),
        server(server),
        central_widget(new QWidget()),
        tabWidget(new QTabWidget()),
        grid(new QGridLayout()),
        clients(new QListWidget()),
        messages(new QListWidget()) {
    setWindowTitle("iMessage server");
    initWidgets();
    initLayout();
    initConnections();
}

void MainWindow::initWidgets() {
    DatabaseManager m;
    setCentralWidget(central_widget);
    central_widget->setLayout(grid);

    tabWidget->addTab(messages, "Messages");
    tabWidget->addTab(clients, "Clients");

    for (const auto& user : m.selectAllUsers()) {
        clients->addItem(user);
    }
}

void MainWindow::initLayout() {
    grid->addWidget(tabWidget);
}

void MainWindow::initConnections() {
    connect(server, &TCPServer::signalDisplayMessage, this, &MainWindow::slotAddMessageToList);
}

void MainWindow::slotAddMessageToList(QByteArray from, QByteArray to, QByteArray msg) {
    messages->addItem((from.toStdString() + " -> " + to.toStdString() + ": " + msg.toStdString()).c_str());
}
