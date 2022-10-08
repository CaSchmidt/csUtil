#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <QtWidgets/QApplication>

#include <Plot/PlotTheme.h>
#include <Plot/PlotWidget.h>

#include "TestSignal.h"

void test_findLeft(const plot::IPlotSeriesData *data)
{
  printf("LEFT: #%d\n", data->size());
  for(int i = 0; i < data->size(); i++) {
    const qreal x = data->valueX(i);
    const int hit = data->findLeft(x);
    printf("x[%3d] = %.3f -> hit = %3d %s\n",
           i, x, hit, hit != i ? "ERROR" : "");
  }
  printf("LEFT-MID\n");
  for(int i = 0; i < data->size()-1; i++) {
    const qreal x = (data->valueX(i) + data->valueX(i+1))/2.0;
    const int hit = data->findLeft(x);
    printf("x[%3d] = %.3f -> hit = %3d %s\n",
           i, x, hit, hit != i ? "ERROR" : "");
  }
  printf("LEFT-OUT\n");
  const qreal  outLeft = data->valueX(0)                - 1;
  printf("findLeft(%.3f) -> %3d\n", outLeft, data->findLeft(outLeft));
  const qreal outRight = data->valueX(data->size() - 1) + 1;
  printf("findLeft(%.3f) -> %3d\n", outRight, data->findLeft(outRight));
  fflush(stdout);
}

void test_findRight(const plot::IPlotSeriesData *data)
{
  printf("RIGHT: #%d\n", data->size());
  for(int i = 0; i < data->size(); i++) {
    const qreal x = data->valueX(i);
    const int hit = data->findRight(x);
    printf("x[%3d] = %.3f -> hit = %3d %s\n",
           i, x, hit, hit != i ? "ERROR" : "");
  }
  printf("RIGHT-MID\n");
  for(int i = 1; i < data->size(); i++) {
    const qreal x = (data->valueX(i-1) + data->valueX(i))/2.0;
    const int hit = data->findRight(x);
    printf("x[%3d] = %.3f -> hit = %3d %s\n",
           i, x, hit, hit != i ? "ERROR" : "");
  }
  printf("RIGHT-OUT\n");
  const qreal  outLeft = data->valueX(0)                - 1;
  printf("findRight(%.3f) -> %3d\n", outLeft, data->findRight(outLeft));
  const qreal outRight = data->valueX(data->size() - 1) + 1;
  printf("findRight(%.3f) -> %3d\n", outRight, data->findRight(outRight));
  fflush(stdout);
}

void info(const plot::IPlotSeriesData *data)
{
  printf("%s [%s]: rangeX = [%.3f,%.3f], rangeY = [%.3f,%.3f]\n",
         qPrintable(data->name()), qPrintable(data->unit()),
         data->rangeX().begin, data->rangeX().end,
         data->rangeY().begin, data->rangeY().end);
  fflush(stdout);
}

void print(const plot::PlotRange& r)
{
  printf("range = [%.3f,%.3f]\n", r.begin, r.end);
  fflush(stdout);
}

void test_range()
{
  using Range = plot::PlotRange;

  Range r{2,2};
  print(r);

  r.initializeUpdate();
  r.update(Range{0.25, 0.75});
  print(r);
  r.update(0);
  r.update(1);
  print(r);

  Range v{25, 75};
  Range r2 = r.subset(v, 100);
  print(r2);
}

int main(int argc, char **argv)
{
  test_range();

  std::srand(std::time(0));

  QApplication app(argc, argv);

  plot::PlotWidget *plot = new plot::PlotWidget();
  plot->setTitleX(plot::PlotTheme::titleString(QStringLiteral("Time"),
                                               QStringLiteral("s")));
  plot->show();
  plot->resize(640, 480);

  plot::PlotSeriesHandle h1 = plot->insert(TestSignal::generate(QStringLiteral("V")));
  plot::PlotSeriesHandle h2 = plot->insert(TestSignal::generate(QStringLiteral("V")));

  plot::IPlotSeriesData *sine = TestSignal::sine(QString());
  // test_findLeft(sine);
  // test_findRight(sine);
  plot::PlotSeriesHandle h3 = plot->insert(sine);

  info(h1.data());
  info(h2.data());
  info(h3.data());

  QFont font = plot->font();
  font.setPointSize(10);
  plot->setFont(font);

  // h1.remove();
  // h2.remove();
  // h3.remove();

  // h3.activate();

  const int result = app.exec();
  delete plot;

  return result;
}
