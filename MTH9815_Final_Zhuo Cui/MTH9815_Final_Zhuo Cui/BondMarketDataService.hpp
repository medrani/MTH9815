/**
 * BondMarketDataService.hpp
 * Defines the data types and Service for BondMarketData.
 *
 * @author Zhuo Cui
 */

#ifndef BONDMARKETDATA_SERVICE_HPP
#define BONDMARKETDATA_SERVICE_HPP

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
#include "BondAlgoExecutionService.hpp"

using namespace std;

class BondMarketDataService : public Service<string, OrderBook<Bond>>
{
private:
	vector<ServiceListener<OrderBook<Bond>>*> m_listeners;

	BondMarketDataService()
	{
		AddListener(BondAlgoExecutionListener::Add_example());
		AddListener(BondHistoricalDataListener::Add_example());
	}

public:
	// Get data on our service given a key
	OrderBook<Bond>& GetData(string key) { };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(OrderBook<Bond>& data) override
	{
		for (auto listener : m_listeners)
		{
			listener->ProcessUpdate(data);
		}
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

	static BondMarketDataService* Add_example()
	{
		static BondMarketDataService example;
		return &example;
	}
};

class BondMarketDataConnector : public Connector<OrderBook<Bond>>
{
private:
	BondMarketDataService* bond_service;

	BondMarketDataConnector()
	{
		bond_service = BondMarketDataService::Add_example();
	}

public:
	void Publish(OrderBook<Bond>& data) override {};

	void ReadMarketData()
	{
		cout << "BondMarketDataConnector called to read the market data" << endl;
		ifstream input_file("marketdata.txt");
		string row;
		PricingSide _bidpricingSide = BID; //initialize the bid price side
		PricingSide _offerpricingSide = OFFER; //initialize the offer price side
		vector<Order> bidOrder; //initialize the bid order vector
		vector<Order> offerOrder; //initialize the offer order vector
		getline(input_file, row);
		while (getline(input_file, row))
		{
			string CUSIPS;
			double bid_price;
			double offer_price;
			long bid_quantity;
			long offer_quantity;
			int bidpivot = 1;
			int offerpivot = 1;

			vector<string> data = readbyline(row);
			CUSIPS = data[0];
			for (int i = 0; i < 5; ++i)
			{
				bid_price = price_translate(data[bidpivot++]);
				bid_quantity = stol(data[bidpivot++]);
				Order bid_order(bid_price, bid_quantity, _bidpricingSide);
				bidOrder.push_back(bid_order);

				offer_price = price_translate(data[offerpivot++]);
				offer_quantity = stol(data[offerpivot++]);
				Order offer_order(offer_price, offer_quantity, _offerpricingSide);
				offerOrder.push_back(offer_order);
			}
			Bond& bond = bond_map[CUSIPS];

			OrderBook<Bond> bond_order_book(bond, bidOrder, offerOrder);
			bond_service->OnMessage(bond_order_book);
		}
	}

	static BondMarketDataConnector* Add_example()
	{
		static BondMarketDataConnector example;
		return &example;
	}

};

#endif