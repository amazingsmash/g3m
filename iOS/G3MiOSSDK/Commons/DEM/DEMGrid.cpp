//
//  DEMGrid.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 10/5/16.
//
//

#include "DEMGrid.hpp"
#include "Projection.hpp"
#include "ILogger.hpp"


DEMGrid::DEMGrid(const Sector&   sector,
                 const Vector2I& extent) :
_sector(sector),
_extent(extent),
_resolution(sector._deltaLatitude.div(extent._y),
            sector._deltaLongitude.div(extent._x))
{
}

DEMGrid::~DEMGrid() {
#ifdef JAVA_CODE
  super.dispose();
#endif
}

const Sector DEMGrid::getSector() const {
  return _sector;
}

const Vector2I DEMGrid::getExtent() const {
  return _extent;
}

const Geodetic2D DEMGrid::getResolution() const {
  return _resolution;
}

Geodetic2D DEMGrid::getInnerPoint(int x, int y) const{
  
  const int mrx = getExtent()._x;
  const int mry = getExtent()._y;
  
  const double v = (double) y / (mry - 1);
  const double u = (double) x / (mrx - 1);
  
  Sector s = getSector();
  
  const Angle lat = getProjection()->getInnerPointLatitude(s, v);
  const Angle lon = getProjection()->getInnerPointLongitude(s, u);
  
//#warning REMOVE TEST
//  if (getSector().contains(lat, lon)){
//    ILogger::instance()->logError("Logic Error " + getSector().description() + " P: " + Geodetic2D(lat, lon).description());
//  }
  
  return Geodetic2D(lat, lon);
}
