/**
 * BondTradeBookingService.hpp
 * Defines the data types and Service for BondTradeBooking.
 *
 * @author Zhuo Cui
 */

#ifndef BONDTRADEBOOKING_SERVICE_HPP
#define BONDTRADEBOOKING_SERVICE_HPP

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
#include "BondPositionService.hpp"

using namespace std;

class BondTradeBookingServiceListener : public ServiceListener<Trade<Bond>>
{
private:
	BondPositionService* bond_service;

	BondTradeBookingServiceListener()
	{
		bond_service = BondPositionService::Add_example();
	};

public:
	// Listener callback to process an add event to the BondTradeBookingService
	void ProcessAdd(Trade<Bond>& tradedata) override
	{
		string cusip = tradedata.GetProduct().GetProductId();
		double quantity = ((tradedata.GetSide() == BUY) ? tradedata.GetQuantity() : -tradedata.GetQuantity());
		bond_position_map[cusip].position_sum(tradedata.GetBook(), quantity);
		bond_service->OnMessage(bond_position_map[cusip]);
	}

	// Listener callback to process a remove event to the BondTradeBookingService
	void ProcessRemove(Trade<Bond>& data) override {};

	// Listener callback to process an update event to the BondTradeBookingService
	void ProcessUpdate(Trade<Bond>& data) override {};

	// return the BondTradeBookingServiceListener object as a pointer
	static BondTradeBookingServiceListener* Add_example()
	{
		static BondTradeBookingServiceListener example;
		return &example;
	}

};

/**
 * Trade Booking Service to book trades to a particular book.
 * Keyed on trade id.
 * Product type is Bond.
 */
class BondTradeBookingService : public TradeBookingService<Bond>
{
private:
	vector<ServiceListener<Trade<Bond>>*> m_listeners;

	BondTradeBookingService()
	{
		AddListener((BondTradeBookingServiceListener::Add_example()));
	}

public:

	void BookTrade(Trade<Bond>& trade) 
	{
		for (auto listener : m_listeners) {listener->ProcessAdd(trade);}
	}

	// Get data on our service given a key
	Trade<Bond>& GetData(string key) { };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Trade<Bond>& data) override
	{
		BookTrade(data);
	}

	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	void AddListener(ServiceListener<Trade<Bond>>* listener)
	{
		m_listeners.push_back(listener);
	};

	// Get all listeners on the Service.
	const vector<ServiceListener<Trade<Bond>>*>& GetListeners() const
	{
		return m_listeners;
	};

	// return the Bondtradeservice object as a pointer
	static BondTradeBookingService* Add_example()
	{
		static BondTradeBookingService example;
		return &example;
	}

};


class BondTradeBookingConnector : public Connector<Trade<Bond>>
{
private:
	BondTradeBookingService* bond_service;

	BondTradeBookingConnector()
	{
		bond_service = BondTradeBookingService::Add_example();
	}

public:
	void Publish(Trade<Bond>& data) override { };

	void Readtradedata()
	{
		cout << "BondTradeBookingConnector called to read the trade data" << endl;
		ifstream input_file("trades.txt");
		string row;
		double bond_price;
		getline(input_file, row);
		while (getline(input_file, row))
		{
			vector<string> input = readbyline(row);
			string cusip = input[0]; string tradeId = input[1]; string book = input[2]; string vol = input[3]; string side = input[4];
			Bond& bond = bond_map[cusip];
			Side side0 = (side == "BUY") ? BUY : SELL;
			Trade<Bond> bond_trade(bond, tradeId, 100, book, stol(vol), side0);
			bond_service->OnMessage(bond_trade);
		}
	}

	static BondTradeBookingConnector* Add_example()
	{
		static BondTradeBookingConnector example;
		return &example;
	}
};

#endif