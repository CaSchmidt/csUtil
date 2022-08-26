/****************************************************************************
** Copyright (c) 2022, Carsten Schmidt. All rights reserved.
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

#include <QtWidgets/QWidget>

#include <cs/Core/csutil_config.h>

class QAbstractItemView;

namespace cs {

  template<typename T>
  inline T *findParentWidget(QWidget *widget)
  {
    if( widget == nullptr ) {
      return nullptr;
    }

    while( (widget = widget->parentWidget()) != nullptr ) {
      if( dynamic_cast<T*>(widget) != nullptr ) {
        return dynamic_cast<T*>(widget);
      }
    }

    return nullptr;
  }

  CS_UTIL_EXPORT QPoint mapToGlobal(const QAbstractItemView *view, const QPoint& p);

  CS_UTIL_EXPORT qreal scale(const QSizeF& from, const QSizeF& to);

  CS_UTIL_EXPORT QRect screenGeometry(const QPoint& globalPos, QWidget *widget);

  CS_UTIL_EXPORT int screenNumber(const QPoint& globalPos, QWidget *widget);

  CS_UTIL_EXPORT void setBackgroundColor(QWidget *w, const QColor& c,
                                         const bool inactive = true);

} // namespace cs
