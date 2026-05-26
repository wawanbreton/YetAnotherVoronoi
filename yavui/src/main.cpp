#include <QApplication>
#include <QLabel>

#include <string>

#include "yav/yav.hpp"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  QLabel label(QString::fromStdString("yavui using " + yav::version()));
  label.resize(320, 80);
  label.show();

  return app.exec();
}
