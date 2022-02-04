#include "tags_pane.h"

#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

const int kButtonLength = 30;

TagsPane::TagsPane() : QFrame()
{
    //top part of pane
    QWidget *topWidget = new QWidget;
    QHBoxLayout *topLayout = new QHBoxLayout;
    QLabel *text = new QLabel("Tags:");

    QWidget *inputWidget = new QWidget;
    QHBoxLayout *inputLayout = new QHBoxLayout;

    QLabel *inputText = new QLabel("Add tag:");
    inputBox = new QLineEdit;
    QPushButton *button = new QPushButton("+");
    button->setMaximumSize(QSize(kButtonLength, kButtonLength));
    connect(button, SIGNAL(clicked()),
            this, SLOT(onButtonClicked()));

    inputLayout->addWidget(inputText);
    inputLayout->addWidget(inputBox);
    inputLayout->addWidget(button);
    inputWidget->setLayout(inputLayout);

    topLayout->addWidget(text);
    topLayout->addWidget(inputWidget);
    topWidget->setLayout(topLayout);

    //bottom part of pane
    QWidget *bottomWidget = new QWidget;
    tagsLayout = new FlowLayout;
    tagsLayout->setAlignment({Qt::AlignLeft});
    bottomWidget ->setLayout(tagsLayout);

    //pane
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topWidget);
    layout->addWidget(bottomWidget);
    setLayout(layout);
    setFrameStyle(2);
}

void TagsPane::addTag(Tag *tag)
{
    tags.append(tag);
    tagsLayout->addWidget(tag);
    inputBox->clear();
}

void TagsPane::removeTag(QString tagName)
{
    for(int i = 0; i < tags.size();) {
        if(tags.at(i)->text() == tagName) {
            Tag *t = tags.at(i);
            tagsLayout->removeWidget(t);
            tags.removeAt(i);
            delete t;
        } else {
            i ++;
        }
    }
}

void TagsPane::onButtonClicked()
{
    QString name = inputBox->text();
    if (name == "") {
        inputBox->clear();
        return;
    }
    for (Tag* t: tags) {
        if (t->text().compare(name, Qt::CaseSensitivity::CaseInsensitive)==0) {
            inputBox->clear();
            return;
        }
    }
    Tag *tag = new Tag(name);
    addTag(tag);
    emit tagAdded(name);
}

void TagsPane::update(QStringList t){
    for (Tag* i: tags) {
        removeTag(i->text());
    }
    //add new ones
    for (QString s: t){
        addTag(new Tag(s));
    }
}
