/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <QLCDNumber>
#include <QComboBox>
#include <QIcon>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QTabWidget>
#include <QScrollArea>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QTimer>

#include "video_player.h"
#include "video_button.h"
#include "main_window.h"

using namespace std;

int main(int argc, char *argv[]) {

    // create the Qt Application
    QApplication app(argc, argv);

    // create the main window and layout
    MainWindow* window = new MainWindow(string(argv[1]));
    window->setWindowTitle("Tomeo");
    window->setMinimumSize(800, 680);

    // showtime!
    window->show();

    QTimer::singleShot(0, window, SLOT(onEventLoopStarted()));
    
    // wait for the app to terminate
    return app.exec();
}
