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
                 const Vector2I& extent,
                 const Projection* projection) :
_sector(sector),
_extent(extent),
_resolution(sector._deltaLatitude.div(extent._y),
            sector._deltaLongitude.div(extent._x)),
_projection(projection),
_latitudes(new double[extent._y]),
_longitudes(new double[extent._x])
{
  
  const Sector s = getSector();
  const Projection* proj = getProjection();
  for (int x = 0; x < _extent._x; ++x) {
    const double u = (double) x / (extent._x - 1);
    const Angle lon = proj->getInnerPointLongitude(s, u);
    _longitudes[x] = lon._degrees;
  }
  
  for (int y = 0; y < _extent._y; ++y) {
    const double v = (double) y / (extent._y - 1);
    const Angle lat = proj->getInnerPointLatitude(s, v);
    _latitudes[y] = lat._degrees;
  }
  
}

DEMGrid::~DEMGrid() {
#ifdef JAVA_CODE
  super.dispose();
#endif
  
  delete [] _latitudes;
  delete [] _longitudes;
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

const Projection* DEMGrid::getProjection() const{
  return _projection;
}

Geodetic2D DEMGrid::getInnerPoint(int x, int y) const{
  return Geodetic2D::fromDegrees(_latitudes[y], _longitudes[x]);
}
