#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    paths.append("styles");
    QCoreApplication::setLibraryPaths(paths);

    QFile styleF;

    styleF.setFileName(":/res/style.qss");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();

    QApplication a(argc, argv);
    a.setStyleSheet(qssStr);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "BTSearcher_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
