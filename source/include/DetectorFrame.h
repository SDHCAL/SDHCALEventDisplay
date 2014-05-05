/*
 *
 * DetectorFrame.h header template automatically generated by a class generator
 * Creation date : sam. mai 3 2014
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


#ifndef DETECTORFRAME_H
#define DETECTORFRAME_H

#include "Rtypes.h"
#include "RQ_OBJECT.h"


class TGMainFrame;
class TGGroupFrame;
class TGButtonGroup;
class TGCheckButton;
class TGHorizontalFrame;
class TGVerticalFrame;
class TGHButtonGroup;
class TGRadioButton;
class TGNumberEntry;
class TGLabel;

namespace sdhcal
{

class SDHCAL;

/** 
 * @brief DetectorFrame class
 */ 
class DetectorFrame 
{
	RQ_OBJECT("sdhcal::DetectorFrame");

 public:

 /**
  * @brief Ctor
  */
 DetectorFrame(TGMainFrame *pMainFrame);

 /**
  * @brief Dtor 
  */
 virtual ~DetectorFrame();

 /**
  *
  */
 void enableShowPartiallyDetectorFrame(Bool_t on);

 /**
  *
  */
 void showDetector(Bool_t on);

 /**
  *
  */
 void showPartiallyDetector(Bool_t on);

 /**
  *
  */
 void updateGeometry();

protected:

 SDHCAL                  *m_pSDHCAL;

 TGGroupFrame            *m_pDetectorFrame;
 TGCheckButton           *m_pShowDetectorButton;
 TGCheckButton           *m_pShowPartialButton;
	TGVerticalFrame         *m_pShowPartialDetectorFrame;
	TGHButtonGroup          *m_pShowHideButtonGroup;
	TGRadioButton           *m_pShowButton;
	TGRadioButton           *m_pHideButton;
	TGHorizontalFrame       *m_pFromToLayerFrame;
	TGLabel                 *m_pFromLayerLabel;
	TGNumberEntry           *m_pFromLayerEntry;
	TGLabel                 *m_pToLayerLabel;
	TGNumberEntry           *m_pToLayerEntry;

 ClassDef(DetectorFrame, 0);
}; 

} 

#endif  //  DETECTORFRAME_H
