#ifndef TAG_H
#define TAG_H

#endif // TAG_H
#include <QString>
#include <QLabel>

//default colors
const int kDefColorNumber = 7;

const QString
    purple = "#ffaaff",
    red = "#ff6e6e",
    green = "#aaffaa",
    pink = "#ffa9d3",
    orange = "#ffaa66",
    blue = "#99ddff",
    dark_blue = "#aaaaff";

const QString colors[] = {
    purple,
    red,
    green,
    pink,
    orange,
    blue,
    dark_blue
};

class Tag : public QLabel {
public:
    Tag(QString name);
    Tag(QString name, QString hexColor);

    QSize sizeHint() const { return minimumSize()+QSize(5,0); }

    //getters
    QString getColor() { return color; }

    //setters
    void setColor(QString newColor);

private:
    //color of tag background, in hex
    QString color;
    //used to cycle through default colors for new tags
    static int defColorIndex;
    //maximum and minimum size of tags
    static const QSize kMaxSize;
    static const QSize kMinSize;

    void setDefaultColor();

};
