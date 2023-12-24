/**
 * BondHistoricalDataService.hpp
 * Defines the data types and Service for BondHistoricalData.
 *
 * @author Zhuo Cui
 */

#ifndef BONDHISTORICALDATA_SERVICE_HPP
#define BONDHISTORICALDATA_SERVICE_HPP

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

using namespace std;

class RiskConnector : public Connector<string>
{
public:
	RiskConnector() {};

	// Output to risk.txt
	void Publish(string& data) override
	{
		ofstream output_file("risk.txt", ios_base::app);
		output_file << data << endl;
	}

	static RiskConnector* Add_example()
	{
		static RiskConnector example;
		return &example;
	}
};


class ExecutionConnector : public Connector<string>
{
public:
	ExecutionConnector() {};

	// Output to execution.txt
	void Publish(string& data) override
	{
		ofstream output_file("execution.txt", ios_base::app);
		output_file << data << endl;
	}

	static ExecutionConnector* Add_example()
	{
		static ExecutionConnector example;
		return &example;
	}
};


class StreamingConnector : public Connector<string>
{
public:
	StreamingConnector() {};

	// Output to streaming.txt
	virtual void Publish(string& data) override
	{
		ofstream output_file("streaming.txt", ios_base::app);
		output_file << data << endl;
	}

	static StreamingConnector* Add_example()
	{
		static StreamingConnector example;
		return &example;
	}
};


class InquiryConnector : public Connector<string>
{
public:
	InquiryConnector() {};

	// Output to allinquiries.txt
	virtual void Publish(string& data) override
	{
		ofstream output_file("allinquiries.txt", ios_base::app);
		output_file << data << endl;
	}

	static InquiryConnector* Add_example()
	{
		static InquiryConnector example;
		return &example;
	}
};

class BondHistoricalDataService : public HistoricalDataService<Bond>
{
private:
	vector<ServiceListener<Bond>*> m_listeners;

public:
	// Get data on our service given a key
	Bond& GetData(string key) { };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Bond& data) override { };

	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	void AddListener(ServiceListener<Bond>* listener)
	{
		m_listeners.push_back(listener);
	};

	// Get all listeners on the Service.
	const vector<ServiceListener<Bond>*>& GetListeners() const
	{
		return m_listeners;
	};

	void PersistData(string persistKey, const Bond& data) { };
	void Persist_Position() {};

	// Persist object the BondRiskService receives into risk.txt
	RiskConnector* riskconnector;
	void Persist_Risk(PV01<Bond>& data)
	{
		cout << "RiskConnector called to persist risk data" << endl;
		string input = "PV01: " + to_string(data.GetPV01());
		riskconnector->Publish(input);
	}

	// Persist object the BondExecutionService receives into execution.txt
	ExecutionConnector* executionconnector;
	void Persist_Execution(string data)
	{
		cout << "ExecutionConnector called to persist execusion data" << endl;
		executionconnector->Publish(data);
	}

	// Persist object the BondStreamingService receives into streaming.txt
	StreamingConnector* streamingconnector;
	void Persist_Streaming(PriceStream<Bond>& data)
	{
		cout << "StreamingConnector called to persist streaming data" << endl;
		string input = "Order ID: " + data.GetProduct().GetProductId();
		streamingconnector->Publish(input);
	}

	// Persist object the BondInquiryService receives into allinquiries.txt
	InquiryConnector* inquiryconnector;
	void Persist_Inquiry(Inquiry<Bond>& data)
	{
		cout << "InquiryConnector called to persist inquiry data" << endl;
		string input = "Inquiry Id: " + data.GetInquiryId();
		inquiryconnector->Publish(input);
	}

	static BondHistoricalDataService* Add_example()
	{
		static BondHistoricalDataService example;
		return &example;
	}

	BondHistoricalDataService()
	{
		riskconnector = RiskConnector::Add_example();
		executionconnector = ExecutionConnector::Add_example();
		streamingconnector = StreamingConnector::Add_example();
		inquiryconnector = InquiryConnector::Add_example();
	}
};


class BondHistoricalDataListener : public ServiceListener<OrderBook<Bond>>
{
private:
	BondHistoricalDataService* bond_service;

public:
	BondHistoricalDataListener()
	{
		bond_service = BondHistoricalDataService::Add_example();
	}

	void ProcessAdd(OrderBook<Bond>& data) override { };
	void ProcessRemove(OrderBook<Bond>& data) override { };
	void ProcessUpdate(OrderBook<Bond>& data) override { };

	static BondHistoricalDataListener* Add_example()
	{
		static BondHistoricalDataListener example;
		return &example;
	}
};

#endif
