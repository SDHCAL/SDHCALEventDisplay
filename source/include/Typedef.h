  /// \file Typedef.h
/*
 *
 * Typedef.h header template automatically generated by a class generator
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


#ifndef TYPEDEF_H
#define TYPEDEF_H

// std
#include <set>
#include <map>

namespace sdhcal
{

	class CaloHit;

	typedef std::set<CaloHit*> CaloHitList;

	/**
	 *
	 */
	enum SemiDigitalThreshold
	{
		THRESHOLD_1 = 1,
		THRESHOLD_2 = 2,
		THRESHOLD_3 = 3
	};

	/**
	 *
	 */
	enum CaloHitColorDisplay
	{
		GRAY_UNIFORM,
		SEMI_DIGITAL_THRESHOLD,
		DENSITY_2D,
		DENSITY_3D
	};

} 

#endif  //  TYPEDEF_H
