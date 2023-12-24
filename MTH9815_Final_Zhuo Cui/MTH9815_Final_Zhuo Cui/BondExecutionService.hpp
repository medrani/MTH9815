/**
 * BondExecutionService.hpp
 * Defines the data types and Service for BondExecutions.
 *
 * @author Zhuo Cui
 */

#ifndef BONDEXECUTION_SERVICE_HPP
#define BONDEXECUTION_SERVICE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include "soa.hpp"
#include "executionservice.hpp"
#include "historicaldataservice.hpp"
#include "inquiryservice.hpp"
#include "marketdataservice.hpp"
#include "positionservice.hpp"
#include "pricingservice.hpp"
#include "products.hpp"
#include "riskservice.hpp"
#include "streamingservice.hpp"
#include "tradebookingservice.hpp"
#include "BondHistoricalDataService.hpp"

using namespace std;

class BondExecutionService : public ExecutionService<Bond>
{
private:
	vector<ServiceListener<ExecutionOrder<Bond>>*> m_listeners;
	BondHistoricalDataService* bond_service;

public:
	// Get data on our service given a key
	ExecutionOrder<Bond>& GetData(string key) { };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(ExecutionOrder<Bond>& data) override {};

	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	void AddListener(ServiceListener<ExecutionOrder<Bond>>* listener)
	{
		m_listeners.push_back(listener);
	};

	// Get all listeners on the Service.
	const vector<ServiceListener<ExecutionOrder<Bond>>*>& GetListeners() const
	{
		return m_listeners;
	};

	void ExecuteOrder(const ExecutionOrder<Bond>& order, Market market)
	{
		cout << "BondExecutionService called to execute the order" << endl;

		bond_service = BondHistoricalDataService::Add_example();
		string s = order.GetProduct().GetProductId() + order.GetOrderId();

		bond_service->Persist_Execution(s);
	}

	static BondExecutionService* Add_example()
	{
		static BondExecutionService example;
		return &example;
	};
};

#endif

