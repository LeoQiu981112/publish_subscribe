#include <iostream>
#include <string.h>

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <nlohmann/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

using namespace std;

// Process the grades data published by sender
void ProcessGradeData(string str,mongocxx::database* dbname ){
	// store json data in variables
	auto data = nlohmann::json::parse(str);
	cout << "DEBUG: stamp: " << data["timestamp"] << '\n';
	cout << "DEBUG: ni: " << data["grades"][0]["ni"] << '\n';
	cout << "DEBUG: cu: " << data["grades"][0]["cu"] << '\n' << '\n';
	string stamp = data["timestamp"].dump();
	string ni = data["grades"][0]["ni"].dump();
	string cu = data["grades"][0]["cu"].dump();

	// create document & insert into specified db collection
	mongocxx::collection coll = (*dbname)["grade"];
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder  
	<< "_id" << stamp
	<< "grades" << bsoncxx::builder::stream::open_array
	<< bsoncxx::builder::stream::open_document
	<< "ni" << ni
	<< "cu" << cu
	<< bsoncxx::builder::stream::close_document
	<< close_array
	<< bsoncxx::builder::stream::finalize;
	coll.insert_one(doc_value.view());
}

// Process the health data published by sender
void ProcessHealthData(string str,mongocxx::database* dbname ){
	// store json data in variables
	auto data = nlohmann::json::parse(str);
	cout << "DEBUG: stamp: " << data["timestamp"] << '\n';
	cout << "DEBUG: health_status: " << data["health_status"] << '\n';
	string stamp = data["timestamp"].dump();
	string health_status = data["health_status"].dump();

	// create document & insert into specified db collection
	mongocxx::collection coll = (*dbname)["health"];
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder  
	<< "_id" << stamp
	<< "health_status" << health_status
	<< bsoncxx::builder::stream::finalize;
	coll.insert_one(doc_value.view());
}

// Process the temperature data published by sender
void ProcessTemperatureData(string str,mongocxx::database* dbname ){
	// store json data in variabless
	auto data = nlohmann::json::parse(str);
	cout << "DEBUG: stamp: " << data["timestamp"] << '\n';
	cout << "DEBUG: temperature: " << data["temperature"] << '\n';
	string stamp = data["timestamp"].dump();
	string temperature = data["temperature"].dump();

	// create document & insert into specified db collection
	mongocxx::collection coll = (*dbname)["temperature"];
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder 
	<< "_id" << stamp
	<< "temperature" << temperature
	<< bsoncxx::builder::stream::finalize;
	coll.insert_one(doc_value.view());
}

int main() {
	// db set up
	mongocxx::instance instance{}; 
	mongocxx::uri uri("mongodb://localhost:27017");
	mongocxx::client client(uri);
	mongocxx::database local = client["local"];

	// messagequeue subscriber set up
	AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");
	string queue_name = "q1";
	string exchange_name = "ex1";
	channel -> DeclareExchange(exchange_name,
		AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
	channel -> DeclareQueue(/* queue_name= */ "q1", 
		/* passive= */ false, 
		/* durable= */ false, 
		/* exculsive= */ false, 
		/* auto_delete= */ false);
	channel -> BindQueue(queue_name, exchange_name, "mtl.grade");
	channel -> BindQueue(queue_name, exchange_name, "mtl.health");
	channel -> BindQueue(queue_name, exchange_name, "mtl.temperature");
	string consumer_tag = channel -> BasicConsume(queue_name, 
	    /* consume_tag= */ "consumer1",  
	    /* no_local= */ false, 
	    /* no_ack=*/ true, 
	    /* exclusive=*/ false, 
	    /* message prefetch count */ 1);

	// wait for publisher to publish data
	while (1) {
		cout << "DEBUG: subscriber wait for the message" << endl;   
		AmqpClient::Envelope::ptr_t envelope =
		channel -> BasicConsumeMessage(consumer_tag);

		string type = envelope -> RoutingKey();
		string buffer = envelope -> Message() -> Body();
		if (type == "mtl.grade"){
			ProcessGradeData(buffer, &local);
		} else if (type == "mtl.health"){
			ProcessHealthData(buffer, &local);
		} else if (type == "mtl.temperature"){
			ProcessTemperatureData(buffer, &local);
		}
	}
	channel -> BasicCancel(consumer_tag);
	return 0;
}
