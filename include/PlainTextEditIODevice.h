#ifndef QTEXTWIDGET_H
#define QTEXTWIDGET_H

#include <QIODevice>
#include <QPlainTextEdit>

////////////////////////////////////////////////////////////////////////////////
// create QIODevice to enable redirection of textstream to QPlainTextEdit widget
////////////////////////////////////////////////////////////////////////////////
//
// usage: to link widget and IODevice in constructor of dialogue
//
// 1.) create widget:
// QPlainTextEdit *textWidget = new QPlainTextEdit;
//
// 2.) define IODevice and link it to the widget when calling it's constructor:
// PlainTextEditIODevice *textDevice = new PlainTextEditIODevice(textWidget, this);
//
// 3.) To print text to the textWidget linkt the textDevice to a QTextStream:
// QTextStream qout(textDevice);
// qout << "formated text";
//
////////////////////////////////////////////////////////////////////////////////
class PlainTextEditIODevice : public QIODevice {
  Q_OBJECT
public:
  PlainTextEditIODevice(QPlainTextEdit* const textWidget, QObject* const parent = 0) :
      QIODevice(parent), textWidget(textWidget) {
    open(QIODevice::WriteOnly | QIODevice::Text);
  }

protected:
  qint64 readData(char* data, qint64 maxSize) { return 0; }
  qint64 writeData(const char* data, qint64 maxSize) {
    if (textWidget) { textWidget->insertPlainText(data); }
    return maxSize;
  }

private:
  QPlainTextEdit* const textWidget;
};

#endif  // QTEXTWIDGET_H
