/**
 * BondAlgoExecutionService.hpp
 * Defines the data types and Service for BondAlgoExecution.
 *
 * @author Zhuo Cui
 */

#ifndef BONDALGOEXECUTION_SERVICE_HPP
#define BONDALGOEXECUTION_SERVICE_HPP

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
#include "BondExecutionService.hpp"

using namespace std;

class BondAlgoExecutionService : public Service<string, OrderBook<Bond>>
{
private:
	vector<ServiceListener<OrderBook<Bond>>*> m_listeners;

	// BondExecutionService object pointer
	BondExecutionService* bond_service = nullptr;

	BondAlgoExecutionService()
	{
		bond_service = BondExecutionService::Add_example();
	}

public:
	// Get data on our service given a key
	OrderBook<Bond>& GetData(string key) { };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(OrderBook<Bond>& data) override
	{
		Bond bond = data.GetProduct();
		string order_id;

		int ID = 1; order_id = to_string(ID); ID++;

		// Alternating between bid and offer
		const Order& order_b = data.GetBidStack()[0];
		ExecutionOrder<Bond> eo_b(bond, BID, order_id, MARKET, order_b.GetPrice(), order_b.GetQuantity(), 0.0, " ", false);
		(*bond_service).ExecuteOrder(eo_b, CME);

		ID++;
		const Order& order_o = data.GetOfferStack()[0];
		ExecutionOrder<Bond> eo_o(bond, OFFER, order_id, MARKET, order_o.GetPrice(), order_o.GetQuantity(), 0.0, " ", false);
		(*bond_service).ExecuteOrder(eo_o, CME);
	}

	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	void AddListener(ServiceListener<OrderBook<Bond>>* listener)
	{
		m_listeners.push_back(listener);
	};

	// Get all listeners on the Service.
	const vector<ServiceListener<OrderBook<Bond>>*>& GetListeners() const
	{
		return m_listeners;
	};

	// Initialize the BondAlgoExecutionService object as a pointer
	static BondAlgoExecutionService* Add_example()
	{
		static BondAlgoExecutionService example;
		return &example;
	}

};

class BondAlgoExecutionListener : public ServiceListener<OrderBook<Bond>>
{
private:
	// BondExecutionService object pointer
	BondAlgoExecutionService* bond_service;

	BondAlgoExecutionListener()
	{
		bond_service = BondAlgoExecutionService::Add_example();
	}

public:
	// Listener callback to process an add event to the Service
	void ProcessAdd(OrderBook<Bond>& data) override { };

	// Listener callback to process a remove event to the Service
	void ProcessRemove(OrderBook<Bond>& data) override { };

	// Listener callback to process an update event to the Service
	void ProcessUpdate(OrderBook<Bond>& data) override
	{
		bond_service->OnMessage(data);
	}

	// Initialize the BondAlgoExecutionListener object as a pointer
	static BondAlgoExecutionListener* Add_example()
	{
		static BondAlgoExecutionListener example;
		return &example;
	}

};

#endif
