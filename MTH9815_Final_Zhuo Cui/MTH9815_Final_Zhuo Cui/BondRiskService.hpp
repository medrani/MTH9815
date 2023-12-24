/**
 * BondRiskService.hpp
 * Defines the data types and Service for BondRisk.
 *
 * @author Zhuo Cui
 */

#ifndef BONDRISK_SERVICE_HPP
#define BONDRISK_SERVICE_HPP

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

class BondRiskService : public RiskService<Bond>
{
private:
	vector<ServiceListener<PV01<Bond>>*> m_listeners;
	map<string, double> m_risk_map;
	Bond m_product;

	BondRiskService() { };

public:
	// Get data on our service given a key
	PV01<Bond>& GetData(string key) { };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(PV01<Bond>& data) override { };

	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	void AddListener(ServiceListener<PV01<Bond>>* listener)
	{
		m_listeners.push_back(listener);
	};

	// Get all listeners on the Service.
	const vector<ServiceListener<PV01<Bond>>*>& GetListeners() const
	{
		return m_listeners;
	};

	// Add new position
	void AddPosition(Position<Bond>& bondposition)
	{
		m_risk_map[CUSIPS[0]] = 0.02;
		m_risk_map[CUSIPS[1]] = 0.03;
		m_risk_map[CUSIPS[2]] = 0.05;
		m_risk_map[CUSIPS[3]] = 0.07;
		m_risk_map[CUSIPS[4]] = 0.10;
		m_risk_map[CUSIPS[5]] = 0.20;
		m_risk_map[CUSIPS[6]] = 0.30;
		
		double pv01_risk = m_risk_map[bondposition.GetProduct().GetProductId()];
		long pv01_vol = bondposition.GetAggregatePosition();
		double pv01 = pv01_vol * pv01_risk;
		m_product = bondposition.GetProduct();
		PV01<Bond> pv01_0(m_product, pv01, pv01_vol);
		BondHistoricalDataService::Add_example()->Persist_Risk(pv01_0);
	}

	// Initialize the BondRiskService object as a pointer
	static BondRiskService* Add_example()
	{
		static BondRiskService example;
		return &example;
	}
};

#endif
