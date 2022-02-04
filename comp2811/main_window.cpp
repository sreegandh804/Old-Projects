#include "main_window.h"
#include <QtGui>

#include <QFileSystemModel>
#include <QScreen>

#include <QVideoWidget>
#include <QPushButton>
#include <QLCDNumber>
#include <QMediaMetaData>
#include <QMediaPlaylist>
#include <QtCore/QDateTime>

#include "flow_layout.h"

MainWindow::MainWindow(std::string path): path(path) {
    videos = importVideos();

    createActions();
    createMenus();
    createWidgets();
    arrangeWidgets();
}

//import all videos in the specified folder
vector<VideoButtonData> MainWindow::importVideos() {
    vector<VideoButtonData> out =  vector<VideoButtonData>();
        QDir directory (QString::fromStdString(path));
        QDirIterator directoryIterator (directory, QDirIterator::Subdirectories);

        while (directoryIterator.hasNext()) { // for all files
            QString file = directoryIterator.next();
                if (file.contains("."))

    #if defined(_WIN32)
                if (file.contains(".wmv")) { // windows
    #else
                if (file.contains(".mp4") || file.contains("MOV"))  { // mac/linux
    #endif
                    QString thumbnail = file.left(file.length() - 4) +".png";
                    if (QFile(thumbnail).exists()) {
                        QImageReader *imageReader = new QImageReader(thumbnail);
                            QImage sprite = imageReader->read();
                            if (!sprite.isNull()) {
                                QIcon* icon = new QIcon(QPixmap::fromImage(sprite));
                                QUrl* url = new QUrl(QUrl::fromLocalFile(file));
                                out.push_back(VideoButtonData(url, icon));
                            }else
                                qDebug() << "Warning: "<< file <<
                                        "'s thumbnail cannot be processed" << Qt::endl;
                    }else
                        qDebug() << "Warning: "<< file << "'s thumbnail cannot be processed" << Qt::endl;
                }
        }
        return out;
}

//create action to be used in the menu bar
void MainWindow::createActions() {

    editPrefAct = new QAction(tr("&Edit Video Preferences"), this);
    connect(editPrefAct, SIGNAL(triggered()), this, SLOT(editPref()));

    editThemePrefAct = new QAction(tr("&Edit Theme Preferences"), this);
    connect(editThemePrefAct, SIGNAL(triggered()), this, SLOT(editThemePref()));

    saveLayoutAct = new QAction(tr("&Save Current Layout"), this);

    importMediaAct = new QAction(tr("&Import Media"), this);
    connect(importMediaAct, SIGNAL(triggered()), this, SLOT(importMedia()));

    importDirectoryAct = new QAction(tr("&Import Folder"), this);
    connect(importDirectoryAct, SIGNAL(triggered()), this, SLOT(importDirectory()));

    exportToFolderAct = new QAction(tr("&Export To Folder"), this);

    exportToSocialMediaAct = new QAction(tr("Export To Social Media"), this);

    editPrefAct = new QAction(tr("&Edit Video Preferences"), this);
    connect(editPrefAct, SIGNAL(triggered()), this, SLOT(editPref()));

    editThemePrefAct = new QAction(tr("&Edit Theme Preferences"), this);
    connect(editThemePrefAct, SIGNAL(triggered()), this, SLOT(editThemePref()));

    saveLayoutAct = new QAction(tr("&Save Current Layout"), this);

    cropAct = new QAction(tr("Crop"), this);

    changeSpeedAct = new QAction(tr("Change Speed"), this);

    changeVolumeAct = new QAction(tr("Change Volume Settings"), this);

    viewDocsAct = new QAction(tr("&Documentation"), this);

    searchHelpAct = new QAction(tr("&Search Help"), this);
    connect(searchHelpAct, SIGNAL(triggered()), this, SLOT(searchHelp()));

}

//create the menu bar using the actions
void MainWindow::createMenus() {
    menuBar()->setNativeMenuBar(false);

    FileMenu = menuBar()->addMenu(tr("&File"));
    FileMenu->addAction(importMediaAct);
    FileMenu->addAction(importDirectoryAct);
    FileMenu->addAction(exportToFolderAct);
    FileMenu->addAction(exportToSocialMediaAct);

    prefMenu = menuBar()->addMenu(tr("&Preferences"));
    prefMenu->addAction(editPrefAct);
    prefMenu->addAction(editThemePrefAct);
    prefMenu->addAction(saveLayoutAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(cropAct);
    toolsMenu->addAction(changeSpeedAct);
    toolsMenu->addAction(changeVolumeAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(viewDocsAct);
    helpMenu->addSeparator();
    helpMenu->addAction(searchHelpAct);
}

//create the widgets to be added to central
void MainWindow::createWidgets() {
    metadata = new QTableWidget();
    tagsPane = new TagsPane();
    createVideoPlayer();
    createVideoGrid();
    createFileHierarchy();

    connect(tagsPane, SIGNAL(tagAdded(QString)), this, SLOT(tagAdded(QString)));

    // tell the player what buttons and videos are available
    player->setContent(&buttons);
}

//set the layout of the widgets
void MainWindow::arrangeWidgets() {
    QVBoxLayout *layout3 = new QVBoxLayout;
    layout3->addWidget(tagsPane);
    layout3->addWidget(metadata);
    layout3->addWidget(tree);
    tagsPane->setMaximumWidth(500);
    metadata->setMaximumWidth(500);
    tree->setMaximumWidth(500);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addWidget(videoWrapper, 1);
    layout2->addWidget(videoGrid, 0);
    videoGrid->setMinimumHeight(200);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(layout3);
    layout->addLayout(layout2);

    QFrame *central = new QFrame;
    central->setLayout(layout);

    setCentralWidget(central);
    resize(1000, 600);
}

//create the viceo player widget
void MainWindow::createVideoPlayer() {
    // the widget that will show the video
    videoWidget = new QVideoWidget;
    QSlider* slider;
    QSlider* volbar;
    QSlider* speedbar;
    slider = new QSlider(Qt::Horizontal);
    volbar = new QSlider(Qt::Horizontal);
    speedbar = new QSlider(Qt::Horizontal);
    slider->setMinimumWidth(400);
    volbar->setMaximumWidth(100);
    speedbar->setMaximumWidth(80);

    //playlist holding all the videos
    QMediaPlaylist* playlist = new QMediaPlaylist;

    // the QMediaPlayer which controls the playback
    player = new VideoPlayer;
    player->setVideoOutput(videoWidget);
    player->setVolume(100);
    player->setPlaylist(playlist);
    player->setPlaybackRate(1);

    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(onMediaError(QMediaPlayer::Error)));

    // Buttons in the QHBox of the video player
    QPushButton *button1 = new QPushButton("Pause");
    button1->setMinimumSize(40,40);
    button1->connect(button1, &QPushButton::clicked, player, &QMediaPlayer::pause);

    QPushButton *button2 = new QPushButton("Play");
    button2->setMinimumSize(40,40);
    button2->connect(button2, &QPushButton::clicked, player, &QMediaPlayer::play);

    QLCDNumber* number;
    number = new QLCDNumber(5);
    number->setMaximumHeight(30);

    QComboBox *combo = new QComboBox;
    combo->addItem("0.5x", QVariant(0.5));
    combo->addItem("1x", QVariant(1));
    combo->addItem("1.5x", QVariant(1.5));
    combo->addItem("2x", QVariant(2));
    combo->setCurrentIndex(1);
    combo->setMaximumWidth(60);
    combo->connect(combo, QOverload<int>::of(&QComboBox::activated), player, &QMediaPlayer::setPlaybackRate);


    QPushButton *button3 = new QPushButton("Mute");
    button3->setMinimumSize(40,40);
    button3->connect(button3, &QPushButton::clicked, player, &QMediaPlayer::setVolume);


    player->connect(player, &QMediaPlayer::durationChanged, slider, &QSlider::setMaximum);
    player->connect(player, &QMediaPlayer::positionChanged, slider, &QSlider::setValue);
    slider->connect(slider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);
    slider->connect(slider, SIGNAL(valueChanged(int)), number, SLOT(display(int)));
    volbar->connect(volbar, &QSlider::sliderMoved, player, &QMediaPlayer::setVolume);
    speedbar->connect(speedbar, &QSlider::sliderMoved, player, &QMediaPlayer::setPlaybackRate);

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(button1);
    layout1->addWidget(button2);
    layout1->addWidget(number);
    layout1->addWidget(slider);
    layout1->addWidget(combo);
    layout1->addWidget(button3);
    layout1->addWidget(volbar);

    QVBoxLayout *topPlayer = new QVBoxLayout();

    // add the video and the buttons to the top level widget
    topPlayer->addWidget(videoWidget);
    topPlayer->addLayout(layout1);

    videoWrapper = new QFrame;
    videoWrapper->setLayout(topPlayer);
    videoWrapper->setFrameStyle(2);
}

//create the file hierarchy
void MainWindow::createFileHierarchy() {
    //create a file hierarchy
    QFileSystemModel *model = new QFileSystemModel;
    QString qstr = QString::fromStdString(path);
    model->setRootPath(qstr);
    QModelIndex idx = model->index(qstr);
    tree = new QTreeView();
    tree->setModel(model);
    tree->setRootIndex(idx);
    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
}

void MainWindow::createVideoGrid() {

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    QScrollArea *grid = new QScrollArea;
    grid->setWidgetResizable(true);

    grid->setWidget(buttonWidget);
    grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // the buttons are arranged in a grid

    layoutGrid = new Video_grid_layout();
    buttonWidget->setLayout(layoutGrid);

    // create the buttons
    for ( int i = 0; i < (int)videos.size(); i++ ) {
        VideoButton *button = new VideoButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(int)), player, SLOT (jumpTo(int))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layoutGrid->addWidget(button);
        button->init(&videos.at(i), i);

    }
    layoutGrid->sort(0);

    buttonWidget->resize(layoutGrid->sizeHint());

    videoGrid = new QFrame();
    QHBoxLayout *l1 = new QHBoxLayout();

    l1->addWidget(new QLabel("Search tag:"));
    QLineEdit *lineEdit = new QLineEdit();
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(searchTags(QString)));
    l1->addWidget(lineEdit);

    l1->addWidget(new QLabel("Sort by::"));
    sortComboBox = new QComboBox();
    l1->addWidget(sortComboBox);
    l1->addStretch(2);

    sortComboBox->addItem("Title");
    sortComboBox->addItem("Duration");
    sortComboBox->addItem("Date");
    connect(sortComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sortVideos(int)));

    QVBoxLayout *l2 = new QVBoxLayout();
    l2->addLayout(l1);
    l2->addWidget(grid);
    videoGrid->setLayout(l2);
    videoGrid->setFrameStyle(2);
}

