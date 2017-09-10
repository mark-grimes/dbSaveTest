FROM alpine

RUN apk add --no-cache g++ sqlite sqlite-dev mariadb-dev

ADD compile.sh sqliteTest.cpp /code/
RUN cd /code && ./compile.sh && ln -s /code/sqliteTest /usr/bin

VOLUME ["/output"]
WORKDIR /output
