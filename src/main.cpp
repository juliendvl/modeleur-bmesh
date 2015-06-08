#include <QApplication>
#include <QTextCodec>
#include "window.h"

int main(int argc, char** argv)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QApplication app(argc, argv);

    Window window;
    window.show();

    return app.exec();
}