void MainWindow::updateMetadata() {
    QStringList metadatalist = player->availableMetaData();
    metadata->setRowCount(metadatalist.size()+1);
    metadata->setColumnCount(1);

    QTableWidgetItem *newItem_meta = new QTableWidgetItem(tr("Meta Data"));
    metadata->setHorizontalHeaderItem(0,newItem_meta);

    QTableWidgetItem *newItem = new QTableWidgetItem(tr("Title"));
    QTableWidgetItem *newItemv = new QTableWidgetItem(player->buttons->at(player->playlist()->currentIndex())->info->title);
    metadata->setVerticalHeaderItem(0,newItem);
    metadata->setItem(0,0,newItemv);

    for (int i = 0; i < metadatalist.size(); i++) {
        QTableWidgetItem *newItem = new QTableWidgetItem(metadatalist.at(i));
        QTableWidgetItem *newItemv = new QTableWidgetItem(player->metaData(metadatalist.at(i)).toString());
        metadata->setVerticalHeaderItem(i+1,newItem);
        metadata->setItem(i+1,0,newItemv);
    }

    metadata->resize(metadata->sizeHint());


}

void MainWindow::editPref() {
    QSlider *brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(-100, 100);
    brightnessSlider->setValue(videoWidget->brightness());
    connect(brightnessSlider, &QSlider::sliderMoved, videoWidget, &QVideoWidget::setBrightness);
    connect(videoWidget, &QVideoWidget::brightnessChanged, brightnessSlider, &QSlider::setValue);

    QSlider *contrastSlider = new QSlider(Qt::Horizontal);
    contrastSlider->setRange(-100, 100);
    contrastSlider->setValue(videoWidget->contrast());
    connect(contrastSlider, &QSlider::sliderMoved, videoWidget, &QVideoWidget::setContrast);
    connect(videoWidget, &QVideoWidget::contrastChanged, contrastSlider, &QSlider::setValue);

    QSlider *hueSlider = new QSlider(Qt::Horizontal);
    hueSlider->setRange(-100, 100);
    hueSlider->setValue(videoWidget->hue());
    connect(hueSlider, &QSlider::sliderMoved, videoWidget, &QVideoWidget::setHue);
    connect(videoWidget, &QVideoWidget::hueChanged, hueSlider, &QSlider::setValue);

    QSlider *saturationSlider = new QSlider(Qt::Horizontal);
    saturationSlider->setRange(-100, 100);
    saturationSlider->setValue(videoWidget->saturation());
    connect(saturationSlider, &QSlider::sliderMoved, videoWidget, &QVideoWidget::setSaturation);
    connect(videoWidget, &QVideoWidget::saturationChanged, saturationSlider, &QSlider::setValue);
    QFormLayout *layoutc = new QFormLayout;
    layoutc->addRow(tr("Brightness"), brightnessSlider);
    layoutc->addRow(tr("Contrast"), contrastSlider);
    layoutc->addRow(tr("Hue"), hueSlider);
    layoutc->addRow(tr("Saturation"), saturationSlider);
    QDialog *m_videoDialog = new QDialog;
    m_videoDialog->setWindowTitle(tr("Video Options"));
    m_videoDialog->setLayout(layoutc);
    m_videoDialog->exec();

}

