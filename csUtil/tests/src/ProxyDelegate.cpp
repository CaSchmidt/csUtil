#include <QtWidgets/QAbstractItemView>

#include <cs/Qt/ItemModel.h>
#include <QtCreator/HighlightingItemDelegate.h>

#include "ProxyDelegate.h"

////// Constants /////////////////////////////////////////////////////////////

constexpr int kIndexDepth = 1;
constexpr int kTabWidth = 8;

////// public ////////////////////////////////////////////////////////////////

ProxyDelegate::ProxyDelegate(QAbstractItemView *view)
  : QAbstractItemDelegate(view)
{
  _delegate  = view->itemDelegate();
  _highlight = new QtCreator::HighlightingItemDelegate(kTabWidth, view);

  view->setItemDelegate(this);
}

ProxyDelegate::~ProxyDelegate()
{
}

void ProxyDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  if( cs::indexDepth(index) == kIndexDepth ) {
    _highlight->paint(painter, option, index);
  } else {
    _delegate->paint(painter, option, index);
  }
}

QSize ProxyDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  if( cs::indexDepth(index) == kIndexDepth ) {
    return _highlight->sizeHint(option, index);
  }
  return _delegate->sizeHint(option, index);
}

void ProxyDelegate::setTabWidth(const int width)
{
  _highlight->setTabWidth(width);
}
