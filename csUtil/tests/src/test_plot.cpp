#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <filesystem>

#include <QtWidgets/QApplication>

#include <cs/Core/Constants.h>
#include <cs/Core/Container.h>
#include <cs/Core/QStringUtil.h>
#include <cs/IO/BufferedReader.h>
#include <cs/IO/File.h>
#include <cs/System/FileSystem.h>
#include <cs/Text/PrintUtil.h>
#include <cs/Text/StringUtil.h>
#include <cs/Text/StringValue.h>

#include <Plot/PlotTheme.h>
#include <Plot/PlotWidget.h>
#include <Plot/Data/ConstantIntervalData.h>
#include <Plot/Data/VectorData.h>

namespace test_csv {

  struct Column {
    Column(const std::string& name = std::string())
      : name(name)
    {
      constexpr std::size_t INIT_SIZE = 10000;

      values.reserve(INIT_SIZE);
    }

    bool isEmpty() const
    {
      return name.empty()  ||  values.empty();
    }

    template<typename Func>
    inline void apply(Func func)
    {
      std::for_each(values.begin(), values.end(), std::forward<Func>(func));
    }

    std::string name;
    std::string unit;
    std::vector<double> values;
  };

  using Columns = std::list<Column>;

  using Series = std::list<plot::PlotSeriesDataPtr>;

  using StringList = std::list<std::string>;

  bool contains(const StringList& list, const std::string& value)
  {
    return std::find(list.cbegin(), list.cend(), value) != list.cend();
  }

  Column makeTime(Column column)
  {
    column.name = "Time";
    column.unit = "s";
    column.apply([](double& value) -> void {
      value /= 1000;
    });

    return column;
  }

  void print(const Column& column)
  {
    cs::println("\"%\" [%] = { %, %, % }", column.name, column.unit,
                column.values[0], column.values[1], column.values[2]);
  }

  void print(const Columns& columns)
  {
    std::for_each(columns.cbegin(), columns.cend(),
                  [](const Column& column) -> void {
      print(column); // NOTE: Use lambda to avoid error of function overload!
    });
  }

  Series load(const std::filesystem::path& filename)
  {
    // Lines are 1-based!
    constexpr std::size_t LINE_NAME  = 2;
    constexpr std::size_t LINE_UNIT  = 3;
    constexpr std::size_t LINE_VALUE = 4;

    constexpr char SEP = ';';

    if( !cs::isFile(filename) ) {
      cs::printerrln("ERROR: File \"%\" not found!", filename);
      return Series();
    }

    cs::File file;
    if( !file.open(filename) ) {
      cs::printerrln("ERROR: Unable to open file \"%\"!", filename);
      return Series();
    }

    Columns columns;
    cs::BufferedReader reader;
    for(std::size_t lineno = 1; !file.atEnd(); lineno++) {
      const std::string  line = reader.getLine(file);
      const StringList fields = cs::split(line, SEP, cs::SplitFlag::Trim);

      if( !columns.empty()  &&  fields.size() != columns.size() ) {
        cs::printerrln("ERROR: Ignoring line % due to invalid number of columns!", lineno);
        continue;
      }

      if( lineno == LINE_NAME ) {
        for(const std::string& field : fields) {
          columns.push_back(Column(field));
        }

      } else if( lineno == LINE_UNIT ) {
        auto colIter = columns.begin();
        for(const std::string& field : fields) {
          colIter->unit = field;
          ++colIter;
        }

      } else if( lineno >= LINE_VALUE ) {
        auto colIter = columns.begin();
        for(const std::string& field : fields) {
          colIter->values.push_back(cs::toValue<double>(field));
          ++colIter;
        }

      }
    } // while( !file.atEnd() )

    // Debug
    print(columns);

    const Column time = makeTime(cs::takeIndex(columns, 0));

    const StringList want{"Motordrehzahl", "Fahrzeuggeschwindigkeit", "Zündwinkel Zylinder 1"};

    Series result;
    for(const Column& column : columns) {
      if( !contains(want, column.name) ) {
        continue;
      }

      const QString name = cs::toQString(cs::toUtf8StringView(column.name));
      const QString unit = cs::toQString(cs::toUtf8StringView(column.unit));

      result.push_back(plot::VectorData<double>::make(name, unit, time.values, column.values));
    }

    return result;
  }

} // namespace test_csv

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

    const value_type omega = 2.0*cs::RealKonst<value_type>::pi*freq;

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

    const value_type omega = 2.0*cs::RealKonst<value_type>::pi*freq;

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

#if 1
  auto series = test_csv::load(cs::reparent(argv[0], "Recording_002.csv"));
  for(const auto& s : series) {
    plot->insert(s.get());
  }
#else
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
#endif

  const int result = app.exec();
  delete plot;

  return result;
}
