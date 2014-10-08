/*
 *
 * EventNavigator.cc source template automatically generated by a class generator
 * Creation date : sam. avr. 26 2014
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


#include "EventNavigator.h"

// root
#include "TGFrame.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TSystem.h"
#include "TGSplitter.h"
#include "TGFileDialog.h"
#include "TGComboBox.h"
#include "TTimer.h"
#include "TEveManager.h"
#include "TGLViewer.h"
#include "TGLAutoRotator.h"

// lcio
#include "EVENT/LCEvent.h"
#include "IO/LCReader.h"
#include "IOIMPL/LCFactory.h"

// streamlog
#include "streamlog/streamlog.h"

// std
#include <cstdlib>
#include <algorithm>

ClassImpQ(sdhcal::EventNavigator)

namespace sdhcal
{

EventNavigator::EventNavigator(TGCompositeFrame *pMainFrame)
{
 m_pLCReader = IOIMPL::LCFactory::getInstance()->createLCReader(1);
 m_pCurrentEvent = NULL;
 m_currentFileName = "";
 m_currentRunNumber = 0;
 m_currentEventNumber = 0;

	m_pEventNavigatorGUI = new EventNavigatorGUI(this, pMainFrame);

	m_timer.Connect("Timeout()", "sdhcal::EventNavigator", this, "goToNextEvent()");
}

//-------------------------------------------------------------------------------------------

EventNavigator::~EventNavigator() 
{
	// gui
	delete m_pEventNavigatorGUI;

	// reader
	m_pLCReader->close();
	delete m_pLCReader;
}

//-------------------------------------------------------------------------------------------

void EventNavigator::goToNextEvent()
{
	if(m_currentFileName.empty())
	{
		streamlog_out(WARNING) << "No lcio file loaded for the moment. Can't load next event !" << std::endl;
		return;
	}

	if(NULL == m_pCurrentEvent)
	{
		streamlog_out(WARNING) << "No event loaded. Couldn't load next event !" << std::endl;
		return;
	}

	m_currentEventIterator++;

	if(m_runToEventCollectionMap.find(m_currentRunNumber)->second.end() == m_currentEventIterator)
	{
		streamlog_out(WARNING) << "Last event reached for this run. Can't load next event !" << std::endl;
		m_currentEventIterator--;
		return;
	}

	std::stringstream eventNumberStream;
	eventNumberStream << *m_currentEventIterator;
	int entryId = m_pEventNavigatorGUI->m_pEventNumberComboBox->FindEntry(eventNumberStream.str().c_str())->EntryId();
	m_pEventNavigatorGUI->m_pEventNumberComboBox->Select(entryId);



//	m_pEventNavigatorGUI->m_pEventNumberEntry->SetNumber(m_currentEventNumber+1);
//	m_pEventNavigatorGUI->m_pEventNumberEntry->Emit("ValueSet(Long_t)", m_currentEventNumber+1);
}

//-------------------------------------------------------------------------------------------

void EventNavigator::goToPreviousEvent()
{
	if(m_currentFileName.empty())
	{
		streamlog_out(WARNING) << "No lcio file loaded for the moment. Can't load previous event !" << std::endl;
		return;
	}

	if(NULL == m_pCurrentEvent)
	{
		streamlog_out(WARNING) << "No event loaded. Couldn't load previous event !" << std::endl;
		return;
	}

	if(m_runToEventCollectionMap.find(m_currentRunNumber)->second.begin() == m_currentEventIterator)
	{
		streamlog_out(WARNING) << "First event reached for this run. Can't load previous event !" << std::endl;
		return;
	}

	m_currentEventIterator--;

	std::stringstream eventNumberStream;
	eventNumberStream << *m_currentEventIterator;
	int entryId = m_pEventNavigatorGUI->m_pEventNumberComboBox->FindEntry(eventNumberStream.str().c_str())->EntryId();
	m_pEventNavigatorGUI->m_pEventNumberComboBox->Select(entryId);
}

//-------------------------------------------------------------------------------------------

void EventNavigator::loadLCIOFile(const std::string &fileName)
{
	if(!m_currentFileName.empty())
	{
		try
		{
			streamlog_out(MESSAGE) << "Closing lcio file : " << m_currentFileName << std::endl;
			m_pLCReader->close();
			delete m_pLCReader;
			m_pLCReader = IOIMPL::LCFactory::getInstance()->createLCReader(1);
			m_currentFileName = "";
		}
		catch(EVENT::Exception &e)
		{
			streamlog_out(WARNING) << "Couldn't close lcio file '" << m_currentFileName << "'. Message : " << e.what() << std::endl;
			m_currentFileName = "";
			return;
		}
	}

	try
	{
	 m_pLCReader->open(fileName);
	 m_currentFileName = fileName;
	 streamlog_out(MESSAGE) << "LCIO file loaded successfully ! (" << m_currentFileName << ")" << std::endl;
	 this->loadRunToEventCollectionMap(m_pLCReader);
	}
	catch(EVENT::Exception &e)
	{
		streamlog_out(WARNING) << "Couldn't open lcio file '" << fileName << "'. Message : " << e.what() << std::endl;
		m_currentFileName = "";
		return;
	}

}

//-------------------------------------------------------------------------------------------

void EventNavigator::goToEvent()
{
	if(0 == m_pEventNavigatorGUI->m_pEventNumberComboBox->GetNumberOfEntries())
	{
		m_currentEventNumber = 0;
		return;
	}

	TGTextLBEntry *textEntry = (TGTextLBEntry*) m_pEventNavigatorGUI->m_pEventNumberComboBox->GetSelectedEntry();

	if(NULL == textEntry)
	{
		m_currentEventNumber = 0;
		return;
	}

	const char *eventNumberStr = textEntry->GetText()->GetString();

	if(NULL == eventNumberStr)
	{
		m_currentEventNumber = 0;
	}
	else
	{
		std::string eventNumberString(eventNumberStr);

		if(!eventNumberString.empty())
			m_currentEventNumber = atoi(eventNumberStr);
		else
			m_currentEventNumber = 0;
	}

	m_currentEventIterator = std::find(m_runToEventCollectionMap.find(m_currentRunNumber)->second.begin(),
			m_runToEventCollectionMap.find(m_currentRunNumber)->second.end(), m_currentEventNumber);

	loadEvent();
}

//-------------------------------------------------------------------------------------------

void EventNavigator::loadEvent()
{
	if(m_currentFileName.empty())
	{
		streamlog_out(WARNING) << "No lcio file loaded for the moment. Can't load event !" << std::endl;
		return;
	}

	try
	{
		EVENT::LCEvent *pLCEvent = NULL;
		pLCEvent = m_pLCReader->readEvent(m_currentRunNumber, m_currentEventNumber);

		if(NULL == pLCEvent)
			throw EVENT::Exception("Can't read event from current lcio file");

		m_pCurrentEvent = pLCEvent;

		this->eventUpdated();
		streamlog_out(MESSAGE) << "Event nb " << m_currentEventNumber << " successfully loaded !" << std::endl;
	}
	catch(EVENT::Exception &e)
	{
		streamlog_out(WARNING) << "Couldn't load event no " << m_currentEventNumber << ", run no " << m_currentRunNumber << ". Message : " << e.what() << std::endl;
		return;
	}
}

//-------------------------------------------------------------------------------------------

void EventNavigator::goToRun()
{
	if(0 == m_pEventNavigatorGUI->m_pRunNumberComboBox->GetNumberOfEntries())
	{
		m_currentRunNumber = 0;
		return;
	}

	TGTextLBEntry *textEntry = (TGTextLBEntry*) m_pEventNavigatorGUI->m_pRunNumberComboBox->GetSelectedEntry();

	if(NULL == textEntry)
	{
		m_currentRunNumber = 0;
		return;
	}

	const char *runNumberStr = textEntry->GetText()->GetString();

	if(NULL == runNumberStr)
	{
		m_currentRunNumber = 0;
	}
	else
	{
		std::string runNumberString(runNumberStr);

		if(!runNumberString.empty())
			m_currentRunNumber = atoi(runNumberStr);
		else
			m_currentRunNumber = 0;
	}

	m_pEventNavigatorGUI->loadEventList(m_currentRunNumber);
}

//-------------------------------------------------------------------------------------------

void EventNavigator::loadFileDialog()
{
 const char *pLcioFileType[] =
 {"LCIO files", "*.slcio",
  "All files" , "*",
  0           , 0 };

	TGFileInfo fileInfo;
	fileInfo.fFileTypes = pLcioFileType;
	fileInfo.fIniDir = StrDup(".");

 new TGFileDialog(gClient->GetRoot(), m_pEventNavigatorGUI->m_pNavigatorFrame, kFDOpen, &fileInfo);
 const char *lcioStr = fileInfo.fFilename;

 if(NULL != lcioStr)
 {
 	std::string lcioFileName = fileInfo.fFilename;
 	this->loadLCIOFile(lcioFileName);
 }
}

//-------------------------------------------------------------------------------------------

void EventNavigator::eventUpdated()
{
	Emit("eventUpdated()");
}

//-------------------------------------------------------------------------------------------

void EventNavigator::loadRunToEventCollectionMap(IO::LCReader *pLCReader)
{
 m_runToEventCollectionMap.clear();

	std::vector<int> eventNumberList;
	pLCReader->getEvents(eventNumberList);

	bool readRunOrEvent = true; // true for read run, false for read event
	int currentRunNumber = 0;

	for(std::vector<int>::iterator iter = eventNumberList.begin() , endIter = eventNumberList.end() ; endIter != iter ; ++iter)
	{
		if(readRunOrEvent)
		{
			// this creates the entry internally
			m_runToEventCollectionMap[*iter];
			currentRunNumber = *iter;
			readRunOrEvent = false;
		}
		else
		{
			m_runToEventCollectionMap[currentRunNumber].insert(*iter);
			readRunOrEvent = true;
		}
	}

 m_pEventNavigatorGUI->loadAvailableRunNumbers();

	m_currentRunNumber = m_runToEventCollectionMap.begin()->first;
	m_currentEventIterator = m_runToEventCollectionMap.begin()->second.begin();
	m_currentEventNumber = *m_currentEventIterator;

	m_pEventNavigatorGUI->loadEventList(m_currentRunNumber);
}

//-------------------------------------------------------------------------------------------

void EventNavigator::startTimer(unsigned int seconds)
{
	m_timer.Stop();
	m_timer.Start(seconds*1000, kFALSE);

	TGLViewer *pGLViewer = gEve->GetDefaultGLViewer();
	pGLViewer->GetAutoRotator()->Start();
}

//-------------------------------------------------------------------------------------------

void EventNavigator::stopTimer()
{
	m_timer.Stop();

	TGLViewer *pGLViewer = gEve->GetDefaultGLViewer();
	pGLViewer->GetAutoRotator()->Stop();
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

EventNavigatorGUI::EventNavigatorGUI(EventNavigator *pEventNavigator, TGCompositeFrame *pMainFrame)
{
	m_pEventNavigator = pEventNavigator;

	// The main frame of the navigator
	m_pNavigatorFrame = new TGGroupFrame(pMainFrame, "Event Navigation", kHorizontalFrame);
	pMainFrame->AddFrame(m_pNavigatorFrame, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 4));

 // the frame for all buttons
	m_pButtonsFrame = new TGVerticalFrame(m_pNavigatorFrame);
	m_pNavigatorFrame->AddFrame(m_pButtonsFrame, new TGLayoutHints(kLHintsBottom));

	// the frame for next and previous buttons
	m_pNextAndPreviousButtonsFrame = new TGHorizontalFrame(m_pButtonsFrame);
	m_pButtonsFrame->AddFrame(m_pNextAndPreviousButtonsFrame, new TGLayoutHints(kLHintsCenterX));

	TString icondir( Form("%s/icons/", gSystem->Getenv("ROOTSYS")));

	// Previous button
	m_pPreviousEventButton = new TGPictureButton(m_pNextAndPreviousButtonsFrame, gClient->GetPicture(icondir + "GoBack.gif"));
	m_pNextAndPreviousButtonsFrame->AddFrame(m_pPreviousEventButton, new TGLayoutHints(kLHintsNormal, 0, 6, 0, 0));
	m_pPreviousEventButton->Connect("Clicked()", "sdhcal::EventNavigator", m_pEventNavigator, "goToPreviousEvent()");

	// Next button
	m_pNextEventButton = new TGPictureButton(m_pNextAndPreviousButtonsFrame, gClient->GetPicture(icondir + "GoForward.gif"));
	m_pNextAndPreviousButtonsFrame->AddFrame(m_pNextEventButton);
	m_pNextEventButton->Connect("Clicked()", "sdhcal::EventNavigator", m_pEventNavigator, "goToNextEvent()");

	// Load lcio file button
	m_pLoadFileButton = new TGTextButton(m_pButtonsFrame, "&Open LCIO file");
	m_pButtonsFrame->AddFrame(m_pLoadFileButton, new TGLayoutHints(kLHintsCenterY ,0 ,0, 8, 0));
	m_pLoadFileButton->Connect("Clicked()", "sdhcal::EventNavigator", m_pEventNavigator, "loadFileDialog()");

	// frame for event and run numbers
	m_pEventNumberFrame = new TGVerticalFrame(m_pNavigatorFrame);

	const int padShift = 15;

	// run number gui
	m_pRunNumberComboBox = new TGComboBox(m_pEventNumberFrame);
		m_pGoToRunLabel = new TGLabel(m_pEventNumberFrame, "Go to run");
	m_pEventNumberFrame->AddFrame(m_pGoToRunLabel, new TGLayoutHints(kLHintsCenterX ,padShift));
	m_pEventNumberFrame->AddFrame(m_pRunNumberComboBox);
	m_pRunNumberComboBox->Resize(100, 20);
	m_pRunNumberComboBox->Connect("Selected(Int_t)", "sdhcal::EventNavigator", m_pEventNavigator, "goToRun()");

	// event number gui
	m_pGoToEventLabel = new TGLabel(m_pEventNumberFrame, "Go to event");
	m_pEventNumberComboBox = new TGComboBox(m_pEventNumberFrame);
	m_pEventNumberFrame->AddFrame(m_pGoToEventLabel, new TGLayoutHints(kLHintsCenterX ,padShift));
	m_pEventNumberFrame->AddFrame(m_pEventNumberComboBox);
	m_pEventNumberComboBox->Resize(100, 20);
	m_pEventNumberComboBox->Connect("Selected(Int_t)", "sdhcal::EventNavigator", m_pEventNavigator, "goToEvent()");

	m_pNavigatorFrame->AddFrame(m_pEventNumberFrame, new TGLayoutHints(kLHintsNormal, 6));
	m_pNavigatorFrame->DrawBorder();

//	m_pAutomaticDisplayGroupFrame = new TGGroupFrame(pMainFrame, "Automatic display", kHorizontalFrame);
//	pMainFrame->AddFrame(m_pAutomaticDisplayGroupFrame);
//
//	m_pAutomaticDisplayFrame = new TGHorizontalFrame(m_pAutomaticDisplayGroupFrame);
//	m_pAutomaticDisplayGroupFrame->AddFrame(m_pAutomaticDisplayFrame, new TGLayoutHints(kLHintsCenterX,0 ,0, 10, 0));
//
//	m_pEventLatencyLabel = new TGLabel(m_pAutomaticDisplayFrame, "Event latency (ms) ");
//	m_pAutomaticDisplayFrame->AddFrame(m_pEventLatencyLabel, new TGLayoutHints(kLHintsCenterX));
//
//	m_pEventLatencyNumberEntry = new TGNumberEntry(m_pAutomaticDisplayFrame, 1, 5, -1, TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive ,TGNumberFormat::kNELLimitMinMax ,1 ,1000);
//	m_pAutomaticDisplayFrame->AddFrame(m_pEventLatencyNumberEntry, new TGLayoutHints(kLHintsCenterX));
//
//	m_pStartAutomaticDisplayButton = new TGTextButton(m_pAutomaticDisplayFrame, "&Start");
//	m_pAutomaticDisplayFrame->AddFrame(m_pStartAutomaticDisplayButton, new TGLayoutHints(kLHintsCenterX, 4));

}

//-------------------------------------------------------------------------------------------

EventNavigatorGUI::~EventNavigatorGUI()
{
	delete m_pNavigatorFrame;
	delete m_pNextAndPreviousButtonsFrame;
	delete m_pButtonsFrame;
	delete m_pPreviousEventButton;
	delete m_pNextEventButton;
	delete m_pLoadFileButton;
	delete m_pEventNumberFrame;
	delete m_pGoToEventLabel;
	delete m_pEventNumberComboBox;
	delete m_pGoToRunLabel;
	delete m_pRunNumberComboBox;
}

//-------------------------------------------------------------------------------------------

void EventNavigatorGUI::loadAvailableRunNumbers()
{
	m_pRunNumberComboBox->RemoveAll();

	if(m_pEventNavigator->m_runToEventCollectionMap.empty())
		return;

	int runNumberEntryId = 0;
	bool isFirst = true;

	for(EventNavigator::RunToEventListMap::iterator iter = m_pEventNavigator->m_runToEventCollectionMap.begin() , endIter = m_pEventNavigator->m_runToEventCollectionMap.end() ; endIter != iter ; ++iter)
	{
		std::stringstream runNumberStream;
		int runNumber = iter->first;
		runNumberStream << runNumber;
		m_pRunNumberComboBox->AddEntry(runNumberStream.str().c_str(), runNumberEntryId);

		if(isFirst)
		{
			streamlog_out(MESSAGE) << "Select first run entry" << std::endl;
			m_pRunNumberComboBox->Select(runNumberEntryId, false);
			isFirst = false;
		}

		runNumberEntryId++;
	}
}

//-------------------------------------------------------------------------------------------

void EventNavigatorGUI::loadEventList(int runNumber)
{
	EventNavigator::RunToEventListMap::iterator findIter = m_pEventNavigator->m_runToEventCollectionMap.find(runNumber);

	if(m_pEventNavigator->m_runToEventCollectionMap.end() == findIter)
		return;

	m_pEventNumberComboBox->RemoveAll();

	int eventNumberEntryId = 0;
	bool isFirst = true;

	for(std::set<int>::iterator iter = findIter->second.begin() , endIter = findIter->second.end() ; endIter != iter ; ++iter)
	{
		std::stringstream eventNumberStream;
		int eventNumber = *iter;
		eventNumberStream << eventNumber;
		m_pEventNumberComboBox->AddEntry(eventNumberStream.str().c_str(), eventNumberEntryId);

		if(isFirst)
		{
			m_pEventNumberComboBox->Select(eventNumberEntryId, false);
			isFirst = false;
		}

		eventNumberEntryId++;
	}

	m_pEventNavigator->goToEvent();
}

} 

