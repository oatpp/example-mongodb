# example-mongodb [![Build Status](https://dev.azure.com/lganzzzo/lganzzzo/_apis/build/status/oatpp.example-mongodb?branchName=master)](https://dev.azure.com/lganzzzo/lganzzzo/_build/latest?definitionId=27&branchName=master)

Example project how to work with MongoDB using [oatpp-mongo](https://github.com/oatpp/oatpp-mongo) mondule.
Project is a web-service with basic CRUD and Swagger-UI.
*Dockerfile and docker-compose.yaml files included.*

More About Oat++:

- [Oat++ Website](https://oatpp.io/)
- [Oat++ Github Repository](https://github.com/oatpp/oatpp)
- [Get Started](https://oatpp.io/docs/start)

## Overview

### Dependencies

- [oatpp](https://github.com/oatpp/oatpp)
- [oatpp-swagger](https://github.com/oatpp/oatpp-swagger)
- [oatpp-mongo](https://github.com/oatpp/oatpp-mongo)
- [mongocxx](http://mongocxx.org/) - Temporary dependency. Until the oatpp-mongo driver will be ready-to-use*

### Project layout

```
|- CMakeLists.txt                        // projects CMakeLists.txt
|- src/
|    |
|    |- controller/                      // Folder containing Controller where all endpoints are declared
|    |- db/                              // Database class is here 
|    |- dto/                             // DTOs are declared here
|    |- App.cpp                          // main() is here
|    |- AppComponent.hpp                 // Service configuration is loaded here
|    |- SwaggerComponent.hpp             // Configuration for swagger-ui
|    
|- utility/install-oatpp-modules.sh      // utility script to install required oatpp-modules.
|- Dockerfile                            // Dockerfile
|- docker-compose.yaml                   // Docker-compose with this service and postgresql
```

## Build and Run

### Using CMake

**Requires** 

- mongocxx installed. To install mongocxx:  
   - On Mac `$ brew install mongo-cxx-driver`
   - On Linux - See Installing mongocxx on Linux section.
   
- `oatpp`, `oatpp-swagger`, `oatpp-mongo` modules installed. You may run `utility/install-oatpp-modules.sh` 
script to install required oatpp modules.   

```
$ mkdir build && cd build
$ cmake ..
$ make 
$ ./example-mongodb  # - run application.
```

### In Docker

#### Dockerfile

To run the web-service only:

```
$ docker build -t example-mongodb .
$ docker run -p 8000:8000 -e DEMO_MONGO_CONN_STR='mongodb://localhost/UserDB' -t example-mongodb
```

#### docker-compose

To run both web-service and mongodb:

```
$ docker-compose up
```


### After run

Go to [http://localhost:8000/swagger/ui](http://localhost:8000/swagger/ui) to try endpoints.

## Installing mongocxx on Linux

Installing mongocxx on Linux is an unclear and painful process.
See [ubuntu-cmake-mongocxx/Dockerfile](https://github.com/oatpp/dockerfiles/blob/master/ci/ubuntu-cmake-mongocxx/Dockerfile)
for instructions that worked for us.
