#ifndef DRACULASTYLE_H
#define DRACULASTYLE_H

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QProxyStyle>
#include <QStyleFactory>

class DraculaStyle: public QProxyStyle {
    Q_OBJECT

   public:
    DraculaStyle();
    explicit DraculaStyle(QStyle *style);

    QStyle *baseStyle() const;

    void polish(QPalette &palette) override;
    void polish(QApplication *app) override;

   private:
    QStyle *styleBase(QStyle *style = Q_NULLPTR) const;
};

#endif // DRACULASTYLE_H
