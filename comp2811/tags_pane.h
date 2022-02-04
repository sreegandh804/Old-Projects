#ifndef TAGS_PANE_H
#define TAGS_PANE_H

#endif // TAGS_PANE_H
#include <QWidget>
#include <QList>
#include <QPushButton>
#include <QLineEdit>
#include <QMediaPlayer>

#include "tag.h"
#include "flow_layout.h"
#include "video_button.h"

class TagsPane : public QFrame{
    Q_OBJECT

public:
    TagsPane();

    void addTag(Tag *t);
    void removeTag(QString tagName);

    Tag* getTagAt(int index);

    void update(QStringList tags);

private:
    QList<Tag*> tags;
    FlowLayout *tagsLayout;
    QLineEdit *inputBox;

signals:
    void tagAdded(QString name);

private slots:
    void onButtonClicked();
};
