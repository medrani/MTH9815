/**
 * BondPricingService.hpp
 * Defines the data types and Service for BondPricing.
 *
 * @author Zhuo Cui
 */

#ifndef BONDPRICING_SERVICE_HPP
#define BONDPRICING_SERVICE_HPP

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
#include "BondStreamingService.hpp"

using namespace std;

/**
 * Pricing Service managing mid prices and bid/offers.
 * Keyed on product identifier.
 * Product type is Bond.
 */
class BondPricingService : public PricingService<Bond>
{
private:
	vector<ServiceListener<Price<Bond>>*> m_listeners;

public:
	// Get data on our service given a key
	Price<Bond>& GetData(string key) { };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Price<Bond>& data) override
	{
		double spread = data.GetBidOfferSpread(); double mid = data.GetMid();
		double offer = mid + spread / 2; double bid = mid - spread / 2;
		PriceStreamOrder pso_o(offer, 0, 0, OFFER);
		PriceStreamOrder pso_b(bid, 0, 0, BID);

		PriceStream<Bond> price_stream(data.GetProduct(), pso_o, pso_b);
		BondStreamingService::Add_example()->PublishPrice(price_stream);
	}

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

	static BondPricingService* Add_example()
	{
		static BondPricingService example;
		return &example;
	}
};

class BondPricingConnector : public Connector<Price<Bond>>
{
private:
	BondPricingService* bond_service;

	BondPricingConnector()
	{
		bond_service = BondPricingService::Add_example();
	}

public:
	void Publish(Price<Bond>& data) override {};

	void ReadPriceData()
	{
		cout << "Pricing connector called to read the price data" << endl;
		ifstream input_file("prices.txt");
		string row;
		getline(input_file, row);
		while (getline(input_file, row))
		{
			vector<string> data = readbyline(row);
			string cusip = data[0]; string mid_price = data[1]; string spread = data[2];
			double mid = price_translate(mid_price);
			double spread_t = price_translate(spread);

			Bond& bond = bond_map[cusip];
			Price<Bond> bond_price(bond, mid, spread_t);
			bond_service->OnMessage(bond_price);
		}
	}

	static BondPricingConnector* Add_example()
	{
		static BondPricingConnector example;
		return &example;
	}

};

#endif