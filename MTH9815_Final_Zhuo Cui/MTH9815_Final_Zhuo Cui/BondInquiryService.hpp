/**
 * BondInquiryService.hpp
 * Defines the data types and Service for BondInquiry.
 *
 * @author Zhuo Cui
 */

#ifndef BONDINQUIRY_SERVICE_HPP
#define BONDINQUIRY_SERVICE_HPP

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

class BondInquiryService : public InquiryService<Bond>
{
private:
	vector<ServiceListener<Inquiry<Bond>>*> m_listeners;

	BondInquiryService() { };

public:
	// Get data on our service given a key
	Inquiry<Bond>& GetData(string key) { };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Inquiry<Bond>& data) override
	{
		data.StateDecide(InquiryState::QUOTED);
		BondHistoricalDataService::Add_example()->Persist_Inquiry(data);
	}

	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	void AddListener(ServiceListener<Inquiry<Bond>>* listener)
	{
		m_listeners.push_back(listener);
	};

	// Get all listeners on the Service.
	const vector<ServiceListener<Inquiry<Bond>>*>& GetListeners() const
	{
		return m_listeners;
	};

	void SendQuote(const string& inquiryId, double price) { };
	void RejectInquiry(const string& inquiryId) { };

	static BondInquiryService* Add_example()
	{
		static BondInquiryService example;
		return &example;
	}
};

class BondInquiryConnector : public Connector<OrderBook <Bond>>
{
private:
	BondInquiryService* bond_service;

	BondInquiryConnector()
	{
		bond_service = BondInquiryService::Add_example();
	}

public:
	void Publish(OrderBook<Bond>& data) override {};

	void ReadInquiryData()
	{
		cout << "BondInquiryConnector called to read the inquiry data" << endl;
		int index = 1;
		ifstream input_file("inquiries.txt");
		string row;
		getline(input_file, row);
		while (getline(input_file, row))
		{
			vector<string> input = readbyline(row);
			string cusip = input[0]; string price = input[1]; string vol = input[2]; string side = input[3]; string state = input[4];
			string ID0 = to_string(index);
			Bond& bond0 = bond_map[cusip];
			Side side0 = (side == "BUY") ? BUY : SELL;
			long vol0 = stod(vol);
			double price0 = stod(price);
			InquiryState state0 = (state == "RECEIVED") ? RECEIVED : DONE;

			Inquiry<Bond> inquiry_object(ID0, bond0, side0, vol0, price0, state0);
			bond_service->OnMessage(inquiry_object);
		}
		index++;
	}

	static BondInquiryConnector* Add_example()
	{
		static BondInquiryConnector example;
		return &example;
	}

};


#endif