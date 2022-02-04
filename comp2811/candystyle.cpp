#include "candystyle.h"

CandyStyle::CandyStyle() : CandyStyle(styleBase()) {}

CandyStyle::CandyStyle(QStyle *style) : QProxyStyle(style) {}

QStyle *CandyStyle::styleBase(QStyle *style) const {
  static QStyle *base =
      !style ? QStyleFactory::create(QStringLiteral("Fusion")) : style;
  return base;
}

QStyle *CandyStyle::baseStyle() const { return styleBase(); }

void CandyStyle::polish(QPalette &palette) {
  // modify palette to dark
    palette.setColor(QPalette::Window, QColor(237, 231, 177));
    palette.setColor(QPalette::WindowText, QColor(138, 89, 47));
    palette.setColor(QPalette::Disabled, QPalette::WindowText,
                     QColor(127, 127, 127));
    palette.setColor(QPalette::Base, QColor(228, 193, 249));
    palette.setColor(QPalette::AlternateBase, QColor(255, 61, 150));
    palette.setColor(QPalette::ToolTipBase, Qt::black);
    palette.setColor(QPalette::ToolTipText, QColor(138, 89, 47));
    palette.setColor(QPalette::Text, QColor(138, 89, 47));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    palette.setColor(QPalette::Light, QColor(255, 61, 150));
    palette.setColor(QPalette::Shadow, QColor(20, 20, 20));
    palette.setColor(QPalette::Button, QColor(255, 61, 150));
    palette.setColor(QPalette::ButtonText, QColor(138, 89, 47));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,
                     QColor(127, 127, 127));
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(211, 248, 226));
    palette.setColor(QPalette::Highlight, QColor(211, 248, 226));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText,
                     QColor(127, 127, 127));
}

void CandyStyle::polish(QApplication *app) {
  if (!app) return;

  // increase font size for better reading,
  // setPointSize was reduced from +2 because when applied this way in Qt5, the
  // font is larger than intended for some reason
  QFont defaultFont = QApplication::font();
  defaultFont.setPointSize(defaultFont.pointSize() + 1);
  app->setFont(defaultFont);

  // loadstylesheet
  QFile qfCandystyle(QStringLiteral(":/darkstyle/darkstyle.qss"));
  if (qfCandystyle.open(QIODevice::ReadOnly | QIODevice::Text)) {
    // set stylesheet
    QString qsStylesheet = QString::fromLatin1(qfCandystyle.readAll());
    app->setStyleSheet(qsStylesheet);
    qfCandystyle.close();
  }
}
