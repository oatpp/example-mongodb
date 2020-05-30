
#include "Database.hpp"

#include "db/Model.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"

#include <mongocxx/client.hpp>
#include <mongocxx/options/insert.hpp>
#include <mongocxx/exception/operation_exception.hpp>

namespace db {

Database::Database(const mongocxx::uri &uri, const std::string &dbName, const std::string &collectionName)
  : m_pool(std::make_shared<mongocxx::pool>(uri)), m_databaseName(dbName), m_collectionName(collectionName)
{}

oatpp::Object<User> Database::userFromDto(const oatpp::Object<UserDto>& dto) {
  auto user = User::createShared();
  user->_id = dto->username;
  user->username = dto->username;
  user->active = dto->active;
  user->role = dto->role;
  return user;
}

oatpp::Object<UserDto> Database::dtoFromUser(const oatpp::Object<User>& user) {
  auto dto = UserDto::createShared();
  dto->username = user->username;
  dto->active = user->active;
  dto->role = user->role;
  return dto;
}

bsoncxx::document::value Database::createMongoDocument(const oatpp::Void &polymorph) {
  // if you have huge docs, you may want to increase starting BufferOutputStream size.
  // Or you may want to use oatpp::data::stream::ChunkedBuffer instead - for no-copy growth.
  oatpp::data::stream::BufferOutputStream stream;
  m_objectMapper.write(&stream, polymorph);
  bsoncxx::document::view view(stream.getData(), stream.getCurrentPosition());
  return bsoncxx::document::value(view);
}

oatpp::Object<UserDto> Database::createUser(const oatpp::Object<UserDto> &userDto) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];
  collection.insert_one(createMongoDocument(userFromDto(userDto)));
  return userDto;
}

oatpp::Object<UserDto> Database::updateUser(const oatpp::Object<UserDto> &userDto) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  collection.update_one(
    createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", userDto->username}
      })
    ),
    createMongoDocument( // <-- Set
      oatpp::Fields<oatpp::Any>({ // map
        { // pair
          "$set", oatpp::Fields<oatpp::Any>({ // you can also define a "strict" DTO for $set operation.
            {"active", userDto->active},
            {"role", userDto->role}
          })
        } // pair
      }) // map
    )
  );

  return userDto;
}

oatpp::Object<UserDto> Database::getUser(const oatpp::String& username) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result =
    collection.find_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", username}
      })
    ));

  if(result) {
    auto view = result->view();
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto user = m_objectMapper.readFromString<oatpp::Object<User>>(bson);
    return dtoFromUser(user);
  }

  return nullptr;
}

oatpp::List<oatpp::Object<UserDto>> Database::getAllUsers() {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto cursor = collection.find(
    createMongoDocument(oatpp::Fields<oatpp::String>({})
  ));

  oatpp::List<oatpp::Object<UserDto>> list({});

  for(auto view : cursor) {
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto user = m_objectMapper.readFromString<oatpp::Object<User>>(bson);
    list->push_back(dtoFromUser(user));
  }

  return list;

}

bool Database::deleteUser(const oatpp::String& username) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result =
    collection.delete_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", username}
      })
    ));

  if(result) {
    return result->deleted_count() == 1;
  }
  return false;
}

}