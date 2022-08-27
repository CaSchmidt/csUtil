#pragma once

#include <QtCore/QVector>

#include <Plot/IPlotSeriesData.h>

using SignalData = QVector<double>;

class TestSignal : public plot::IPlotSeriesData {
public:
  TestSignal(const QString& name, const QString& unit,
             const SignalData& x, const SignalData& y);
  ~TestSignal();

  QString name() const;
  QString unit() const;

  qreal valueX(const int i) const;
  qreal valueY(const int i) const;

  void values(QPointF *points, const int L, const int R) const;

  QPointF value(const int i) const;
  int size() const;

  static TestSignal *generate(const QString& unit);
  static TestSignal *sine(const QString& unit);

private:
  static int _count;
  QString _name;
  QString _unit;
  SignalData _x;
  SignalData _y;
};
