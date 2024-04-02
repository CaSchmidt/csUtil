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

  void print(const DB& db, const DB::key_type id)
  {
    cs::println("find(%): %", id, db.find(id).toString());
  }

  void print(const DB& db)
  {
    const auto keys = db.listKeys<std::list>();
    for(const DB::key_type key : keys) {
      print(db, key);
    }
  }

} // namespace util

namespace db {

  using key_type = util::DB::key_type;

  TEST_CASE("Various DB operations (integral key).", "[integral]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    util::DB db;

    {
      util::Item item = util::Item(db.nextKey());
      REQUIRE( item.id() == 1 );

      REQUIRE( !db.add(item) );
      REQUIRE( !db.contains(item.id()) );
    }

    { // create
      cs::println("CRUD: create");

      REQUIRE( db.add({db.nextKey(), "one"}) );

      REQUIRE( db.add({db.nextKey(), "two"}) );

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
        const util::Item old = db.find(key);
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

} // namespace db
