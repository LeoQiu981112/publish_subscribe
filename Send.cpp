#include <string.h>
#include <iostream>
#include <fstream>
#include <thread>

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <nlohmann/json.hpp>
using namespace std;

// Publish grade data 
void SendGradeData(){
  // messagequeue publisher set up
  AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");
  string queue_name = "q1"; //routing key
  string exchange_name = "ex1";
  channel -> DeclareExchange(exchange_name,
    AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
  channel -> DeclareQueue(/* queue_name= */ "q1", 
    /* passive= */ false, 
    /* durable= */ false, 
    /* exculsive= */ false, 
    /* auto_delete= */ false);
  channel->BindQueue(queue_name, exchange_name, "mtl.grade");

  // process json file and publish data
  string message;
  ifstream i("mtl_grade.json");
  nlohmann::json j;
  i >> j;
  for (nlohmann::json::iterator it = j.begin(); it != j.end(); it++) {
    nlohmann::json k =  (*it);
    message = k.dump(); 
    channel -> BasicPublish(exchange_name, "mtl.grade", 
      AmqpClient::BasicMessage::Create(message),true);
    cout << "DEBUG: publisher1 send grade msg: " << message << endl;
  }
}

// Publish health data 
void SendHealthData(){
  // messagequeue publisher set up
  AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");
  string queue_name = "q1"; //routing key
  string exchange_name = "datacenter";
  channel->DeclareExchange(exchange_name,
    AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
  channel->DeclareQueue(/* queue_name= */ "q1", 
    /* passive= */ false, 
    /* durable= */ false, 
    /* exculsive= */ false, 
    /* auto_delete= */ false);
  channel->BindQueue(queue_name, exchange_name, "mtl.health");

  // process json file and publish data
  string message;
  ifstream i("mtl_health.json");
  nlohmann::json j;
  i >> j;
  for (nlohmann::json::iterator it = j.begin(); it != j.end(); it++) {
    nlohmann::json k =  (*it);
    message = k.dump(); 
    channel->BasicPublish(exchange_name, "mtl.health", 
      AmqpClient::BasicMessage::Create(message),true);
    cout << "DEBUG: publisher2 send health msg: " << message << endl;
  }
}

// Publish temperature data
void SendTemperatureData(){
  // messagequeue publisher set up
  AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");
  string queue_name = "q1"; //routing key
  string exchange_name = "datacenter";
  channel->DeclareExchange(exchange_name,
   AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
  channel->DeclareQueue(/* queue_name= */ "q1", 
    /* passive= */ false, 
    /* durable= */ false, 
    /* exculsive= */ false, 
    /* auto_delete= */ false);
  channel->BindQueue(queue_name, exchange_name, "mtl.temperature");

  // process json file and publish data
  string message;
  ifstream i("mtl_temperature.json");
  nlohmann::json j;
  i >> j;
  for (nlohmann::json::iterator it = j.begin(); it != j.end(); it++) {
    nlohmann::json k =  (*it);
    message = k.dump(); 
    channel->BasicPublish(exchange_name, "mtl.temperature", 
      AmqpClient::BasicMessage::Create(message),true);
    cout << "DEBUG: publisher3 send temperature msg: " << message << endl;
  }
}

int main() {
  // run all three publishers
  thread t1(SendGradeData);
  thread t2(SendHealthData);
  thread t3(SendTemperatureData);

  t1.join(); 
  t2.join(); 
  t3.join(); 
}



