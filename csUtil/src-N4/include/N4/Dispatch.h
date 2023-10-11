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

#ifndef N4_DISPATCH_H
#define N4_DISPATCH_H

#include <N4/ExprBase.h>
#include <N4/SIMD.h>
#include <N4/TypeTraits.h>

namespace n4 {

  namespace impl {

    using simd::simd_t;

    ////// Dispatch 1 Argument ///////////////////////////////////////////////
    ///
    /// Syntax: simd_t OP::eval(simd_t)

    template<typename OP, typename traits_T, typename ARG1>
    class DispatchV : public ExprBase<traits_T,DispatchV<OP,traits_T,ARG1>> {
    public:
      DispatchV(const ARG1& arg1)
        : _arg1(arg1)
      {
      }

      inline simd_t eval() const
      {
        return OP::eval(_arg1.eval());
      }

    private:
      const ARG1& _arg1;
    };

    ////// Dispatch 2 Arguments //////////////////////////////////////////////
    ///
    /// Syntax: simd_t OP::eval(simd_t, simd_t)

    template<typename OP, typename traits_T, typename ARG2>
    class DispatchSV : public ExprBase<traits_T,DispatchSV<OP,traits_T,ARG2>> {
    public:
      DispatchSV(const real_t arg1, const ARG2& arg2)
        : _arg1(arg1)
        , _arg2(arg2)
      {
      }

      inline simd_t eval() const
      {
        return OP::eval(simd::set(_arg1), _arg2.eval());
      }

    private:
      const real_t _arg1;
      const ARG2&  _arg2;
    };

    template<typename OP, typename traits_T, typename ARG1>
    class DispatchVS : public ExprBase<traits_T,DispatchVS<OP,traits_T,ARG1>> {
    public:
      DispatchVS(const ARG1& arg1, const real_t arg2)
        : _arg1(arg1)
        , _arg2(arg2)
      {
      }

      inline simd_t eval() const
      {
        return OP::eval(_arg1.eval(), simd::set(_arg2));
      }

    private:
      const ARG1&  _arg1;
      const real_t _arg2;
    };

    template<typename OP, typename traits_T, typename ARG1, typename ARG2>
    class DispatchVV : public ExprBase<traits_T,DispatchVV<OP,traits_T,ARG1,ARG2>> {
    public:
      DispatchVV(const ARG1& arg1, const ARG2& arg2)
        : _arg1(arg1)
        , _arg2(arg2)
      {
      }

      inline simd_t eval() const
      {
        return OP::eval(_arg1.eval(), _arg2.eval());
      }

    private:
      const ARG1& _arg1;
      const ARG2& _arg2;
    };

    ////// Dispatch 3 Arguments //////////////////////////////////////////////
    ///
    /// Syntax: simd_t OP::eval(simd_t, simd_t, simd_t)

    template<typename OP, typename traits_T, typename ARG1>
    class DispatchVSS : public ExprBase<traits_T,DispatchVSS<OP,traits_T,ARG1>> {
    public:
      DispatchVSS(const ARG1& arg1, const real_t arg2, const real_t arg3)
        : _arg1(arg1)
        , _arg2(arg2)
        , _arg3(arg3)
      {
      }

      inline simd_t eval() const
      {
        return OP::eval(_arg1.eval(), simd::set(_arg2), simd::set(_arg3));
      }

    private:
      const ARG1&  _arg1;
      const real_t _arg2;
      const real_t _arg3;
    };

  } // namespace impl

} // namespace n4

#endif // N4_DISPATCH_H
