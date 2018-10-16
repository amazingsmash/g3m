//
//  WebMercatorPyramidDEMProvider.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 10/19/16.
//
//

#include "WebMercatorPyramidDEMProvider.hpp"


#include "Sector.hpp"
#include "ErrorHandling.hpp"
#include "MercatorUtils.hpp"
#include "PyramidNode.hpp"
#include "Vector2I.hpp"
#include "IMathUtils.hpp"


WebMercatorPyramidDEMProvider::WebMercatorPyramidDEMProvider(const double deltaHeight,
                                                       const Vector2S& tileExtent) :
PyramidDEMProvider(deltaHeight, 1, tileExtent)
{
}

WebMercatorPyramidDEMProvider::~WebMercatorPyramidDEMProvider() {
#ifdef JAVA_CODE
  super.dispose();
#endif
}

PyramidNode* WebMercatorPyramidDEMProvider::createNode(PyramidNode* parent,
                                                    const size_t childID) {
  if (parent == NULL) {
    // creating root node
    return new PyramidNode(NULL,       // parent
                           childID,
                           Sector::FULL_SPHERE,
                           0, 0, 0,    // z, x, y
                           this);
  }

  const int nextZ = parent->_z + 1;
  const int x2    = parent->_x * 2;
  const int y2    = parent->_y * 2; 

  const Geodetic2D lower = parent->_sector._lower;
  const Geodetic2D upper = parent->_sector._upper;
  const Angle splitLongitude = Angle::midAngle(lower._longitude, upper._longitude);
  const Angle splitLatitude  = MercatorUtils::calculateSplitLatitude(lower._latitude, upper._latitude);

  if (childID == 0) {
    const Sector sector(lower,
                        Geodetic2D(splitLatitude, splitLongitude));
    return new PyramidNode(parent, childID, sector,
                           nextZ, x2, y2 + 1,
                           this);
  }
  else if (childID == 1) {
    const Sector sector(Geodetic2D(lower._latitude,   splitLongitude),
                        Geodetic2D(  splitLatitude, upper._longitude));
    return new PyramidNode(parent, childID, sector,
                           nextZ, x2 + 1, y2 + 1,
                           this);
  }
  else if (childID == 2) {
    const Sector sector(Geodetic2D(  splitLatitude, lower._longitude),
                        Geodetic2D(upper._latitude,   splitLongitude));
    return new PyramidNode(parent, childID, sector,
                           nextZ, x2, y2,
                           this);
  }
  else if (childID == 3) {
    const Sector sector(Geodetic2D(splitLatitude, splitLongitude),
                        upper);
    return new PyramidNode(parent, childID, sector,
                           nextZ, x2 + 1, y2,
                           this);
  }
  else {
    THROW_EXCEPTION("Man, isn't it a QuadTree?");
  }
}

//Check out https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#X_and_Y

int WebMercatorPyramidDEMProvider::getSectorLevel(const Sector& s) const{
  int z = 0;
  double lon = 360.0;
  while (lon > s._deltaLongitude._degrees){
    lon = lon / 2.0;
    z++;
  }
  
  return z;
}

Vector2I WebMercatorPyramidDEMProvider::getSectorXY(const Sector& s, int z) const{
  
//  double zoomDLon = 360.0 / ( IMathUtils::instance()->pow(2.0, z));
//  double zoomDLat = 170.1022 / ( IMathUtils::instance()->pow(2.0, z));
//  
//  int x = (int)((s._lower._longitude._degrees + 180.0) / zoomDLon);
//  int y = (int)((90.0 - s._upper._latitude._degrees) / zoomDLat);
//  
//  return Vector2I(x,y);
  
  const IMathUtils* mu = IMathUtils::instance();
  Geodetic2D m = s.getCenter();
  
  double n = mu->pow(2.0, z);
  int xtile = (int)mu->floor(n * ((m._longitude._degrees + 180.0) / 360));
  int ytile = (int)mu->floor(n * (1.0 - (mu->log(tan(m._latitude._radians) + (1.0/mu->cos(m._latitude._radians))) / PI)) / 2.0);
  return Vector2I(xtile,ytile);
}



Geodetic2D WebMercatorPyramidDEMProvider::getNWCornerOfTile(int x, int y, int z) const{
  const IMathUtils* mu = IMathUtils::instance();
  double n = mu->pow(2.0, z);
  double lon_deg = x / n * 360.0 - 180.0;
  double lat_rad = mu->atan(mu->sinh(PI * (1 - 2 * y / n)));
  double lat_deg = TO_DEGREES(lat_rad);
  return Geodetic2D::fromDegrees(lat_deg, lon_deg);
}