void MainWindow::editThemePref() {

    QGroupBox *groupBox1 = new QGroupBox(tr("Default Theme Options"));
    QRadioButton *radio1 = new QRadioButton(tr("&Dark Theme"));
    QRadioButton *radio3 = new QRadioButton(tr("&Dracula Theme"));
    QRadioButton *radio4 = new QRadioButton(tr("&Default"));

    connect(radio1, SIGNAL(clicked()), this, SLOT(dark()));
    connect(radio3, SIGNAL(clicked()), this, SLOT(dracula()));
    connect(radio4, SIGNAL(clicked()), this, SLOT(def()));

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(radio1);
    vbox2->addWidget(radio3);
    vbox2->addWidget(radio4);
    vbox2->addStretch(1);
    groupBox1->setLayout(vbox2);

    QGroupBox *groupBox2 = new QGroupBox(tr("Custom Theme Options"));
    QPushButton *color1 = new QPushButton;
    color1->setStyleSheet("background-color: yellow");
    QPushButton *color2 = new QPushButton;
    color2->setStyleSheet("background-color: red");
    QPushButton *color3 = new QPushButton;
    color3->setStyleSheet("background-color: blue");
    QPushButton *color4 = new QPushButton;
    color4->setStyleSheet("background-color: green");
    QPushButton *color5 = new QPushButton;
    color5->setStyleSheet("background-color: brown");
    QPushButton *color6 = new QPushButton;
    color6->setStyleSheet("background-color: grey");
    QPushButton *color7 = new QPushButton;
    color7->setStyleSheet("background-color: pink");
    QPushButton *color8 = new QPushButton;
    color8->setStyleSheet("background-color: violet");
    QPushButton *color9 = new QPushButton;
    color9->setStyleSheet("background-color: purple");
    QPushButton *color10 = new QPushButton;
    color10->setStyleSheet("background-color: orange");

    connect(color1, SIGNAL(clicked()), this, SLOT(color1()));
    connect(color2, SIGNAL(clicked()), this, SLOT(color2()));
    connect(color3, SIGNAL(clicked()), this, SLOT(color3()));
    connect(color4, SIGNAL(clicked()), this, SLOT(color4()));
    connect(color5, SIGNAL(clicked()), this, SLOT(color5()));
    connect(color6, SIGNAL(clicked()), this, SLOT(color6()));
    connect(color7, SIGNAL(clicked()), this, SLOT(color7()));
    connect(color8, SIGNAL(clicked()), this, SLOT(color8()));
    connect(color9, SIGNAL(clicked()), this, SLOT(color9()));
    connect(color10, SIGNAL(clicked()), this, SLOT(color10()));

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(color1, 0, 0);
    grid->addWidget(color2, 0, 1);
    grid->addWidget(color3, 0, 2);
    grid->addWidget(color4, 0, 3);
    grid->addWidget(color5, 0, 4);
    grid->addWidget(color6, 1, 0);
    grid->addWidget(color7, 1, 1);
    grid->addWidget(color8, 1, 2);
    grid->addWidget(color9, 1, 3);
    grid->addWidget(color10, 1, 4);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addItem(grid);
    groupBox2->setLayout(hbox);

    QDialog *m_themeDialog = new QDialog;
    m_themeDialog->setWindowTitle(tr("Theme Options"));

    QFormLayout *layoutr = new QFormLayout;
    layoutr->addWidget(groupBox1);
    layoutr->addWidget(groupBox2);
    layoutr->setLabelAlignment(Qt::AlignLeft);
    m_themeDialog->setLayout(layoutr);
    m_themeDialog->exec();
}

