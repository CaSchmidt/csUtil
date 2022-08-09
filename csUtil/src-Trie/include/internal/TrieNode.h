/****************************************************************************
** Copyright (c) 2018, Carsten Schmidt. All rights reserved.
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

#ifndef CS_TRIENODE_H
#define CS_TRIENODE_H

#include <cstdint>

#include <list>
#include <memory>
#include <string>
#include <type_traits>

#include "cs/Trie/TrieMatch.h"
#include "internal/FlatTrieUtil.h"

namespace cs {

  class TrieNode {
  private:
    enum StatusBits : uint32_t {
      Mask_Children = 0x00007FFF,
      Mask_Letter   = 0xFFFF0000,
      Mask_Match    = 0x00008000,
      Shift_Letter  = 16
    };

    using Children = std::unique_ptr<TrieNode[]>;

    Children _children;
    uint32_t _status;

  public:
    using letter_type = uint16_t;

    template<typename T>
    using if_letter_t = std::enable_if_t<sizeof(T) <= sizeof(letter_type),letter_type>;

    TrieNode(const letter_type letter = 0)
      : _children()
      , _status()
    {
      _status   = letter;
      _status <<= Shift_Letter;
    }

    ~TrieNode() = default;

    TrieNode(TrieNode&&) = default;
    TrieNode& operator=(TrieNode&&) = default;

    template<typename T>
    const TrieNode *base(const std::size_t idxStr, const std::basic_string<T>& str,
                         if_letter_t<T> * = nullptr) const
    {
      if( idxStr == str.size() ) {
        return this;
      }

      TrieNode *child = findLetter(str[idxStr]);
      if( child == nullptr ) {
        return nullptr;
      }

      return child->base(idxStr + 1, str);
    }

    template<typename T>
    TrieMatch find(const std::size_t idxStr, const std::basic_string<T>& str,
                       if_letter_t<T> * = nullptr) const
    {
      if( idxStr == str.size() ) {
        if( isMatch() ) {
          return TrieMatch::Exact;
        } else {
          return TrieMatch::Partial;
        }
      }

      TrieNode *idxMatch = findLetter(str[idxStr]);

      if( idxMatch == nullptr ) {
        return TrieMatch::None;
      }

      return idxMatch->find(idxStr + 1, str);
    }

    void flattened(FlatTrie::Links& links, FlatTrie::Letters& letters,
                   std::size_t& cntNodes) const
    {
      if( childCount() < 1 ) {
        return;
      }

      const std::size_t baseIndex = cntNodes;

      cntNodes += childCount();

      for(std::size_t i = 0; i < childCount(); i++) {
        letters[baseIndex + i] = _children[i].letter();

        if( _children[i].childCount() > 0 ) {
          links[baseIndex + i] = static_cast<FlatTrie::link_type>(cntNodes);
        } else {
          links[baseIndex + i] = 0;
        }

        if( _children[i].isMatch() ) {
          links[baseIndex + i] |= impl_trie::Mask_EOW;
        }

        _children[i].flattened(links, letters, cntNodes);
      }

      links[baseIndex + childCount() - 1] |= impl_trie::Mask_EOL;
    }

    template<typename T>
    void insert(const std::size_t idxStr, const std::basic_string<T>& str,
                if_letter_t<T> * = nullptr)
    {
      if( idxStr == str.size() ) {
        _status |= Mask_Match;
        return;
      }

      TrieNode *idxMatch = insertLetter(str[idxStr]);

      idxMatch->insert(idxStr + 1, str);
    }

    template<typename T>
    void insertReverse(const std::size_t idxStr, const std::basic_string<T>& str,
                       if_letter_t<T> * = nullptr)
    {
      if( idxStr == 0 ) {
        _status |= Mask_Match;
        return;
      }

      TrieNode *idxMatch = insertLetter(str[idxStr - 1]);

      idxMatch->insertReverse(idxStr - 1, str);
    }

    template<typename T>
    void list(std::list<std::basic_string<T>>& words, std::basic_string<T>& str,
              if_letter_t<T> * = nullptr) const
    {
      if( isMatch() ) {
        words.push_back(str);
      }

      for(std::size_t i = 0; i < childCount(); i++) {
        const TrieNode& child = _children[i];
        str.push_back(static_cast<T>(child.letter()));
        child.list(words, str);
        str.pop_back();
      }
    }

    void statistics(std::size_t& numNodes) const
    {
      numNodes++;

      for(std::size_t i = 0; i < childCount(); i++) {
        _children[i].statistics(numNodes);
      }
    }

  private:
    TrieNode(const TrieNode&) = delete;
    TrieNode& operator=(const TrieNode&) = delete;

    constexpr std::size_t childCount() const
    {
      return _status & Mask_Children;
    }

    constexpr letter_type letter() const
    {
      return _status >> Shift_Letter;
    }

    constexpr bool isMatch() const
    {
      return (_status & Mask_Match) != 0;
    }

    TrieNode *findLetter(const letter_type letter) const
    {
      TrieNode *result = nullptr;
      for(std::size_t i = 0; i < childCount(); i++) {
        if( _children[i].letter() == letter ) {
          result = &_children[i];
          break;
        }
      }
      return result;
    }

    TrieNode *insertLetter(const letter_type letter)
    {
      TrieNode *result = findLetter(letter);

      if( result == nullptr ) {
        std::size_t idxInsert = 0;

        // (1) Increment Child Count BEFORE Insertion ////////////////////////

        _status++; // Increases childCount()!!!

        // (2) Insert Child //////////////////////////////////////////////////

        if( !_children ) { // No Children Yet! -> Add First
          _children = std::make_unique<TrieNode[]>(1);

        } else { // Grow Child Population to Insert New Child
          // Compute (Sorted) Insertion Index Into OLD Children
          for(idxInsert = 0; idxInsert < childCount() - 1; idxInsert++) {
            if( _children[idxInsert].letter() > letter ) {
              break;
            }
          }
          // Save Old Pointers
          Children childrenOld(std::move(_children));
          // New Pointers
          _children = std::make_unique<TrieNode[]>(childCount());
          // Copy Old
          for(std::size_t i = 0; i < idxInsert; i++) {
            _children[i] = std::move(childrenOld[i]);
          }
          for(std::size_t i = idxInsert; i < childCount() - 1; i++) {
            _children[i + 1] = std::move(childrenOld[i]);
          }
        }

        // (3) Setup Inserted Child //////////////////////////////////////////

        _children[idxInsert]._status   = letter;
        _children[idxInsert]._status <<= Shift_Letter;

        // (4) Return Result /////////////////////////////////////////////////

        result = &_children[idxInsert];
      }

      return result;
    }
  };

} // namespace cs

#endif // CS_TRIENODE_H
