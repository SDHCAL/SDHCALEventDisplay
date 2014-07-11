/*
 *
 * LcioCollectionFrame.cc source template automatically generated by a class generator
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


#include "LcioCollectionFrame.h"

// root
#include "TGFrame.h"
#include "TGLabel.h"
#include "TGTextEntry.h"
#include "TGComboBox.h"

// streamlog
#include "streamlog/streamlog.h"

// sdhcal
#include "Gui.h"
#include "EventNavigator.h"
#include "EventNavigator.h"

// lcio
#include "EVENT/LCEvent.h"
#include "EVENT/LCCollection.h"
#include "EVENT/LCIO.h"

ClassImpQ(sdhcal::LcioCollectionFrame);

namespace sdhcal
{

LcioCollectionFrame::LcioCollectionFrame(TGCompositeFrame *pMainFrame)
{
	m_pLcioCollectionFrame = new TGGroupFrame(pMainFrame, "Lcio Collections", kHorizontalFrame);
	pMainFrame->AddFrame(m_pLcioCollectionFrame, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 4));

	// sdhcal collection entry
	m_pSDHCALCollectionFrame = new TGHorizontalFrame(m_pLcioCollectionFrame);
	m_pLcioCollectionFrame->AddFrame(m_pSDHCALCollectionFrame);

	m_pSDHCALCollectionLabel = new TGLabel(m_pSDHCALCollectionFrame, "SDHCAL collection");
	m_pSDHCALCollectionLabel->SetWidth(30);
	m_pSDHCALCollectionFrame->AddFrame(m_pSDHCALCollectionLabel, new TGLayoutHints(kLHintsCenterX, 0, 0, 8));

//	m_pSDHCALCollectionEntry = new TGTextEntry(m_pSDHCALCollectionFrame);
//	m_pSDHCALCollectionEntry->SetText("HCALBarrel");
//	m_pSDHCALCollectionFrame->AddFrame(m_pSDHCALCollectionEntry, new TGLayoutHints(kLHintsCenterX, 10, 0, 5));

	m_pSDHCALCollectionComboBox = new TGComboBox(m_pSDHCALCollectionFrame);
	m_pSDHCALCollectionComboBox->Resize(100, 20);
	m_pSDHCALCollectionFrame->AddFrame(m_pSDHCALCollectionComboBox, new TGLayoutHints(kLHintsCenterX, 10, 0, 5));

	EventNavigator *pEventNavigator = Gui::getInstance()->getEventNavigator();
	pEventNavigator->Connect("eventUpdated()", "sdhcal::LcioCollectionFrame", this, "loadCollectionList()");
	pEventNavigator->HighPriority("eventUpdated()", "loadCollectionList()");
}

//-------------------------------------------------------------------------------------------

LcioCollectionFrame::~LcioCollectionFrame()
{

}


std::string LcioCollectionFrame::getSDHCALCollectionName() const
{
	if(0 == m_pSDHCALCollectionComboBox->GetNumberOfEntries())
	{
		streamlog_out(WARNING) << "No CalorimeteHit collection available" << std::endl;
		return std::string("");
	}

	TGTextLBEntry *textEntry = (TGTextLBEntry*) m_pSDHCALCollectionComboBox->GetSelectedEntry();

	if(NULL == textEntry)
		return std::string("");

	const char *collectionName = textEntry->GetText()->GetString();

	if(NULL != collectionName)
		return std::string(collectionName);
	else
		return std::string("");

//
// OLD CODE
//
//	const char *text = m_pSDHCALCollectionEntry->GetText();
//
//	if(!text)
//		return std::string("");
//	else
//	 return std::string(text);
}


void LcioCollectionFrame::loadCollectionList()
{
	m_pSDHCALCollectionComboBox->RemoveAll();

	EVENT::LCEvent *pLCEvent = Gui::getInstance()->getEventNavigator()->getCurrentEvent();

	if(NULL == pLCEvent)
		return;

	const std::vector<std::string> *collectionNames = pLCEvent->getCollectionNames();
	int collectionID = 0;

	for(std::vector<std::string>::const_iterator iter = collectionNames->begin(), endIter = collectionNames->end() ; endIter != iter ; ++iter)
	{
		EVENT::LCCollection *pCurrentCollection = pLCEvent->getCollection(*iter);

		if(EVENT::LCIO::CALORIMETERHIT == pCurrentCollection->getTypeName()
				|| EVENT::LCIO::SIMCALORIMETERHIT == pCurrentCollection->getTypeName())
		{
			m_pSDHCALCollectionComboBox->AddEntry((*iter).c_str(), collectionID);
			collectionID++;
		}
	}

	if(0 != m_pSDHCALCollectionComboBox->GetNumberOfEntries())
		m_pSDHCALCollectionComboBox->Select(0);
}

} 