void MainWindow::saveLayout() {

}

void MainWindow::importMedia() {
    QString mediaPath = QString::fromStdString(path);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Media"), QDir::homePath(),
                                                    tr("Videos (*.mp4 *.wmv *.mov);;"
                                                       "Images(*.jpg *.png *.bmp)"));
    if (fileName != "") {
        QString DestinationDirectory = QFileDialog::getExistingDirectory(this,
                                                                         tr("Select Directory to"
                                                                            "save you file to"),
                                                                         mediaPath);
        QStringList filePath = fileName.split("/");
        QString filename = filePath.at(filePath.size()-1);
        QFile::copy(fileName, DestinationDirectory+QDir::separator()+filename);

        videos = importVideos();
    }
}

void MainWindow::importDirectory() {
    QString destinationDirectory = QString::fromStdString(path);
    QString sourceDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                              QDir::homePath());
    QStringList directoryPath = sourceDirectory.split("/");
    QString directory = directoryPath.at(directoryPath.size() - 1);
    destinationDirectory = destinationDirectory + QDir::separator() + directory;
    copyDirectory(sourceDirectory, destinationDirectory);
}

void MainWindow::copyDirectory(QString source, QString destination) {
    QDir sourceDirectory(source);
    QDir destinationDirectory(destination);

    if (!sourceDirectory.exists())
        return;

    if (!destinationDirectory.exists()) {
        destinationDirectory.mkdir(destination);
    }

    foreach (QString file, sourceDirectory.entryList(QDir::Files)) {
        QFile::copy(source + QDir::separator() + file, destination + QDir::separator() + file);
    }

    foreach (QString directory, sourceDirectory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        copyDirectory(source + QDir::separator() + directory,
                      destination + QDir::separator() + directory);
    }
}

