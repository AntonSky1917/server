#ifndef IMESSAGESERVER_MAINWINDOW_H
#define IMESSAGESERVER_MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QTabWidget>
#include <QListWidget>
#include "TCPServer.h"
#include "DatabaseManager.h"

class MainWindow : public QMainWindow {
Q_OBJECT;
public:
    explicit MainWindow(TCPServer* server, QWidget* parent = nullptr);

public slots:
    void slotAddMessageToList(QByteArray, QByteArray, QByteArray);

private:
    void initWidgets();
    void initLayout();
    void initConnections();

    QWidget *central_widget;
    QGridLayout *grid;
    QTabWidget* tabWidget;
    TCPServer* server;

    QListWidget* clients;
    QListWidget* messages;
};


#endif //IMESSAGESERVER_MAINWINDOW_H
