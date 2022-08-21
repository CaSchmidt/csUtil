#include <cstdio>
#include <cstdlib>

#include <QtWidgets/QApplication>

#include <cs/Qt/WListEditor.h>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  cs::WListEditor *editor = new cs::WListEditor();
  editor->setWindowTitle(QStringLiteral("cs::WListEditor"));
  editor->resize(640, 480);
  editor->show();

  const int result = app.exec();

  delete editor;

  return result;
}
