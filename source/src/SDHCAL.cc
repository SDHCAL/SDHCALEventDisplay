/*
 *
 * SDHCAL.cc source template automatically generated by a class generator
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


#include "SDHCAL.h"

#include "streamlog/streamlog.h"

#include "Config.h"

#include "Exceptions.h"

#include "gear/CalorimeterParameters.h"
#include "gear/LayerLayout.h"
#include "GearMgr.h"

#include "TGeoManager.h"
#include "TGeoMedium.h"
#include "TGeoMaterial.h"
#include "TGeoVolume.h"

namespace sdhcal
{

//-------------------------------------------------------------------------------------------

Layer::Layer(unsigned int layerNumber, TGeoManager *pGeoManager)
 : m_layerNumber(layerNumber),
  	m_color(kWhite),
  	m_transparency(0),
  	m_pMaterial(NULL),
  	m_pMedium(NULL),
  	m_pBottomBorder(NULL),
  	m_pTopBorder(NULL),
  	m_pRightBorder(NULL),
  	m_pLeftBorder(NULL)
{
	// iron
	m_pMaterial = new TGeoMaterial("Fe", 55.845, 26, 7.87);
	m_pMedium = new TGeoMedium("Iron", 1, m_pMaterial);

	const float frameThickness = 1.f;
	const float frameLenght = 500.f;

 m_pBottomBorder = pGeoManager->MakeBox("BottomBorder", m_pMedium, frameThickness, frameLenght, frameThickness);
 m_pTopBorder    = pGeoManager->MakeBox("TopBorder",    m_pMedium, frameThickness, frameLenght, frameThickness);
 m_pRightBorder  = pGeoManager->MakeBox("RightBorder",  m_pMedium, frameLenght, frameThickness, frameThickness);
 m_pLeftBorder   = pGeoManager->MakeBox("LeftBorder",   m_pMedium, frameLenght, frameThickness, frameThickness);

 this->setTransparency(70);
}

//-------------------------------------------------------------------------------------------

Layer::~Layer()
{

}

//-------------------------------------------------------------------------------------------

void Layer::setColor(unsigned int color)
{
	m_color = color;
	m_pBottomBorder->SetFillColor(color);
	m_pTopBorder->SetFillColor(color);
	m_pRightBorder->SetFillColor(color);
	m_pLeftBorder->SetFillColor(color);

	m_pBottomBorder->SetLineColor(color);
	m_pTopBorder->SetLineColor(color);
	m_pRightBorder->SetLineColor(color);
	m_pLeftBorder->SetLineColor(color);
}

//-------------------------------------------------------------------------------------------

void Layer::setTransparency(unsigned int transparency)
{
	m_transparency = transparency;
	m_pBottomBorder->SetTransparency(transparency);
	m_pTopBorder->SetTransparency(transparency);
	m_pRightBorder->SetTransparency(transparency);
	m_pLeftBorder->SetTransparency(transparency);
}

//-------------------------------------------------------------------------------------------

void Layer::hide()
{
	m_pBottomBorder->SetTransparency(100);
	m_pTopBorder->SetTransparency(100);
	m_pRightBorder->SetTransparency(100);
	m_pLeftBorder->SetTransparency(100);
}

//-------------------------------------------------------------------------------------------

void Layer::show()
{
	m_pBottomBorder->SetTransparency(m_transparency);
	m_pTopBorder->SetTransparency(m_transparency);
	m_pRightBorder->SetTransparency(m_transparency);
	m_pLeftBorder->SetTransparency(m_transparency);
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

SDHCAL::SDHCAL(TGeoManager *pGeoManager)
 : m_alreadyBuilt(false),
   m_pGeoManager(pGeoManager),
   m_globalColor(0),
   m_globalTransparency(0),
   m_numberOfLayers(48)
{

}

//-------------------------------------------------------------------------------------------

SDHCAL::~SDHCAL() 
{
	for(std::map<unsigned int, Layer*>::iterator iter = m_layerMap.begin() , endIter = m_layerMap.end() ; endIter != iter ; ++iter)
	{
		Layer *pLayer = iter->second;
		delete pLayer;
	}

	m_layerMap.clear();
}

//-------------------------------------------------------------------------------------------

void SDHCAL::build()
{
	if(m_alreadyBuilt)
		return;

	streamlog_out(MESSAGE) << "Building SDHCAL geometry" << std::endl;

	Config *pConfig = Config::getInstance();
	gear::GearMgr *pGearMgr = pConfig->getGearMgr();

	if(NULL == pGearMgr)
		throw EVENT::Exception("Gear mgr not initialized while building SDHCAL geometry");

	const gear::CalorimeterParameters &sdhcalParameters = pGearMgr->getHcalEndcapParameters();
	const gear::LayerLayout &layerLayout = sdhcalParameters.getLayerLayout();
	const unsigned int nLayers(layerLayout.getNLayers());
	unsigned int nCells0 = 96;
	unsigned int nCells1 = 96;
	m_numberOfLayers = layerLayout.getNLayers();

	TGeoVolume *pTopVolume = m_pGeoManager->GetTopVolume();

	for(unsigned int layer=0 ; layer<nLayers ; layer++)
	{
		const float cellSize0 = layerLayout.getCellSize0(layer);
		const float cellSize1 = layerLayout.getCellSize1(layer);
		const float layerThickness = layerLayout.getThickness(layer);
		const float absorberThickness = layerLayout.getAbsorberThickness(layer);
		const float zShift = (layerThickness - absorberThickness)/2.f;
		const float xShift = nCells0*cellSize0/2.0;
		const float yShift = nCells1*cellSize1/2.0;

		Layer *pLayer = new Layer(layer, m_pGeoManager);
		m_layerMap[layer] = pLayer;

		pTopVolume->AddNodeOverlap(pLayer->m_pBottomBorder, layer, new TGeoTranslation(-xShift, 0,      layer*layerThickness + absorberThickness + zShift));
		pTopVolume->AddNodeOverlap(pLayer->m_pTopBorder,    layer, new TGeoTranslation( xShift, 0,      layer*layerThickness + absorberThickness + zShift));
		pTopVolume->AddNodeOverlap(pLayer->m_pRightBorder,  layer, new TGeoTranslation( 0,      yShift, layer*layerThickness + absorberThickness + zShift));
		pTopVolume->AddNodeOverlap(pLayer->m_pLeftBorder,   layer, new TGeoTranslation( 0,     -yShift, layer*layerThickness + absorberThickness + zShift));
	}

	m_alreadyBuilt = true;

	this->show();
}

//-------------------------------------------------------------------------------------------

void SDHCAL::showLayers(unsigned int fromLayer, unsigned int toLayer)
{
	for(std::map<unsigned int, Layer*>::iterator iter = m_layerMap.begin() , endIter = m_layerMap.end() ; endIter != iter ; ++iter)
	{
		const unsigned int layer = iter->first;

		if(layer < fromLayer || layer > toLayer)
			continue;

		iter->second->show();
	}
}

//-------------------------------------------------------------------------------------------

void SDHCAL::hideLayers(unsigned int fromLayer, unsigned int toLayer)
{
	for(std::map<unsigned int, Layer*>::iterator iter = m_layerMap.begin() , endIter = m_layerMap.end() ; endIter != iter ; ++iter)
	{
		const unsigned int layer = iter->first;

		if(layer < fromLayer || layer > toLayer)
			continue;

		iter->second->hide();
	}
}

//-------------------------------------------------------------------------------------------

void SDHCAL::hide()
{
	for(std::map<unsigned int, Layer*>::iterator iter = m_layerMap.begin() , endIter = m_layerMap.end() ; endIter != iter ; ++iter)
	{
		iter->second->hide();
	}
}

//-------------------------------------------------------------------------------------------

void SDHCAL::show()
{
	for(std::map<unsigned int, Layer*>::iterator iter = m_layerMap.begin() , endIter = m_layerMap.end() ; endIter != iter ; ++iter)
	{
		iter->second->show();
	}
}

//-------------------------------------------------------------------------------------------

void SDHCAL::setLayerColor(unsigned int layer, unsigned int color)
{
	std::map<unsigned int, Layer*>::iterator findIter = m_layerMap.find(layer);

	if(m_layerMap.end() == findIter)
		return;

	findIter->second->setColor(color);
}

//-------------------------------------------------------------------------------------------

void SDHCAL::setLayersColor(unsigned int color)
{
	m_globalColor = color;

	for(std::map<unsigned int, Layer*>::iterator iter = m_layerMap.begin() , endIter = m_layerMap.end() ; endIter != iter ; ++iter)
	{
		iter->second->setColor(color);
	}
}

//-------------------------------------------------------------------------------------------

void SDHCAL::setLayerTransparency(unsigned int layer, unsigned int transparency)
{
	std::map<unsigned int, Layer*>::iterator findIter = m_layerMap.find(layer);

	if(m_layerMap.end() == findIter)
		return;

	findIter->second->setTransparency(transparency);
}

//-------------------------------------------------------------------------------------------

void SDHCAL::setLayersTransparency(unsigned int transparency)
{
	m_globalTransparency = transparency;

	for(std::map<unsigned int, Layer*>::iterator iter = m_layerMap.begin() , endIter = m_layerMap.end() ; endIter != iter ; ++iter)
	{
		iter->second->setTransparency(transparency);
	}
}

} 

