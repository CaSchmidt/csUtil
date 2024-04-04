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

  ////// Item ////////////////////////////////////////////////////////////////

  template<typename DerivedT, typename KeyTraitsT>
  struct AbstractItem : cs::CRTPbase<AbstractItem<DerivedT,KeyTraitsT>> {
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

  template<typename KeyTraitsT, typename ItemT>
  struct is_item : std::bool_constant<
      std::is_same_v<typename KeyTraitsT::value_type,decltype(std::declval<ItemT>().id())>
  > { };

  template<typename KeyTraitsT, typename ItemT>
  inline constexpr bool is_item_v = is_item<KeyTraitsT,ItemT>::value;

  ////// Store ///////////////////////////////////////////////////////////////

  template<typename ItemT,
           template<typename...> typename StoreT>
  requires is_item_v<typename ItemT::traits_type,ItemT>
  class ItemStore {
  public:
    using   item_type = ItemT;
    using traits_type = typename item_type::traits_type;
    using    key_type = typename traits_type::value_type;

    using store_type = StoreT<key_type,item_type>;
    using  size_type = typename store_type::size_type;
    using value_type = typename store_type::value_type;

    using const_iterator = typename store_type::const_iterator;
    using       iterator = typename store_type::iterator;

    using DirtyHandler = std::function<void(void)>;

    ItemStore() noexcept
      : _dummy(traits_type::makeInvalid())
    {
    }

    ~ItemStore() noexcept
    {
    }

    ItemStore(const ItemStore&) noexcept = default;
    ItemStore& operator=(const ItemStore&) noexcept = default;

    ItemStore(ItemStore&&) noexcept = default;
    ItemStore& operator=(ItemStore&&) noexcept = default;

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

    template<template<typename...> typename C>
    C<item_type> list() const
    {
      if( isEmpty() ) {
        return C<item_type>();
      }

      C<item_type> result;
      reserve(result, size());

      for(const value_type& v : _store) {
        result.push_back(v.second);
      }

      return result;
    }

    template<template<typename...> typename C>
    C<key_type> listKeys() const
    {
      if( isEmpty() ) {
        return C<key_type>();
      }

      C<key_type> result;
      reserve(result, size());

      for(const value_type& v : _store) {
        result.push_back(v.second.id());
      }

      sort(result);

      return result;
    }

    key_type nextKey() const
    {
      const const_iterator hit = std::max_element(_store.cbegin(),
                                                  _store.cend(),
                                                  value_less);

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
    static constexpr auto value_less = [](const value_type& a, const value_type& b) -> bool {
      return a.second.id() < b.second.id();
    };

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
