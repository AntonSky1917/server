#include <QApplication>
#include <QCommandLineParser>
#include "src/TCPServer.h"
#include "src/MainWindow.h"

// запускаем TCP-сервер на порту 6000.
// анализируем аргументы командной строки, чтобы получить информацию о подключении к базе данных
// создаем графический интерфейс и запускаем главный цикл приложения
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    TCPServer server(6000);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({
                              {{"host", "host"},
                                      QCoreApplication::translate("main", "host for postgresql db."),
                                      QCoreApplication::translate("main", "string")},
                              {{"u", "user"},
                                      QCoreApplication::translate("main", "user for postgresql db."),
                                      QCoreApplication::translate("main", "string")},
                              {{"d", "database_name"},
                                      QCoreApplication::translate("main", "database name for postgresql db."),
                                      QCoreApplication::translate("main", "string")},
                              {{"p", "password"},
                                      QCoreApplication::translate("main", "password for postgresql db."),
                                      QCoreApplication::translate("main", "string")},
                              {{"port", "port"},
                                      QCoreApplication::translate("main", "port for postgresql db. Default: 5432"),
                                      QCoreApplication::translate("main", "int")},
                      });
    parser.process(app);
    if (!parser.isSet("host") || !parser.isSet("user") || !parser.isSet("database_name") || !parser.isSet("password")) {
        parser.showHelp(1);
        return 1;
    } else {
        DatabaseInfo::host = parser.value("host");
        DatabaseInfo::user = parser.value("user");
        DatabaseInfo::database_name = parser.value("database_name");
        DatabaseInfo::password = parser.value("password");
        DatabaseInfo::port = (parser.value("port").isEmpty() ? 5432 : parser.value("port").toInt());
    }

    MainWindow w(&server);
    w.show();
    return QApplication::exec();
}
