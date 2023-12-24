/**
 * BondAlgoStreamingService.hpp
 * Defines the data types and Service for BondAlgoStreaming.
 *
 * @author Zhuo Cui
 */

#ifndef BONDALGOSTREAMING_SERVICE_HPP
#define BONDALGOSTREAMING_SERVICE_HPP

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

class BondAlgoStreamingService : public Service<string, PriceStream<Bond>>
{
private:
	vector<ServiceListener<PriceStream<Bond>>*> m_listeners;

	BondPricingService* bond_service;

	BondAlgoStreamingService()
	{
		bond_service = BondPricingService::Add_example();
	}

public:
	// Publish two-way prices
	void PublishPrice(PriceStream<Bond>& priceStream)
	{
		BondHistoricalDataService::Add_example()->Persist_Streaming(priceStream);
	}

	// Get data on our service given a key
	PriceStream<Bond>& GetData(string key) { };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(PriceStream<Bond>& data) override {};

	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	void AddListener(ServiceListener<PriceStream<Bond>>* listener)
	{
		m_listeners.push_back(listener);
	};

	// Get all listeners on the Service.
	const vector<ServiceListener<PriceStream<Bond>>*>& GetListeners() const
	{
		return m_listeners;
	};

	static BondAlgoStreamingService* Add_example()
	{
		static BondAlgoStreamingService example;
		return &example;
	}
};

#endif
