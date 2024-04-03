#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <list>
#include <unordered_map>
#include <vector>

#include <catch.hpp>

#include <cs/Core/DbStore.h>
#include <cs/Text/MixIn.h>
#include <cs/Text/PrintUtil.h>
#include <cs/Text/StringUtil.h>

namespace util {

  template<typename DB_t>
  void print(const DB_t& db,
             const typename DB_t::key_type& id)
  {
    cs::println("find(%): %", id, db.find(id).toString());
  }

  template<typename DB_t>
  void print(const DB_t& db)
  {
    const auto keys = db.template listKeys<std::list>();
    for(const typename DB_t::key_type& key : keys) {
      print(db, key);
    }
  }

} // namespace util

namespace db_integral {

  struct Item
      : cs::DbItem<Item,cs::DbIntegralKeyTraits<>>
      , cs::ToStringMixIn<Item> {
  public:
    using DbItem::key_type;
    using DbItem::traits_type;

    Item(const key_type id = traits_type::makeInvalid(),
         const std::string& name = std::string()) noexcept
      : name(name)
      , _id{id}
    {
    }

    auto elements() const
    {
      return std::forward_as_tuple(_id, name);
    }

    std::string name;

  private:
    friend DbItem;

    inline key_type impl_id() const
    {
      return _id;
    }

    inline bool impl_isValid() const
    {
      return !name.empty();
    }

    key_type _id{traits_type::makeInvalid()};
  };

  using DB = cs::DbStore<Item,std::unordered_map>;

  using key_type = DB::key_type;

  TEST_CASE("Various DB operations (integral key).", "[integral]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    DB db;

    {
      Item item = Item(db.nextKey());
      REQUIRE( item.id() == 1 );

      REQUIRE( !db.add(item) );
      REQUIRE( !db.contains(item.id()) );
    }

    { // create
      cs::println("CRUD: create");

      REQUIRE( db.nextKey() == 1 );
      REQUIRE( db.add({db.nextKey(), "one"}) );

      REQUIRE( db.nextKey() == 2 );
      REQUIRE( db.add({db.nextKey(), "two"}) );

      REQUIRE( db.nextKey() == 3 );
      REQUIRE( db.add({db.nextKey(), "three"}) );

      REQUIRE( db.size() == 3 );
    }

    { // read
      cs::println("CRUD: read");

      constexpr key_type INVALID = 42;
      REQUIRE( db.find(INVALID).id() == 0 );
      util::print(db, INVALID);

      const auto keys = db.listKeys<std::list>();
      for(const key_type key : keys) {
        REQUIRE( db.find(key).id() == key );
        util::print(db, key);
      }
    }

    { // update
      cs::println("CRUD: update");

      const auto keys = db.listKeys<std::vector>();
      for(const key_type key : keys) {
        const Item old = db.find(key);
        REQUIRE( !db.set({old.id()}) ); // Invalid; omits 'name'!
        REQUIRE( db.set({old.id(), cs::toUpper(old.name)}) );
        REQUIRE( db.find(key).name == cs::toUpper(old.name) );
        util::print(db, key);
      }
    }

    { // destroy
      cs::println("CRUD: destroy");

      constexpr key_type ID = 2;
      REQUIRE( db.remove(ID) );
      REQUIRE( !db.contains(ID) );
      REQUIRE( db.size() == 2 );

      util::print(db);
    }
  }

} // namespace db_integral
