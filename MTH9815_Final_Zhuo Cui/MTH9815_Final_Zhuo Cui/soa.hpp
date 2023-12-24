/**
 * soa.hpp
 * Definition of our Service Oriented Architecture (SOA) Service base class
 *
 * @author Breman Thuraisingham
 */

#ifndef SOA_HPP
#define SOA_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

using namespace std;

/**
 * Definition of a generic base class ServiceListener to listen to add, update, and remove
 * events on a Service. This listener should be registered on a Service for the Service
 * to notify all listeners for these events.
 */
template<typename V>
class ServiceListener
{

public:

	// Listener callback to process an add event to the Service
	virtual void ProcessAdd(V& data) = 0;

	// Listener callback to process a remove event to the Service
	virtual void ProcessRemove(V& data) = 0;

	// Listener callback to process an update event to the Service
	virtual void ProcessUpdate(V& data) = 0;

};

/**
 * Definition of a generic base class Service.
 * Uses key generic type K and value generic type V.
 */
template<typename K, typename V>
class Service
{

public:

	// Get data on our service given a key
	V& GetData(K key) { };

	// The callback that a Connector should invoke for any new or updated data
	virtual void OnMessage(V& data) = 0;

	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	virtual void AddListener(ServiceListener<V>* listener) = 0;

	// Get all listeners on the Service.
	virtual const vector< ServiceListener<V>* >& GetListeners() const = 0;

};

/**
 * Definition of a Connector class.
 * This will invoke the Service.OnMessage() method for subscriber Connectors
 * to push data to the Service.
 * Services can invoke the Publish() method on this Service to publish data to the Connector
 * for a publisher Connector.
 * Note that a Connector can be publisher-only, subscriber-only, or both publisher and susbcriber.
 */
template<typename V>
class Connector
{

public:

	// Publish data to the Connector
	virtual void Publish(V& data) = 0;

};

vector<string> readbyline(string row)
{
	stringstream input_file(row);
	vector<string> input; string in;
	while (getline(input_file, in, ' ')) { input.push_back(in); }
	return input;
}

// Translate price of the form '100-xyz' to double
double price_translate(string price)
{
	double xy = stoi(price.substr(5, 2));
	double z = stoi(price.substr(7, 1));
	double ret = 100 + xy / 32.0 + z / 256.0;
	return ret;
}

#endif
