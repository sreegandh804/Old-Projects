#include "tag.h"
#include <QChar>

using namespace std;

int Tag::defColorIndex = 0;
const QSize Tag::kMaxSize = QSize(150, 30);
const QSize Tag::kMinSize = QSize(10, 30);

Tag::Tag(QString name)
{
    setText(name);
    setDefaultColor();
    setAlignment(Qt::AlignCenter);
    setMaximumSize(kMaxSize);
    setMinimumSize(kMinSize);
}

Tag::Tag(QString name, QString hexColor)
{
    QLabel();
    setText(name);
    setAlignment(Qt::AlignCenter);
    setMaximumSize(kMaxSize);
    setMinimumSize(kMinSize);

    //check if hex_color is formatted correctly as a hexadecimal color
    int ok = 1;
    if(hexColor.length() == 7) {
        if(hexColor.at(0) == '#') {
            for(int i = 1; i <= 6; i ++) {
                QChar c = hexColor.at(i);
                if(!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
                    ok = 0;
                }
            }
        } else {
            ok = 0;
        }
    } else {
        ok = 0;
    }

    if(ok == 1) {
        color = hexColor;
        setStyleSheet("background-color: " + color + "; border: 1px solid black");
    } else {
        setDefaultColor();
    }
}

void Tag::setColor(QString newColor)
{
    //check if hex_color is formatted correctly as a hexadecimal color
    int ok = 1;
    if(newColor.length() == 7) {
        if(newColor.at(0) == '#') {
            for(int i = 1; i <= 6; i ++) {
                QChar c = newColor.at(i);
                if(!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
                    ok = 0;
                }
            }
        } else {
            ok = 0;
        }
    } else {
        ok = 0;
    }

    if(ok == 1) {
        color = newColor;
        setStyleSheet("background-color: " + color + "; border: 1px solid black");
    }
}

void Tag::setDefaultColor()
{
    color = colors[defColorIndex];
    setStyleSheet("background-color: " + color + "; border: 1px solid black");

    defColorIndex ++;
    if(defColorIndex >= kDefColorNumber)
        defColorIndex %= kDefColorNumber;
}
