  /// \file CaloHit.h
/*
 *
 * CaloHit.h header template automatically generated by a class generator
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


#ifndef CALOHIT_H
#define CALOHIT_H

#include "Typedef.h"
#include "CartesianVector.h"

#include "TEveBoxSet.h"
#include "TEveBox.h"

namespace sdhcal
{

/**
 *
 */
class CaloHitCell
{
public:

	unsigned int      m_iCell;
	unsigned int      m_jCell;
	unsigned int      m_layer;

	float              m_cellSize0;
	float              m_cellSize1;
	float              m_cellThickness;
};

/** 
 * @brief CaloHit class
 */ 
class CaloHit : public TEveBox
{
 public:

 /**
  * @brief Ctor
  */
 CaloHit(CartesianVector position, CaloHitCell cell, SemiDigitalThreshold semiDigitalThreshold);

 /**
  *
  */
 CaloHit(CaloHit *pCaloHit);

 /**
  * @brief Dtor 
  */
 virtual ~CaloHit();

 /**
  *
  */
 SemiDigitalThreshold getSemiDigitalThreshold() const;

 /**
  *
  */
 const CartesianVector &getPosition() const;

 /**
  *
  */
 const CaloHitCell &getCell() const;

 /**
  *
  */
 float getDensity2D() const;

 /**
  *
  */
 float getDensity3D() const;

 /**
  *
  */
 void setColor(unsigned int color);

 /**
  *
  */
 void setTransparency(unsigned int transparency);

 /**
  *
  */
 void show();

 /**
  *
  */
 void hide();


protected:

 SemiDigitalThreshold     m_semiDigitalThreshold;
 CartesianVector          m_position;
 CaloHitCell              m_cell;
 float                   m_density2D;
 float                   m_density3D;
 unsigned int           m_color;
 unsigned int           m_transparency;

 friend class EventManager;
 friend class CaloHitHelper;
};

//-------------------------------------------------------------------------------------------

inline SemiDigitalThreshold CaloHit::getSemiDigitalThreshold() const
{
	return m_semiDigitalThreshold;
}

//-------------------------------------------------------------------------------------------

inline const CartesianVector &CaloHit::getPosition() const
{
	return m_position;
}

//-------------------------------------------------------------------------------------------

inline const CaloHitCell &CaloHit::getCell() const
{
	return m_cell;
}

//-------------------------------------------------------------------------------------------

inline float CaloHit::getDensity2D() const
{
	return m_density2D;
}

//-------------------------------------------------------------------------------------------

inline float CaloHit::getDensity3D() const
{
	return m_density3D;
}


} 

#endif  //  CALOHIT_H
