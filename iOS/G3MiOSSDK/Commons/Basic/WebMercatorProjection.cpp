//
//  WebMercatorProjection.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/14/16.
//
//

#include "WebMercatorProjection.hpp"

#include <stddef.h>

#include "Angle.hpp"
#include "IMathUtils.hpp"
#include "Sector.hpp"


WebMercatorProjection* WebMercatorProjection::INSTANCE = NULL;

const double WebMercatorProjection::_upperLimitRadians = Angle::fromDegrees(85.0511287798)._radians;
const double WebMercatorProjection::_lowerLimitRadians = Angle::fromDegrees(-85.0511287798)._radians;


WebMercatorProjection::WebMercatorProjection() {
}

WebMercatorProjection::~WebMercatorProjection() {
#ifdef JAVA_CODE
  super.dispose();
#endif
}

WebMercatorProjection* WebMercatorProjection::instance() {
  if (INSTANCE == NULL) {
    INSTANCE = new WebMercatorProjection();
  }
  return INSTANCE;
}

const std::string WebMercatorProjection::getEPSG() const {
  return "EPSG:3857";
}

double WebMercatorProjection::getU(const Angle& longitude) const {
  return (longitude._radians + PI) / (PI*2);
}

double WebMercatorProjection::getV(const Angle& latitude) const {
  const double latitudeRadians = latitude._radians;

  if (latitudeRadians >= _upperLimitRadians) {
    return 0;
  }
  if (latitudeRadians <= _lowerLimitRadians) {
    return 1;
  }

  const IMathUtils* mu = IMathUtils::instance();
  const double pi4 = PI * 4;
  const double latSin = SIN(latitudeRadians);
  return 0.5 - ( mu->log( (1.0 + latSin) / (1.0 - latSin) ) / pi4 );
}

const Angle WebMercatorProjection::getInnerPointLongitude(double u) const {
  return Angle::fromRadians(IMathUtils::instance()->linearInterpolation(-PI, PI, u));
}

const Angle WebMercatorProjection::getInnerPointLatitude(double v) const {
  const IMathUtils* mu = IMathUtils::instance();

  const double exp = mu->exp(-2 * PI * (1.0 - v - 0.5));
  const double atan = mu->atan(exp + PI);
  return Angle::fromRadians((PI / 2) - 2 * atan);
}

const Angle WebMercatorProjection::getInnerPointLongitude(const Sector& sector,
                                                          double u) const {
  return Angle::linearInterpolation(sector._lower._longitude,
                                    sector._upper._longitude,
                                    u);
}

const Angle WebMercatorProjection::getInnerPointLatitude(const Sector& sector,
                                                         double v) const {
  const double lowerV = getV(sector._lower._latitude);
  const double upperV = getV(sector._upper._latitude);
  const double sV = lowerV + ((upperV - lowerV) * v);

  return getInnerPointLatitude(sV);
}

Vector2D WebMercatorProjection::getUV(const Sector& sector, const Geodetic2D& p) const{

  const double sinMinSectorLat = SIN(sector._lower._latitude._radians);
  const double sinMaxSectorLat = SIN(sector._upper._latitude._radians);
  const double sinLat = SIN(p._latitude._radians);

  const IMathUtils* mu = IMathUtils::instance();

  const double fsinMinSectorLat = mu->log((-sinMinSectorLat-1) / (sinMinSectorLat-1));
  const double fsinMaxSectorLat = mu->log((-sinMaxSectorLat-1) / (sinMaxSectorLat-1));
  const double fsinLat = mu->log((-sinLat-1) / (sinLat-1));


  const double v = (fsinMaxSectorLat - fsinLat) / (fsinMaxSectorLat - fsinMinSectorLat);

  const double u = 1.0 - ((sector._upper._longitude._radians - p._longitude._radians) /
  (sector._upper._longitude._radians - sector._lower._longitude._radians ));

  return Vector2D(u,v);
}
