#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widget ventana;
    ventana.show();
    return app.exec();
}
