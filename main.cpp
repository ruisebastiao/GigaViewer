#include <QtGui>
#include <QGLWidget>

#include "mainwindow.h"
#include "maingui.h"
#include "coordinator.h"
#include "imagepacket.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<ImagePacket>("ImagePacket");

    MainWindow win;
    MainGui view;
    Coordinator theBoss(&view);
    win.setCentralWidget(&view);

    QShortcut *fullToggle = new QShortcut(QKeySequence(Qt::Key_F),&win);
    QShortcut *closeEv = new QShortcut(QKeySequence(Qt::Key_Escape),&win);

    QObject::connect(&win,SIGNAL(windowClosed()),&theBoss,SLOT(stopAcquisition()));
    QObject::connect(&view,SIGNAL(closeApplic()),&win,SLOT(close()));
    QObject::connect(fullToggle,SIGNAL(activated()),&win,SLOT(toggleFullscreen()));
    QObject::connect(closeEv,SIGNAL(activated()),&win,SLOT(close()));

    win.showFullScreen();

    if (argc==2) {
        if (QFile::exists(argv[1])) view.newMoviePressed(argv[1]);
    }

    return a.exec();
}