void MainWindow::exportToFolder() {

}

void MainWindow::exportToSocialMedia() {

}

void MainWindow::toolsSelect() {

}

void MainWindow::viewDocs() {

}

void MainWindow::searchHelp() {

    QVBoxLayout* lay = new QVBoxLayout;

    QLabel *label = new QLabel;
    //QPixmap pm(":/help.png"); // <- path to image file
    //label->setPixmap(pm);
    label->setScaledContents(true);
    label->setMaximumSize(700,400);

    QPushButton *contact = new QPushButton("Contact Us");
    QPushButton *feedback = new QPushButton("Feedback");
    QPushButton *what = new QPushButton("What's New");
    QHBoxLayout *hor = new QHBoxLayout;
    hor->addWidget(contact);
    hor->addWidget(feedback);
    hor->addWidget(what);

    lay->addLayout(hor);
    lay->addWidget(label);

    QDialog dialog(this);
    dialog.setModal(true);
    dialog.setLayout(lay);
    dialog.setWindowTitle("Help");

    dialog.exec();
}

void MainWindow::tagAdded(QString name){
    player->buttons->at(player->playlist()->currentIndex())->info->tags.append(name);
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status){
    if (status == QMediaPlayer::BufferedMedia){
        if (loading) {
            //read tags
            QStringList tags = player->metaData(QMediaMetaData::Keywords).toStringList();
            player->buttons->at(player->playlist()->currentIndex())->info->tags = tags;
            player->buttons->at(player->playlist()->currentIndex())->info->duration = player->metaData(QMediaMetaData::Duration).toInt();
            QFileInfo info(player->buttons->at(player->playlist()->currentIndex())->info->url->toString());
            player->buttons->at(player->playlist()->currentIndex())->info->title = info.baseName();

            counter++;
            if (counter < player->playlist()->mediaCount()) {
                player->jumpTo(counter);
            } else {
                loading = false;
                player->jumpTo(0);
                player->pause();
            }
        } else {
            metadata->clear();
            metadata->setRowCount(0);
            updateMetadata();

            tagsPane->update(player->buttons->at(player->playlist()->currentIndex())->info->tags);
        }
    }

}

