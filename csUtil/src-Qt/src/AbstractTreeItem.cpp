/****************************************************************************
** Copyright (c) 2014, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <QtCore/QModelIndex>

#include "cs/Qt/AbstractTreeItem.h"

namespace cs {

  ////// Public //////////////////////////////////////////////////////////////

  CS_UTIL_EXPORT AbstractTreeItem *csTreeItem(const QModelIndex& index)
  {
    return static_cast<AbstractTreeItem*>(index.internalPointer());
  }

  ////// public //////////////////////////////////////////////////////////////

  AbstractTreeItem::AbstractTreeItem(AbstractTreeItem *parent)
    : _children()
    , _parent(parent)
  {
  }

  AbstractTreeItem::~AbstractTreeItem()
  {
    qDeleteAll(_children);
  }

  void AbstractTreeItem::appendChild(AbstractTreeItem *child)
  {
    if( child != nullptr ) {
      child->_parent = this;
      _children.append(child);
    }
  }

  void AbstractTreeItem::insertChild(int row, AbstractTreeItem *child)
  {
    if( child != nullptr ) {
      child->_parent = this;
      _children.insert(row, child);
    }
  }

  AbstractTreeItem *AbstractTreeItem::takeChild(int row)
  {
    if( row < 0  ||  row >= _children.size() ) {
      return nullptr;
    }
    AbstractTreeItem *child = _children.takeAt(row);
    child->_parent = nullptr;
    return child;
  }

  void AbstractTreeItem::removeChild(int row)
  {
    if( row >= 0  &&  row < _children.size() ) {
      delete _children.takeAt(row);
    }
  }

  AbstractTreeItem *AbstractTreeItem::childItem(int row) const
  {
    return _children.value(row, nullptr);
  }

  AbstractTreeItem *AbstractTreeItem::parentItem() const
  {
    return _parent;
  }

  int AbstractTreeItem::row() const
  {
    if( _parent != nullptr ) {
      return _parent->_children.indexOf(const_cast<AbstractTreeItem*>(this));
    }
    return 0;
  }

  int AbstractTreeItem::rowCount() const
  {
    return _children.size();
  }

} // namespace cs
