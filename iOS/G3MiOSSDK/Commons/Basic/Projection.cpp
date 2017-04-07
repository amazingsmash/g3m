//
//  Projection.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/14/16.
//
//

#include "Projection.hpp"

#include "Vector2D.hpp"
#include "MutableVector2D.hpp"
#include "Geodetic2D.hpp"
#include "Sector.hpp"

Projection::Projection() {

}

Projection::~Projection() {
#ifdef JAVA_CODE
  super.dispose();
#endif
}


Vector2D Projection::getUV(const Angle& latitude,
                           const Angle& longitude) const {
  return Vector2D(getU(longitude),
                  getV(latitude));
}

void Projection::getUV(const Angle& latitude,
                       const Angle& longitude,
                       MutableVector2D& result) const {
  result.set(getU(longitude),
             getV(latitude));
}

Vector2D Projection::getUV(const Geodetic2D& position) const {
  return Vector2D(getU(position._longitude),
                  getV(position._latitude));
}

void Projection::getUV(const Geodetic2D& position,
                       MutableVector2D& result) const {
  result.set(getU(position._longitude),
             getV(position._latitude));
}

Geodetic2D Projection::getInnerPoint(const Sector& sector, double u, double v) const{
  const Angle lat = getInnerPointLatitude(sector, v);
  const Angle lon = getInnerPointLongitude(sector, u);
  return Geodetic2D(lat, lon);
}

Vector2D Projection::getUV(const Sector& sector, const Geodetic2D& p) const{
  
  const double lowerV = getV(sector._lower._latitude);
  const double upperV = getV(sector._upper._latitude);
  const double deltaV = upperV - lowerV;
  
  const double globalV = getV(p._latitude);
  const double v = (upperV - globalV) / deltaV;
  
  
  const double lowerU = getU(sector._lower._longitude);
  const double upperU = getU(sector._upper._longitude);
  const double deltaU = upperU - lowerU;
  
  const double globalU = getU(p._longitude);
  const double u = (globalU - lowerU) / deltaU;
  
  return Vector2D(u,v);
  
  
  
//  const double mercatorLowerGlobalV = MercatorUtils::getMercatorV(tileSector._lower._latitude);
//  const double mercatorUpperGlobalV = MercatorUtils::getMercatorV(tileSector._upper._latitude);
//  const double mercatorDeltaGlobalV = mercatorLowerGlobalV - mercatorUpperGlobalV;
//  for (int i = 0; i < verticesArray.size(); i++){
//    
//    p->getU
//    
//    //U
//    const double m_u = tileSector.getUCoordinate(verticesArray[i]->_longitude);
//    
//    //V
//    const double mercatorGlobalV = MercatorUtils::getMercatorV(verticesArray[i]->_latitude);
//    const double m_v = (mercatorGlobalV - mercatorUpperGlobalV) / mercatorDeltaGlobalV;
//    
//    textCoords.add((float)m_u, (float)m_v);
  
}
