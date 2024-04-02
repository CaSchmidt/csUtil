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

#include <functional>

#include <cs/Core/Container.h>
#include <cs/Core/CRTPbase.h>

namespace cs {

  ////// Key Traits //////////////////////////////////////////////////////////

  template<typename T = unsigned>
  requires std::is_integral_v<T>
  struct DbIntegralKeyTraits {
    using value_type = T;

    static constexpr bool isValid(const value_type id)
    {
      return id > 0;
    }

    static constexpr value_type makeInvalid()
    {
      return 0;
    }

    static constexpr value_type makeNext(const value_type id)
    {
      constexpr value_type ONE = 1;

      return id + ONE;
    }
  };

  ////// Item ////////////////////////////////////////////////////////////////

  template<typename DerivedT, typename KeyTraitsT>
  struct DbItem : cs::CRTPbase<DbItem<DerivedT,KeyTraitsT>> {
    using traits_type = KeyTraitsT;
    using    key_type = typename traits_type::value_type;

    inline key_type id() const
    {
      return this->as_derived().impl_id();
    }

    inline bool isValid() const
    {
      return traits_type::isValid(id())  &&  this->as_derived().impl_isValid();
    }

    inline explicit operator bool() const
    {
      return isValid();
    }
  };

  ////// Store ///////////////////////////////////////////////////////////////

  template<typename ItemT,
           template<typename...> typename StoreT>
  class DbStore {
  public:
    using   item_type = ItemT;
    using traits_type = typename item_type::traits_type;
    using    key_type = typename item_type::key_type;

    using store_type = StoreT<key_type,item_type>;
    using  size_type = typename store_type::size_type;
    using value_type = typename store_type::value_type;

    using const_iterator = typename store_type::const_iterator;
    using       iterator = typename store_type::iterator;

    using DirtyHandler = std::function<void(void)>;

    DbStore() noexcept
      : _dummy(traits_type::makeInvalid())
    {
    }

    ~DbStore() noexcept
    {
    }

    DbStore(const DbStore&) noexcept = default;
    DbStore& operator=(const DbStore&) noexcept = default;

    DbStore(DbStore&&) noexcept = default;
    DbStore& operator=(DbStore&&) noexcept = default;

    void setDirtyHandler(DirtyHandler handler)
    {
      _setDirty = handler;
    }

    // Generic DB Interface //////////////////////////////////////////////////

    void clear()
    {
      _store.clear();
    }

    bool contains(const key_type& id) const
    {
      return _store.count(id) > 0;
    }

    bool isEmpty() const
    {
      return _store.empty();
    }

    bool isValid(const bool check_data = true) const
    {
      if( traits_type::isValid(_dummy.id()) ) {
        return false;
      }

      for(const value_type& item : _store) {
        if( !traits_type::isValid(item.first) ) {
          return false;
        }

        if( item.first != item.second.id() ) {
          return false;
        }

        if( check_data  &&  !item.second.isValid() ) {
          return false;
        }
      } // For Each Item

      return true;
    }

    key_type nextKey() const
    {
      constexpr auto lambda_less = [](const value_type& a, const value_type& b) -> bool {
        return a.second.id() < b.second.id();
      };

      const const_iterator hit = std::max_element(_store.cbegin(),
                                                  _store.cend(),
                                                  lambda_less);

      return hit != _store.cend()
          ? traits_type::makeNext(hit->second.id())
          : traits_type::makeNext(traits_type::makeInvalid());
    }

    size_type size() const
    {
      return _store.size();
    }

    // CRUD Interface ////////////////////////////////////////////////////////

    // CRUD: create
    bool add(item_type item)
    {
      if( !item  ||  contains(item.id()) ) {
        return false;
      }

      _store[item.id()] = std::move(item);

      setModified();

      return true;
    }

#if 0
    item_type& find(const key_type& id)
    {
      const iterator hit = _store.find(id);

      if( hit != _store.end() ) {
        setModified();
      }

      return hit != _store.end()
          ? hit->second
          : _dummy;
    }
#endif

    // CRUD: read
    const item_type& find(const key_type& id) const
    {
      const const_iterator hit = _store.find(id);

      return hit != _store.cend()
          ? hit->second
          : _dummy;
    }

    // CRUD: delete
    bool remove(const key_type& id)
    {
      const size_type count = _store.erase(id);

      setModified();

      return count > 0;
    }

    // CRUD: update
    bool set(item_type item)
    {
      if( !item  ||  !contains(item.id()) ) {
        return false;
      }

      _store.at(item.id()) = std::move(item);

      setModified();

      return true;
    }

  private:
    void setModified()
    {
      if( _setDirty ) {
        _setDirty();
      }
    }

    item_type    _dummy;
    DirtyHandler _setDirty;
    store_type   _store;
  };

} // namespace cs
