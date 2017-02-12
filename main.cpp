#include "fmdb.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  fmdb w;
  w.show();

  return a.exec();
}
