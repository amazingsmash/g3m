//
//  WGS84Projection.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/14/16.
//
//

#include "WGS84Projection.hpp"

#include <stddef.h>

#include "Angle.hpp"
#include "IMathUtils.hpp"
#include "Sector.hpp"


WGS84Projection* WGS84Projection::INSTANCE = NULL;


WGS84Projection::WGS84Projection() {
}

WGS84Projection::~WGS84Projection() {
#ifdef JAVA_CODE
  super.dispose();
#endif
}

WGS84Projection* WGS84Projection::instance() {
  if (INSTANCE == NULL) {
    INSTANCE = new WGS84Projection();
  }
  return INSTANCE;
}

const std::string WGS84Projection::getEPSG() const {
  return "EPSG:4326";
}

double WGS84Projection::getU(const Angle& longitude) const {
  return (longitude._radians + PI) / (PI*2);
}

double WGS84Projection::getV(const Angle& latitude) const {
  return (HALF_PI - latitude._radians) / PI;
}

const Angle WGS84Projection::getInnerPointLongitude(double u) const {
  return Angle::fromRadians(IMathUtils::instance()->linearInterpolation(-PI, PI, u));
}

const Angle WGS84Projection::getInnerPointLatitude(double v) const {
  return Angle::fromRadians(IMathUtils::instance()->linearInterpolation(-HALF_PI, HALF_PI, 1.0 - v));
}

const Angle WGS84Projection::getInnerPointLongitude(const Sector& sector,
                                                    double u) const {
  return Angle::linearInterpolation(sector._lower._longitude,
                                    sector._upper._longitude,
                                    u);
}

const Angle WGS84Projection::getInnerPointLatitude(const Sector& sector,
                                                   double v) const {
  return Angle::linearInterpolation(sector._lower._latitude,
                                    sector._upper._latitude,
                                    1.0 - v);
}

Geodetic2D* WGS84Projection::createInnerPoint(const Sector& sector, double u, double v) const{
  return new Geodetic2D(getInnerPointLatitude(sector, v), getInnerPointLongitude(sector, u));
}

Vector2D WGS84Projection::getUV(const Sector& sector, const Geodetic2D& p) const{
  const double v = (sector._upper._latitude._radians - p._latitude._radians) /
  (sector._upper._latitude._radians - sector._lower._latitude._radians );
  
  
  const double u = (sector._upper._longitude._radians - p._longitude._radians) /
  (sector._upper._longitude._radians - sector._lower._longitude._radians );
  
  return Vector2D(u,v);
}
