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

#pragma once

#include <QtCore/QList>
#include <QtCore/QVariant>

#include <cs/Core/csutil_config.h>

namespace cs {

  class CS_UTIL_EXPORT AbstractTreeItem {
  public:
    AbstractTreeItem(AbstractTreeItem *parent = nullptr);
    virtual ~AbstractTreeItem();

    void appendChild(AbstractTreeItem *child);
    void insertChild(int row, AbstractTreeItem *child);
    AbstractTreeItem *takeChild(int row);
    void removeChild(int row);

    AbstractTreeItem *childItem(int row) const;
    AbstractTreeItem *parentItem() const;
    int row() const;

    int rowCount() const;

    virtual int columnCount() const = 0;
    virtual QVariant data(int column, int role = Qt::DisplayRole) const = 0;

  private:
    Q_DISABLE_COPY(AbstractTreeItem)

    QList<AbstractTreeItem*> _children;
    AbstractTreeItem *_parent;
  };

  CS_UTIL_EXPORT AbstractTreeItem *treeItem(const QModelIndex& index);

} // namespace cs
