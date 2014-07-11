  /// \file EventManager.h
/*
 *
 * EventManager.h header template automatically generated by a class generator
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


#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Rtypes.h"
#include "RQ_OBJECT.h"

#include "Typedef.h"
#include "OrderedCaloHitList.h"

class TEveBoxSet;
class TEveCompound;

namespace EVENT
{
 class LCCollection;
}

namespace sdhcal
{

class EventNavigator;
class CaloHitLoader;

/** 
 * @brief EventManager class
 */ 
class EventManager 
{

	RQ_OBJECT("sdhcal::EventManager");

public:

 /**
  * @brief Ctor
  */
 EventManager(EventNavigator *pEventNavigator);

 /**
  * @brief Dtor
  */
 virtual ~EventManager();

 /**
  *
  */
 void loadCurrentEvent();

 /**
  *
  */
 void loadCaloHitCollection(EVENT::LCCollection *pCollection);

 /**
  *
  */
 void loadSimCaloHitCollection(EVENT::LCCollection *pCollection);

 /**
  *
  */
 unsigned int getCaloHitColor(CaloHit *pCaloHit);

 /**
  *
  */
 void clear();

 /**
  *
  */
 OrderedCaloHitList &getCurrentOrderedCaloHitList();

 /**
  *
  */
 CaloHitList &getCurrentCaloHitList();

 /**
  *
  */
 void caloHitsAvailable();  //*SIGNAL*

protected:


 // members
 EventNavigator          *m_pEventNavigator;

 // calo hit related members
 CaloHitLoader           *m_pCaloHitLoader;
 CaloHitList              m_currentCaloHitList;
 OrderedCaloHitList       m_currentOrderedCaloHitList;
// TEveBoxSet              *m_pEveCaloHitList;
 TEveCompound            *m_pEveCaloHitList;

 CaloHitColorDisplay      m_caloHitColorDisplay;

 ClassDef(EventManager,0);
};


inline OrderedCaloHitList &EventManager::getCurrentOrderedCaloHitList()
{
	return m_currentOrderedCaloHitList;
}


inline CaloHitList &EventManager::getCurrentCaloHitList()
{
	return m_currentCaloHitList;
}

} 

#endif  //  EVENTMANAGER_H
