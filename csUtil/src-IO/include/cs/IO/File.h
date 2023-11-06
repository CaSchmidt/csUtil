/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
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

#include <filesystem>
#include <memory>

#include <cs/Core/Buffer.h>
#include <cs/Core/Flags.h>
#include <cs/IO/AbstractIODevice.h>

namespace cs {

  enum class FileOpenFlag : unsigned int {
    None      = 0,
    Read      = 0x01,
    Write     = 0x02,
    ReadWrite = Read | Write,
    Append    = 0x10,
    Truncate  = 0x11
  };

} // namespace cs

CS_ENABLE_FLAGS(cs::FileOpenFlag);

namespace cs {

  using FileImplPtr = std::unique_ptr<class FileImpl>;

  class CS_UTIL_EXPORT File : public AbstractIODevice {
  public:
    using offset_type = uint64_t;

    using OpenFlags = Flags<FileOpenFlag>;

    File() noexcept;
    ~File() noexcept;

    void close();
    bool isOpen() const;
    bool open(const std::filesystem::path& path, const OpenFlags flags = FileOpenFlag::Read);

    std::filesystem::path path() const;

    bool seek(const offset_type offset) const;
    offset_type tell() const;

    offset_type size() const;

    std::size_t read(void *data, const std::size_t sizData) const;
    std::size_t write(const void *data, const std::size_t sizData) const;

    Buffer readAll() const;

  private:
    FileImplPtr _impl{nullptr};
  };

} // namespace cs
