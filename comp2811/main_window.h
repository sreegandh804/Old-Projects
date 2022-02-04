#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QActionGroup>
#include <QMenuBar>
#include <QTableWidget>
#include <QFileDialog>
#include <string>
#include <vector>
#include <QTreeView>
#include <QScrollArea>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QRadioButton>

#include "video_player.h"
#include "video_button.h"
#include "tags_pane.h"
#include "video_grid_layout.h"
#include "darkstyle.h"
#include "draculastyle.h"
//#include "candystyle.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(std::string path);

//protected:
    //void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void editPref();
    void editThemePref();
    void saveLayout();
    void importMedia();
    void importDirectory();
    void copyDirectory(QString source, QString destination);
    void exportToFolder();
    void exportToSocialMedia();
    void toolsSelect();
    void viewDocs();
    void searchHelp();

    void tagAdded(QString name);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onMediaError(QMediaPlayer::Error e);
    void onEventLoopStarted();
    void sortVideos(int index);
    void searchTags(QString text);

    void dark();
    void dracula();
    //void candy();
    void def();

    void color1();
    void color2();
    void color3();
    void color4();
    void color5();
    void color6();
    void color7();
    void color8();
    void color9();
    void color10();

private:
    void createActions();
    void createMenus();
    void createWidgets();
    void arrangeWidgets();

    vector<VideoButtonData> importVideos();
    void createVideoPlayer();
    void createFileHierarchy();
    void createVideoGrid();
    void updateMetadata();

    //Widgets
    QFrame *videoWrapper;
    QTreeView *tree;
    TagsPane* tagsPane;
    QTableWidget* metadata;
    QFrame *videoGrid;

    QMenu *FileMenu;
    QMenu *prefMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QAction *editPrefAct;
    QAction *editThemePrefAct;
    QAction *saveLayoutAct;
    QAction *importMediaAct;
    QAction *importDirectoryAct;
    QAction *exportToFolderAct;
    QAction *exportToSocialMediaAct;
    QAction *cropAct;
    QAction *changeSpeedAct;
    QAction *changeVolumeAct;
    QAction *viewDocsAct;
    QAction *searchHelpAct;

    std::string path;
    std::vector<VideoButton*> buttons;
    std::vector<VideoButtonData> videos;
    VideoPlayer *player;
    bool loading = true;
    int counter = 0; //secrets
    Video_grid_layout *layoutGrid;
    QComboBox *sortComboBox;
    QVideoWidget *videoWidget;

signals:

};



#endif // MAIN_WINDOW_H
