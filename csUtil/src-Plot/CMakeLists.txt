list(APPEND Plot_HEADERS
  include/internal/AxisLabel.h
  include/internal/Draw.h
  include/internal/DrawHelper.h
  include/internal/IAxisElement.h
  include/internal/IPlotElement.h
  include/internal/IPlotImplementation.h
  include/internal/ISeriesControl.h
  include/internal/ITitleElement.h
  include/internal/IViewportControl.h
  include/internal/IXAxisControl.h
  include/internal/Layout.h
  include/internal/Mapping.h
  include/internal/PanAndZoom.h
  include/internal/Scale.h
  include/internal/Scope.h
  include/internal/ScopeRow.h
  include/internal/Series.h
  include/internal/SeriesStore.h
  include/internal/SinglePlotImpl.h
  include/internal/XAxis.h
  include/internal/XTitle.h
  include/internal/YAxis.h
  include/internal/YTitle.h
  include/Plot/Data/ConstantIntervalData.h
  include/Plot/Data/VectorData.h
  include/Plot/IPlotSeriesData.h
  include/Plot/Plot.h
  include/Plot/PlotAxisLabelFormat.h
  include/Plot/PlotRange.h
  include/Plot/PlotSeriesHandle.h
  include/Plot/PlotTheme.h
  include/Plot/PlotWidget.h
  )

list(APPEND Plot_SOURCES
  src/AxisLabel.cpp
  src/Draw.cpp
  src/DrawHelper.cpp
  src/IAxisElement.cpp
  src/IPlotElement.cpp
  src/IPlotImplementation.cpp
  src/ISeriesControl.cpp
  src/IPlotSeriesData.cpp
  src/ITitleElement.cpp
  src/IViewportControl.cpp
  src/IXAxisControl.cpp
  src/main_plot.cpp
  src/Mapping.cpp
  src/PanAndZoom.cpp
  src/PlotSeriesHandle.cpp
  src/PlotTheme.cpp
  src/PlotWidget.cpp
  src/Scale.cpp
  src/Scope.cpp
  src/ScopeRow.cpp
  src/Series.cpp
  src/SeriesStore.cpp
  src/SinglePlotImpl.cpp
  src/XAxis.cpp
  src/XTitle.cpp
  src/YAxis.cpp
  src/YTitle.cpp
  )

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${Plot_HEADERS}
  PRIVATE ${Plot_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
