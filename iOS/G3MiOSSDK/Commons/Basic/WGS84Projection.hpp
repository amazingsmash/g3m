//
//  WGS84Projection.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/14/16.
//
//

#ifndef WGS84Projection_hpp
#define WGS84Projection_hpp

#include "Projection.hpp"

#warning rename class missing C
class WGS84Projection : public Projection {
private:
  static WGS84Projection* INSTANCE;

  WGS84Projection();

protected:
  virtual ~WGS84Projection();

public:

  static WGS84Projection* instance();

  const std::string getEPSG() const;

  double getU(const Angle& longitude) const;
  double getV(const Angle& latitude) const;

  const Angle getInnerPointLongitude(double u) const;
  const Angle getInnerPointLatitude(double v) const;

  const Angle getInnerPointLongitude(const Sector& sector,
                                     double u) const;
  const Angle getInnerPointLatitude(const Sector& sector,
                                    double v) const;
  
  Geodetic2D* createInnerPoint(const Sector& sector, double u, double v) const;
  
};

#endif
