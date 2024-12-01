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

#pragma once

#include <algorithm>
#include <span>
#include <string_view>

#include <cs/Core/Buffer.h>

namespace cs {

  using BufferView = std::span<const byte_t,std::dynamic_extent>;

  ////// View Operations /////////////////////////////////////////////////////

  constexpr BufferView subview(const BufferView& view,
                               const std::size_t offset,
                               const std::size_t count = std::dynamic_extent)
  {
    return offset < view.size()
        ? view.subspan(offset, std::min(count, view.size() - offset))
        : BufferView();
  }

  ////// Conversion Helpers //////////////////////////////////////////////////

  constexpr Buffer toBuffer(const BufferView& view)
  {
    return !view.empty()
        ? Buffer(view.begin(), view.end())
        : Buffer();
  }

  inline BufferView toBufferView(const void *data, const std::size_t size)
  {
    return BufferView(reinterpret_cast<const byte_t*>(data), size);
  }

  constexpr std::string_view toStringView(const BufferView& view)
  {
    return !view.empty()
        ? std::string_view(reinterpret_cast<const char*>(view.data()), view.size())
        : std::string_view();
  }

} // namespace cs
