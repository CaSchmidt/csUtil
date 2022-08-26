#pragma once

#include <QtWidgets/QAbstractItemDelegate>

namespace QtCreator {
  class HighlightingItemDelegate;
}
class QAbstractItemView;

class ProxyDelegate : public QAbstractItemDelegate {
  Q_OBJECT
public:
  ProxyDelegate(QAbstractItemView *view);
  ~ProxyDelegate();

  void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

  void setTabWidth(const int width);

private:
  QAbstractItemDelegate *_delegate;
  QtCreator::HighlightingItemDelegate *_highlight;
};
