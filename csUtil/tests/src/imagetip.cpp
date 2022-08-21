#include <QtGui/QHelpEvent>
#include <QtWidgets/QFileDialog>

#include <cs/Qt/ImageTip.h>

#include "imagetip.h"
#include "ui_imagetip.h"

////// public ////////////////////////////////////////////////////////////////

WMainWindow::WMainWindow(QWidget *parent, Qt::WindowFlags flags)
  : QMainWindow(parent, flags)
  , ui(new Ui::WMainWindow)
  , _image()
{
  ui->setupUi(this);

  ui->filenameEdit->installEventFilter(this);

  // Signals & Slots /////////////////////////////////////////////////////////

  connect(ui->quitAction, &QAction::triggered,
          this, &WMainWindow::close);

  connect(ui->openButton, &QPushButton::clicked,
          this, &WMainWindow::openFile);
}

WMainWindow::~WMainWindow()
{
  delete ui;
}

////// protected /////////////////////////////////////////////////////////////

bool WMainWindow::eventFilter(QObject *object, QEvent *event)
{
  // printf("test 1\n"); fflush(stdout);

  if( object != ui->filenameEdit  ||  event->type() != QEvent::ToolTip  ||
      _image.isNull() ) {
    return false;
  }

  cs::ImageTip::Flags flags = 0;
  if( ui->forcePositionCheck->isChecked() ) {
    flags |= cs::ImageTip::ForcePosition;
  }
  if( ui->drawBorderCheck->isChecked() ) {
    flags |= cs::ImageTip::DrawBorder;
  }
  if( ui->noEffectsCheck->isChecked() ) {
    flags |= cs::ImageTip::NoEffects;
  }

  QHelpEvent *helpEvent = dynamic_cast<QHelpEvent*>(event);

  QPoint pos = helpEvent->globalPos();
  if( flags.testFlag(cs::ImageTip::ForcePosition) ) {
    pos += QPoint(8, 8); // Some offset so the tip will not immediately hide.
  }

  cs::ImageTip::showImage(pos, _image, ui->filenameEdit, flags);

  return true;
}

////// private slots /////////////////////////////////////////////////////////

void WMainWindow::openFile()
{
  const QString filename =
      QFileDialog::getOpenFileName(this, QStringLiteral("Open"),
                                   QString(), QStringLiteral("Images (*.gif *.jpg *.png)"));
  if( filename.isEmpty() ) {
    return;
  }

  ui->filenameEdit->clear();
  if( _image.load(filename) ) {
    ui->filenameEdit->setText(filename);
  }
}
