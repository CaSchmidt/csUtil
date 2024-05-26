/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
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

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "cs/Qt/FileIO.h"

namespace cs {

  namespace impl_fileio {

    QString readEncodedText(const QString& filename, const char *codec)
    {
      QFile file(filename);
      if( !file.open(QIODevice::ReadOnly) ) {
        return QString();
      }

      QTextStream stream(&file);
      stream.setCodec(codec);
      const QString text = stream.readAll();

      file.close();

      return text;
    }

    bool writeEncodedText(const QString& filename, const QString& text,
                          const char *codec)
    {
      QFile file(filename);
      if( !file.open(QIODevice::WriteOnly) ) {
        return false;
      }

      QTextStream stream(&file);
      stream.setCodec(codec);
      stream << text;
      stream.flush();

      file.close();

      return true;
    }

  } // namespace impl_fileio

  QString CS_UTIL_EXPORT readUtf8Text(const QString& filename)
  {
    return impl_fileio::readEncodedText(filename, "UTF-8");
  }

  bool CS_UTIL_EXPORT writeUtf8Text(const QString& filename, const QString& text)
  {
    return impl_fileio::writeEncodedText(filename, text, "UTF-8");
  }

} // namespace cs
