#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <QtWidgets/QApplication>

#include <cs/Core/Constants.h>

#include <Plot/PlotTheme.h>
#include <Plot/PlotWidget.h>
#include <Plot/Data/ConstantIntervalData.h>
#include <Plot/Data/VectorData.h>

namespace test_signal {

  using vector_type = std::vector<double>;
  using   size_type = vector_type::size_type;
  using  value_type = vector_type::value_type;

  using ConstInterData = plot::ConstantIntervalData<value_type>;
  using     VectorData = plot::VectorData<value_type>;

  size_type count = 0;

  plot::PlotSeriesDataPtr generate(const QString& unit)
  {
    const value_type         t0 = 0.0;
    const value_type          T = 5.0;
    const value_type         dt = 0.002;
    const  size_type numSamples = size_type(T/dt) + 1;

    const value_type    amp = (value_type)(std::rand()%10 + 1); // rand()%10 == [0,9]
    const value_type   freq = (value_type)(std::rand()%3 + 1);  // rand()%3  == [0,2]
    const value_type tshift = (value_type)(std::rand()%11)*0.1; // rand()%11 == [0,10]

    const value_type omega = 2.0*cs::real_konst<value_type>::pi*freq;

    vector_type x(numSamples), y(numSamples);
    for(size_type i = 0; i < numSamples; i++) {
      const value_type   t = t0 + value_type(i)*dt;
      const value_type phi = omega*t;

      x[i] = t + tshift;
      y[i] = amp*std::sin(phi);
    }

    return VectorData::make(QStringLiteral("Signal ") + QString::number(++count),
                            unit, x, y);
  }

  plot::PlotSeriesDataPtr sine(const QString& unit)
  {
    const value_type       freq = 1.0;
    const value_type         t0 = 0.0;
    const value_type         dt = 0.01;
    const  size_type numSamples = 101;

    const value_type omega = 2.0*cs::real_konst<value_type>::pi*freq;

    vector_type x(numSamples), y(numSamples);
    for(size_type i = 0; i < numSamples; i++) {
      const value_type   t = t0 + value_type(i)*dt;
      const value_type phi = omega*t;

      x[i] = t;
      y[i] = std::sin(phi);
    }

#if 1
    return ConstInterData::make(QStringLiteral("Signal ") + QString::number(++count),
                                unit, t0, dt, y);
#else
    return VectorData::make(QStringLiteral("Signal ") + QString::number(++count),
                            unit, x, y);
#endif
  }

} // namespace test_signal

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
  using namespace plot;

  test_range();

  std::srand(std::time(0));

  QApplication app(argc, argv);

  PlotWidget *plot = new PlotWidget();
  plot->setTitleX(PlotTheme::titleString(QStringLiteral("Time"),
                                         QStringLiteral("s")));
  plot->show();
  plot->resize(640, 480);

  PlotSeriesDataPtr sig1 = test_signal::generate(QStringLiteral("V"));
  PlotSeriesHandle h1 = plot->insert(sig1.get());
  PlotSeriesDataPtr sig2 = test_signal::generate(QStringLiteral("V"));
  PlotSeriesHandle h2 = plot->insert(sig2.get());
  PlotSeriesDataPtr sig3 = test_signal::sine(QString());
  PlotSeriesHandle h3 = plot->insert(sig3.get());

  info(h1.data());
  info(h2.data());
  info(h3.data());

  QFont font = plot->font();
  font.setPointSize(10);
  plot->setFont(font);

  /*
  {
    PlotTheme t = plot->theme();
    t.setAxisLabelFormat(PlotTheme::XAxis, {'e', 1});
    t.setAxisLabelFormat(PlotTheme::YAxis, {'e', 1});
    plot->setTheme(t);
  }
  */

  // plot->setDrawFlag(Marks);
  // plot->setDrawFlag(Steps);

  // h1.remove();
  // h2.remove();
  // h3.remove();

  // h3.activate();

  const int result = app.exec();
  delete plot;

  return result;
}
