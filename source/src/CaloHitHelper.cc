/*
 *
 * CaloHitHelper.cc source template automatically generated by a class generator
 * Creation date : lun. mai 5 2014
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
 * @author remi
 * @copyright CNRS , IPNL
 */


#include "CaloHitHelper.h"

#include "OrderedCaloHitList.h"
#include "CaloHit.h"

namespace sdhcal
{

void CaloHitHelper::CalculateCaloHitProperties(const OrderedCaloHitList &orderedCaloHitList)
{
	for(OrderedCaloHitList::const_iterator layerIter = orderedCaloHitList.begin() , layerEndIter = orderedCaloHitList.end() ; layerEndIter != layerIter ; ++layerIter)
	{
		for(CaloHitList::iterator iter = layerIter->second.begin() , endIter = layerIter->second.end() ; endIter != iter ; ++iter)
		{
			CaloHit *pCaloHit = *iter;
			pCaloHit->m_density2D = CaloHitHelper::CalculateCaloHitDensity2D(pCaloHit, layerIter->second);
			pCaloHit->m_density3D = CaloHitHelper::CalculateCaloHitDensity3D(pCaloHit, orderedCaloHitList);
		}
	}
}


float CaloHitHelper::CalculateCaloHitDensity2D(CaloHit *pCaloHit, const CaloHitList &caloHitList)
{
	CaloHitCell caloHitCell = pCaloHit->getCell();
	float density2D = 0.f;

	for(CaloHitList::const_iterator iter = caloHitList.begin() , endIter = caloHitList.end() ; endIter != iter ; ++iter)
	{
		CaloHit *pOtherCaloHit = *iter;
		CaloHitCell otherCaloHitCell = pOtherCaloHit->getCell();

		if(abs(static_cast<int>(caloHitCell.m_iCell)-static_cast<int>(otherCaloHitCell.m_iCell)) < 2
		&& abs(static_cast<int>(caloHitCell.m_jCell)-static_cast<int>(otherCaloHitCell.m_jCell)) < 2)
		{
			density2D += static_cast<float>(pOtherCaloHit->getSemiDigitalThreshold());
		}
	}
	density2D /= 9.f;

	return density2D;
}



float CaloHitHelper::CalculateCaloHitDensity3D(CaloHit *pCaloHit, const OrderedCaloHitList &orderedCaloHitList)
{
	const unsigned int layer = pCaloHit->getCell().m_layer;
	float density3D = 0.f;

	if(layer != 0)
	{
		OrderedCaloHitList::const_iterator previousLayerIter = orderedCaloHitList.find(layer-1);

		if(orderedCaloHitList.end() != previousLayerIter)
		{
			density3D += CaloHitHelper::CalculateCaloHitDensity2D(pCaloHit, previousLayerIter->second);
		}
	}

	OrderedCaloHitList::const_iterator nextLayerIter = orderedCaloHitList.find(layer+1);

	if(orderedCaloHitList.end() != nextLayerIter)
	{
		density3D += CaloHitHelper::CalculateCaloHitDensity2D(pCaloHit, nextLayerIter->second);
	}

	density3D += pCaloHit->m_density2D;

	density3D /= 3.f;

	return density3D;
}

} 