void MainWindow::onMediaError(QMediaPlayer::Error e) {
    qDebug() << e;
}

void MainWindow::onEventLoopStarted() {
    player->jumpTo(counter);
}

void MainWindow::sortVideos(int index) {
    layoutGrid->sort(index);
}

void MainWindow::searchTags(QString text) {
    layoutGrid->search(text);
}

void MainWindow::dark(){
    QApplication::setStyle(new DarkStyle);
}
void MainWindow::dracula(){
    QApplication::setStyle(new DraculaStyle);
}
void MainWindow::def(){
    QApplication::setStyle(QStyleFactory::keys().at(0));
}
void MainWindow::color1(){
    window()->setStyleSheet("color: black; background-color: yellow");
}
void MainWindow::color2(){
    window()->setStyleSheet("color: blue; background-color: red");
}
void MainWindow::color3(){
    window()->setStyleSheet("color: red; background-color: blue");
}
void MainWindow::color4(){
    window()->setStyleSheet("color: black; background-color: green");
}
void MainWindow::color5(){
    window()->setStyleSheet("color: black; background-color: brown");
}
void MainWindow::color6(){
    window()->setStyleSheet("color: black; background-color: grey");
}
void MainWindow::color7(){
    window()->setStyleSheet("color: black; background-color: pink");
}
void MainWindow::color8(){
    window()->setStyleSheet("color: black; background-color: violet");
}
void MainWindow::color9(){
    window()->setStyleSheet("color: black; background-color: purple");
}
void MainWindow::color10(){
    window()->setStyleSheet("color: black; background-color: orange");
}
