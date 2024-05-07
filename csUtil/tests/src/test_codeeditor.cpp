#include <cstdio>
#include <cstdlib>

#include <memory>

#include <QtWidgets/QApplication>

#include <QtExamples/CodeEditor.h>

using WidgetPtr = std::unique_ptr<QWidget>;

////// Main //////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  WidgetPtr w = std::make_unique<QtExamples::CodeEditor>();
  w->resize(640, 480);
  w->show();

  return app.exec();
}
