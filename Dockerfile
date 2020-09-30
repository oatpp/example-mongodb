FROM lganzzzo/alpine-mongocxx:latest

ADD src/ /service/src/
ADD test/ /service/test/

ADD utility/ /service/utility/
ADD CMakeLists.txt /service/CMakeLists.txt

WORKDIR /service/utility

RUN ./install-oatpp-modules.sh Release

WORKDIR /service/build

RUN cmake ..
RUN make

EXPOSE 8000 8000

ENTRYPOINT ["./example-mongodb"]
