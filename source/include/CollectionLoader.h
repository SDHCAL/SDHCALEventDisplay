  /// \file CollectionLoader.h
/*
 *
 * CollectionLoader.h header template automatically generated by a class generator
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


#ifndef COLLECTIONLOADER_H
#define COLLECTIONLOADER_H


namespace sdhcal
{

/** 
 * @brief CollectionLoader class
 */ 
class CollectionLoader 
{
 public:

	/**
	 *
	 */
	virtual void loadCollection(EVENT::LCEvent *pLCEvent, const std::string &collectionName) = 0;

	/**
	 *
	 */
	virtual const std::string &getCompatibleCollectionType() = 0;

}; 

} 

#endif  //  COLLECTIONLOADER_H
