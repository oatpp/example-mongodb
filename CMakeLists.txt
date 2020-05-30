cmake_minimum_required(VERSION 3.1)

set(project_name example-mongodb)

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

add_executable(${project_name}
        src/db/Database.cpp
        src/db/Database.hpp
        src/AppComponent.hpp
        src/SwaggerComponent.hpp
        src/controller/UserController.hpp
        src/dto/DTOs.hpp
        src/App.cpp
        src/db/Model.hpp)

###################################################################
# Find mongocxx

find_package(mongocxx REQUIRED)

message("LIBBSONCXX_INCLUDE_DIRS=${LIBBSONCXX_INCLUDE_DIRS}")
message("LIBBSONCXX_LIBRARIES=${LIBBSONCXX_LIBRARIES}")

message("LIBMONGOCXX_INCLUDE_DIRS=${LIBMONGOCXX_INCLUDE_DIRS}")
message("LIBMONGOCXX_LIBRARIES=${LIBMONGOCXX_LIBRARIES}")

###################################################################

find_package(oatpp          1.1.0 REQUIRED)
find_package(oatpp-swagger  1.1.0 REQUIRED)
find_package(oatpp-mongo    1.1.0 REQUIRED)

## include directories

target_include_directories(${project_name}
        PUBLIC src
)

## link libs

target_link_libraries(${project_name}
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC oatpp::oatpp-mongo
)

if (TARGET mongo::mongocxx_shared)
    target_link_libraries(${project_name}
            PUBLIC mongo::mongocxx_shared
    )
    message("mongo::mongocxx_shared is used")
elseif(TARGET mongo::mongocxx_static)
    target_link_libraries(${project_name}
            PUBLIC mongo::mongocxx_static
    )
endif()

## define path to swagger-ui res folder

add_definitions(-DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res")

