#ifndef CANDYSTYLE_H
#define CANDYSTYLE_H

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QProxyStyle>
#include <QStyleFactory>

class CandyStyle: public QProxyStyle {
    Q_OBJECT

   public:
    CandyStyle();
    explicit CandyStyle(QStyle *style);

    QStyle *baseStyle() const;

    void polish(QPalette &palette) override;
    void polish(QApplication *app) override;

   private:
    QStyle *styleBase(QStyle *style = Q_NULLPTR) const;
  };

#endif // CANDYSTYLE_H
