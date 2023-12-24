/**
 * BondPositionService.hpp
 * Defines the data types and Service for BondPosition.
 *
 * @author Zhuo Cui
 */

#ifndef BONDPOSITION_SERVICE_HPP
#define BONDPOSITION_SERVICE_HPP

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
#include "BondRiskService.hpp"

using namespace std;

class BondPositionServiceListener : public ServiceListener<Position<Bond>>
{
private:
	// BondPositionService should be linked to a BondRiskService via a ServiceListenher
	BondRiskService* bond_service = nullptr;

	BondPositionServiceListener()
	{
		bond_service = BondRiskService::Add_example();
	}

public:
	// Listener callback to process an add event to the BondRiskService
	void ProcessAdd(Position<Bond>& data) override
	{
		bond_service->AddPosition(data);
	}

	// Listener callback to process a remove event to the BondRiskService
	void ProcessRemove(Position<Bond>& data) override { };

	// Listener callback to process an update event to the BondRiskService
	void ProcessUpdate(Position<Bond>& data) override { };

	// Initialize the BondPositionServiceListener object as a pointer
	static BondPositionServiceListener* Add_example()
	{
		static BondPositionServiceListener example;
		return &example;
	}
};

class BondPositionService : public PositionService<Bond>
{
private:
	vector<ServiceListener<Position<Bond>>*> m_listeners;

	BondPositionService()
	{
		AddListener((BondPositionServiceListener::Add_example()));
	}

public:
	// Add a trade to the service
	void AddTrade(const Trade<Bond>& trade) { };

	// Get data on our service given a key
	Position<Bond>& GetData(string key) { Position<Bond> p; return p; };

	// The callback that a Connector should invoke for any new or updated data
	virtual void OnMessage(Position<Bond>& data) override
	{
		for (auto listener_instance : m_listeners)
		{
			listener_instance->ProcessAdd(data);
		}
	}

	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	void AddListener(ServiceListener<Position<Bond>>* listener)
	{
		m_listeners.push_back(listener);
	};

	// Get all listeners on the Service.
	const vector<ServiceListener<Position<Bond>>*>& GetListeners() const
	{
		return m_listeners;
	};

	// Initialize the BondPositionService object as a pointer
	static BondPositionService* Add_example()
	{
		static BondPositionService example;
		return &example;
	}

};

#endif