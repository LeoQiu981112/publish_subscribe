Publisher-Suscriber c++ implementation
==========================================
This is an implementation of the Publish/Subscribe model which uses the rabbitmq server 
and one of the provided c++ client library from the offical site, SimpleAmqpClient.
Interactions with MongoDB are realized through the mongocxx driver.   

Required Packages, Libraries and their dependencies:
-----------------------------------------------------

(since the lowest cmake version requirement for one of the libaries is 3.2, cmake 3.2 or later should be installed)

rabbitmq-server
https://packagecloud.io/rabbitmq/rabbitmq-server
dependencies: 
erlang  

c++ wrapper: SimpleAmqpClient  
https://github.com/alanxz/SimpleAmqpClient
dependencies:
boost   1.47.0+
rabbitmq-c  0.5.1+ 
cmake  2.8+

RabbitMQ C AMQP client library: librabbitmq
https://github.com/alanxz/rabbitmq-c
dependencies:
boost-1.47.0+
OpenSSL v0.9.8+ 
A C compiler (GCC 4.4+)
cmake  2.6+

c++ JSON library: nlohmann json 
https://github.com/nlohmann/json

mongodb c++ driver: mongocxx
http://mongocxx.org/mongocxx-v3/installation/
dependencies:
A compiler that suports C++11 (gcc, clang, or Visual Studio)
libmongoc
cmake 3.2+  

mongoDB C driver: libmongoc
http://mongoc.org/libmongoc/current/installing.html
dependencies:
Sphinx
OpenSSL 

BSON library: libbson-1.0
http://mongoc.org/libmongoc/current/installing.html

How to compile
---------------

in terminal do:
g++ Send.cpp -o Send -lSimpleAmqpClient -pthread -std=c++11 
g++ Recv.cpp -o Recv -lSimpleAmqpClient $(pkg-config --cflags --libs libmongocxx) -Wl, -rpath,/usr/local/lib 
(order interchangeable)

Running the code
-----------------

in terminal do:
mongod (if running mongod gives an error, namely "exception in init and listen: Nonexistentpath: Data directory /dat/db not found",
then in that case create an folder named mongo and create a folder named db inside of it, and run in terminal:   mongod --dbpath ~/somedirectory/mongo/db )
./Recv
./Send

Test for MongoDB data insertion
-------------------------------

in terminal do:
mongo  

then in mongo shell do:
use local
db.grade.find()
db.health.find()
db.temperature.find()


resetting
---------

delete the inserted collections from MongoDB 

In mongo shell do:
use local
db.grade.drop()
db.health.drop()
db.temperature.drop()

can now rerun the code