#include <cstdio>
#include <cstdlib>

#include <QtCore/QMetaObject>
#include <QtCore/QThread>
#include <QtWidgets/QApplication>

#include <cs/Logging/IProgress.h>
#include <cs/Logging/WProgressLogger.h>

////// Constants /////////////////////////////////////////////////////////////

constexpr int MAX_PROGRESS = 10;

////// Worker Thread /////////////////////////////////////////////////////////

class WorkerThread : public QThread {
public:
  WorkerThread() = default;
  ~WorkerThread() = default;

  volatile bool is_running{true};
  QObject *object{nullptr};

  void run()
  {
    while( is_running ) {
      if( object != nullptr ) {
        QMetaObject::invokeMethod(object, "stepValue", Qt::AutoConnection);
      }
      printf("."); fflush(stdout);
      sleep(1);
    }
    printf("\n"); fflush(stdout);
  }
};

////// Main //////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  cs::WProgressLogger proglog;
  proglog.progress()->setProgressRange(0, MAX_PROGRESS);

  WorkerThread thread;
  thread.object = &proglog;

  thread.start();
  proglog.exec();

  thread.is_running = false;
  thread.wait();

  return EXIT_SUCCESS;
}
