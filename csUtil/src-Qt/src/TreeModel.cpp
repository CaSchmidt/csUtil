/****************************************************************************
** Copyright (c) 2016, Carsten Schmidt. All rights reserved.
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

#include "cs/Qt/TreeModel.h"

#include "cs/Qt/AbstractTreeItem.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  TreeModel::TreeModel(AbstractTreeItem *rootItem, QObject *parent)
    : QAbstractItemModel(parent)
    , _root(rootItem)
  {
  }

  TreeModel::~TreeModel()
  {
  }

  AbstractTreeItem *TreeModel::root() const
  {
    return _root.get();
  }

  void TreeModel::setRoot(AbstractTreeItem *rootItem)
  {
    if( rootItem == nullptr ) {
      return;
    }
    beginResetModel();
    _root.reset(rootItem);
    endResetModel();
  }

  int TreeModel::columnCount(const QModelIndex& parent) const
  {
    if( parent.isValid() ) {
      return treeItem(parent)->columnCount();
    }
    return _root->columnCount();
  }

  QVariant TreeModel::data(const QModelIndex& index, int role) const
  {
    if( !index.isValid() ) {
      return QVariant();
    }
    return treeItem(index)->data(index.column(), role);
  }

  Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
  {
    if( !index.isValid() ) {
      return Qt::NoItemFlags;
    }
    return QAbstractItemModel::flags(index);
  }

  QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const
  {
    if( orientation == Qt::Horizontal ) {
      return _root->data(section, role);
    }
    return QVariant();
  }

  QModelIndex TreeModel::index(int row, int column,
                               const QModelIndex& parent) const
  {
    if( !hasIndex(row, column, parent) ) {
      return QModelIndex();
    }

    AbstractTreeItem *parentItem = parent.isValid()
        ? treeItem(parent)
        : _root.get();

    AbstractTreeItem *childItem = parentItem->childItem(row);
    if( childItem != nullptr ) {
      return createIndex(row, column, childItem);
    }

    return QModelIndex();
  }

  QModelIndex TreeModel::parent(const QModelIndex& child) const
  {
    if( !child.isValid() ) {
      return QModelIndex();
    }

    AbstractTreeItem  *childItem = treeItem(child);
    AbstractTreeItem *parentItem = childItem->parentItem();

    if( parentItem == _root.get() ) {
      return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
  }

  int TreeModel::rowCount(const QModelIndex& parent) const
  {
    if( parent.column() > 0 ) {
      return 0;
    }

    AbstractTreeItem *parentItem = parent.isValid()
        ? treeItem(parent)
        : _root.get();

    return parentItem->rowCount();
  }

} // namespace cs
