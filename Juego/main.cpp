#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // QApplication gestiona los recursos a nivel de aplicación y el flujo de control
    QApplication a(argc, argv);

    // Ventana principal
    MainWindow w;

    // Mostramos la ventana en la pantalla
    w.show();

    // Iniciamos el bucle de eventos principal de Qt
    return a.exec();
}
