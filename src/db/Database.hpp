
#ifndef example_oatpp_mongo_Database_hpp
#define example_oatpp_mongo_Database_hpp

#include "dto/DTOs.hpp"
#include "Model.hpp"

#include "oatpp-mongo/bson/mapping/ObjectMapper.hpp"

#include <mongocxx/pool.hpp>
#include <bsoncxx/document/value.hpp>

namespace db {

class Database {
private:
  std::shared_ptr<mongocxx::pool> m_pool;
  std::string m_databaseName;
  std::string m_collectionName;
  oatpp::mongo::bson::mapping::ObjectMapper m_objectMapper;
private:
  oatpp::Object<User> userFromDto(const oatpp::Object<UserDto>& dto);
  oatpp::Object<UserDto> dtoFromUser(const oatpp::Object<User>& user);
private:
  bsoncxx::document::value createMongoDocument(const oatpp::Void &polymorph);
public:

  Database(const mongocxx::uri &uri, const std::string &dbName, const std::string &collectionName);

  oatpp::Object<UserDto> createUser(const oatpp::Object<UserDto> &userDto);
  oatpp::Object<UserDto> updateUser(const oatpp::Object<UserDto> &userDto);
  oatpp::Object<UserDto> getUser(const oatpp::String& username);
  oatpp::List<oatpp::Object<UserDto>> getAllUsers();

  bool deleteUser(const oatpp::String& username);

};

}

#endif //example_oatpp_mongo_Database_hpp
