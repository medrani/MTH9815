/**
 * GUIService.hpp
 * Defines the data types and Service for GUI.
 *
 * @author Zhuo Cui
 */

#ifndef GUI_SERVICE_HPP
#define GUI_SERVICE_HPP

#include <string>
#include "soa.hpp"
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
#include "BondPricingService.hpp"
#include "BondHistoricalDataService.hpp"

using namespace std;

class GUIService : public Service<string, Price<Bond>>
{
private:
	vector<ServiceListener<Price<Bond>>*> m_listeners;

	BondPricingService* bond_service;

	GUIService()
	{
		bond_service = BondPricingService::Add_example();
	}

public:

	// Get data on our service given a key
	PriceStream<Bond>& GetData(string key) { };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Price<Bond>& data) override {};

	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	void AddListener(ServiceListener<Price<Bond>>* listener)
	{
		m_listeners.push_back(listener);
	};

	// Get all listeners on the Service.
	const vector<ServiceListener<Price<Bond>>*>& GetListeners() const
	{
		return m_listeners;
	};

	static GUIService* Add_example()
	{
		static GUIService example;
		return &example;
	}
};

#endif
