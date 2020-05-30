
#ifndef example_oatpp_mongo_Model_hpp
#define example_oatpp_mongo_Model_hpp

#include "dto/DTOs.hpp"

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace db {

class User : public oatpp::DTO {

  DTO_INIT(User, DTO)

  DTO_FIELD(String, _id);
  DTO_FIELD(String, username);
  DTO_FIELD(Boolean, active);
  DTO_FIELD(String, role);

};

}

#include OATPP_CODEGEN_END(DTO)

#endif // example_oatpp_mongo_Model_hpp
