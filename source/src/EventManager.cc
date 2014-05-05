  /// \file EventManager.cc
/*
 *
 * EventManager.cc source template automatically generated by a class generator
 * Creation date : lun. avr. 28 2014
 *
 * This file is part of SDHCALEventDisplay libraries.
 * 
 * SDHCALEventDisplay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * SDHCALEventDisplay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with SDHCALEventDisplay.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#include "EventManager.h"

// sdhcal
#include "EventNavigator.h"
#include "Config.h"
#include "CaloHit.h"
#include "Gui.h"
#include "LcioCollectionFrame.h"
#include "CaloHitHelper.h"

// lcio
#include "EVENT/LCEvent.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"
#include "EVENT/CalorimeterHit.h"
#include "UTIL/CellIDDecoder.h"


// streamlog
#include "streamlog/streamlog.h"

// gear
#include "gear/GEAR.h"
#include "gearxml/GearXML.h"
#include "GearMgr.h"
#include "gear/CalorimeterParameters.h"
#include "gear/LayerLayout.h"

// root
#include "TEveBoxSet.h"
#include "TEveBox.h"
#include "TEveManager.h"

extern TEveManager *gEve;

ClassImpQ(sdhcal::EventManager)

namespace sdhcal
{

//-------------------------------------------------------------------------------------------

EventManager::EventManager(EventNavigator *pEventNavigator)
{
	m_pEventNavigator = pEventNavigator;
	m_pEventNavigator->Connect("eventUpdated()", "sdhcal::EventManager", this, "loadCurrentEvent()");

	m_pEveCaloHitList = new TEveBoxSet();

	m_caloHitColorDisplay = SEMI_DIGITAL_THRESHOLD;
}

//-------------------------------------------------------------------------------------------

EventManager::~EventManager()
{
	delete m_pEveCaloHitList;
}

//-------------------------------------------------------------------------------------------

void EventManager::loadCurrentEvent()
{
	this->clear();
	EVENT::LCEvent *pLCEvent = m_pEventNavigator->getCurrentEvent();

	if(NULL != pLCEvent)
	{
		std::string sdhcalCollectionName = Gui::getInstance()->getLcioCollectionFrame()->getSDHCALCollectionName();

		delete m_pEveCaloHitList;
		m_pEveCaloHitList = new TEveBoxSet();
		m_pEveCaloHitList->SetName(sdhcalCollectionName.c_str());
		m_pEveCaloHitList->SetPickable(true);
		m_pEveCaloHitList->Reset(TEveBoxSet::kBT_AABox, kTRUE, 64);
		m_pEveCaloHitList->SetOwnIds(true);
		m_pEveCaloHitList->SetMainTransparency(100);
		m_pEveCaloHitList->SetAntiFlick(true);

		try
		{
			EVENT::LCCollection *pCurrentCollection = pLCEvent->getCollection(sdhcalCollectionName);
			std::string collectionTypeName = pCurrentCollection->getTypeName();

			if(collectionTypeName == EVENT::LCIO::CALORIMETERHIT)
			{
				try
				{
					this->loadCaloHitCollection(pCurrentCollection);
					CaloHitHelper::CalculateCaloHitProperties(m_currentOrderedCaloHitList);
					gEve->AddElement(m_pEveCaloHitList);
					gEve->Redraw3D();
					this->caloHitsAvailable();
				}
				catch(EVENT::Exception &e)
				{
					streamlog_out(WARNING) << "Exception thrown while loading collection : " << sdhcalCollectionName << ". Message : " << e.what() << std::endl;
				}
			}
			else
				throw EVENT::Exception("SDHCAL collection type mismatch. Expected CALORIMETERHIT !");

		}
		catch(EVENT::Exception &e)
		{
			streamlog_out(WARNING) << "Couldn't load sdhcal collection " << sdhcalCollectionName << ". Message : " << e.what() << std::endl;
		}
		catch(gear::Exception &e)
		{
			streamlog_out(WARNING) << "Couldn't load sdhcal collection " << sdhcalCollectionName << ". GEAR message : " << e.what() << std::endl;
		}

	}
}

//-------------------------------------------------------------------------------------------

void EventManager::loadCaloHitCollection(EVENT::LCCollection *pCollection)
{
	if(LCIO::CALORIMETERHIT != pCollection->getTypeName())
			throw EVENT::Exception("Bad collection type while loading calorimeter hits");

		UTIL::CellIDDecoder<EVENT::CalorimeterHit> cellIDDecoder(pCollection);
		const std::string layerCodingString(pCollection->getParameters().getStringVal(LCIO::CellIDEncoding));

		std::string layerCoding;

		if(layerCodingString.empty())
		{
			layerCoding = "K-1";
		}
		else
		{
			layerCoding = (layerCodingString.find("K-1") == std::string::npos) ? "K" : "K-1";
		}

		std::string cellIDCodingI("I");
		std::string cellIDCodingJ("J");

		gear::GearMgr *pGearMgr = Config::getInstance()->getGearMgr();

		if(NULL == pGearMgr)
			throw EVENT::Exception("Gear file not parsed yet. Please parse the gear file before loading any collection !");

		const gear::CalorimeterParameters &calorimeterParameters = pGearMgr->getHcalEndcapParameters();
		const gear::LayerLayout &layerLayout = calorimeterParameters.getLayerLayout();

		float sdhcalThreshold1(static_cast<float>(calorimeterParameters.getDoubleVal("SemiDigitalThreshold1")));
		float sdhcalThreshold2(static_cast<float>(calorimeterParameters.getDoubleVal("SemiDigitalThreshold2")));
		float sdhcalThreshold3(static_cast<float>(calorimeterParameters.getDoubleVal("SemiDigitalThreshold3")));

		streamlog_out(MESSAGE) << "Loading calo hits size : " << pCollection->getNumberOfElements() << std::endl;

		for(unsigned int elt=0 ; elt<pCollection->getNumberOfElements() ; elt++)
		{
			EVENT::CalorimeterHit *pCaloHit = NULL;
			pCaloHit = dynamic_cast<EVENT::CalorimeterHit*>(pCollection->getElementAt(elt));

			if(NULL == pCaloHit)
				throw EVENT::Exception("Bad element type while loading calorimeter hits");

			CaloHitCell caloHitCell;
			caloHitCell.m_iCell   = cellIDDecoder(pCaloHit)[cellIDCodingI.c_str()];
			caloHitCell.m_jCell   = cellIDDecoder(pCaloHit)[cellIDCodingJ.c_str()];
			caloHitCell.m_layer   = cellIDDecoder(pCaloHit)[layerCoding.c_str()];
			caloHitCell.m_cellSize0 = layerLayout.getCellSize0(caloHitCell.m_layer);
			caloHitCell.m_cellSize1 = layerLayout.getCellSize1(caloHitCell.m_layer);
			caloHitCell.m_cellThickness = fabs(layerLayout.getThickness(caloHitCell.m_layer) - layerLayout.getAbsorberThickness(caloHitCell.m_layer));

			float layerThickness = layerLayout.getThickness(caloHitCell.m_layer);
			float absorberThickness = layerLayout.getAbsorberThickness(caloHitCell.m_layer);

			float posX = caloHitCell.m_iCell*caloHitCell.m_cellSize0 - 96*caloHitCell.m_cellSize0/2.0 - caloHitCell.m_cellSize0/2.0;
			float posY = caloHitCell.m_jCell*caloHitCell.m_cellSize1 - 96*caloHitCell.m_cellSize1/2.0 - caloHitCell.m_cellSize1/2.0;
			float posZ = caloHitCell.m_layer*layerThickness + absorberThickness + (layerThickness-absorberThickness)/2.0;

			CartesianVector caloHitPosition(posX, posY,	posZ);

			SemiDigitalThreshold threshold;

			if(sdhcalThreshold1 == pCaloHit->getEnergy())
			{
				threshold = THRESHOLD_1;
			}
			else if(sdhcalThreshold2 == pCaloHit->getEnergy())
			{
				threshold = THRESHOLD_2;
			}
			else if(sdhcalThreshold3 == pCaloHit->getEnergy())
			{
				threshold = THRESHOLD_3;
			}
			else
			{
				std::stringstream ss;
				ss << "While loading calo hit : semi digital threshold bad matching. Should be "
						"(" << sdhcalThreshold1 << "," << sdhcalThreshold2 << "" << sdhcalThreshold3 << ").";
				throw EVENT::Exception(ss.str());
			}

			// Create a calo hit
			CaloHit *pNewCaloHit = new CaloHit(elt, caloHitPosition, caloHitCell, threshold);
			pNewCaloHit->assignBoxSet(m_pEveCaloHitList);

			m_currentCaloHitList.insert(pNewCaloHit);
			m_currentOrderedCaloHitList[pNewCaloHit->getCell().m_layer].insert(pNewCaloHit);
			
			m_pEveCaloHitList->AddBox(posX, posY, posZ, caloHitCell.m_cellSize0*0.8, caloHitCell.m_cellSize1*0.8, layerThickness-absorberThickness);
			unsigned int color = this->getCaloHitColor(pNewCaloHit);
			pNewCaloHit->setColor(color);
			pNewCaloHit->setTransparency(0);
		}
}

//-------------------------------------------------------------------------------------------

unsigned int EventManager::getCaloHitColor(CaloHit *pCaloHit)
{
	switch(m_caloHitColorDisplay)
	{
		case GRAY_UNIFORM:
		{
			return kGray+1;
		}
		case SEMI_DIGITAL_THRESHOLD:
		{
			if(THRESHOLD_1 == pCaloHit->getSemiDigitalThreshold())
				return kGreen;
			if(THRESHOLD_2 == pCaloHit->getSemiDigitalThreshold())
				return kBlue;
			if(THRESHOLD_3 == pCaloHit->getSemiDigitalThreshold())
				return kRed;
		}
		default:
			return kGray+1;
	}
}

//-------------------------------------------------------------------------------------------

void EventManager::clear()
{
	for(CaloHitList::iterator iter = m_currentCaloHitList.begin() , endIter = m_currentCaloHitList.end() ; endIter != iter ; ++iter)
	{
		CaloHit *pCaloHit = *iter;
		delete pCaloHit;
	}

	m_pEveCaloHitList->RemoveElements();

	m_currentCaloHitList.clear();
	m_currentOrderedCaloHitList.clear();
}



void EventManager::caloHitsAvailable()
{
	Emit("caloHitsAvailable()");
}

} 

