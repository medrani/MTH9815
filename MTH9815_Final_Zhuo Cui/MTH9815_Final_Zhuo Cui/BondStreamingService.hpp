/**
 * BondStreamingService.hpp
 * Defines the data types and Service for BondStreaming.
 *
 * @author Zhuo Cui
 */

#ifndef BONDSTREAMING_SERVICE_HPP
#define BONDSTREAMING_SERVICE_HPP

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

class BondStreamingService : public StreamingService<Bond>
{
private:
	vector<ServiceListener<PriceStream<Bond>>*> m_listeners;

	BondStreamingService() {};

public:
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

	void PublishPrice(PriceStream<Bond>& priceStream)
	{
		BondHistoricalDataService::Add_example()->Persist_Streaming(priceStream);
	}

	static BondStreamingService* Add_example()
	{
		static BondStreamingService example;
		return &example;
	}
};

#endif
