/**
 * main.cpp
 * main function: test the functionality of the bond services
 * @author Zhuo Cui
 */

#include <iostream>
#include "BondAlgoExecutionService.hpp"
#include "BondAlgoStreamingService.hpp"
#include "BondExecutionService.hpp"
#include "BondInquiryService.hpp"
#include "BondMarketDataService.hpp"
#include "BondPositionService.hpp"
#include "BondPricingService.hpp"
#include "BondRiskService.hpp"
#include "BondStreamingService.hpp"
#include "BondTradeBookingService.hpp"
#include "GUIService.hpp"

using namespace std;

int main()
{
	maps_initial();
	BondTradeBookingConnector::Add_example()->Readtradedata();
	BondMarketDataConnector::Add_example()->ReadMarketData();
	BondPricingConnector::Add_example()->ReadPriceData();
	BondInquiryConnector::Add_example()->ReadInquiryData();

	return 0;
}
