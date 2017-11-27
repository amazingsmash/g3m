package org.glob3.mobile.generated;
//
//  WGS84Projection.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/14/16.
//
//

//
//  WGS84Projection.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/14/16.
//
//



public class WGS84Projection extends Projection
{
  private static WGS84Projection INSTANCE = null;

  private WGS84Projection()
  {
  }

  public void dispose()
  {
    super.dispose();
  }


  public static WGS84Projection instance()
  {
    if (INSTANCE == null)
    {
      INSTANCE = new WGS84Projection();
    }
    return INSTANCE;
  }

  public final String getEPSG()
  {
    return "EPSG:4326";
  }

  public final double getU(Angle longitude)
  {
    return (longitude._radians + DefineConstants.PI) / (DefineConstants.PI *2);
  }
  public final double getV(Angle latitude)
  {
    return (DefineConstants.HALF_PI - latitude._radians) / DefineConstants.PI;
  }

  public final Angle getInnerPointLongitude(double u)
  {
    return Angle.fromRadians(IMathUtils.instance().linearInterpolation(-DefineConstants.PI, DefineConstants.PI, u));
  }
  public final Angle getInnerPointLatitude(double v)
  {
    return Angle.fromRadians(IMathUtils.instance().linearInterpolation(-DefineConstants.HALF_PI, DefineConstants.HALF_PI, 1.0 - v));
  }

  public final Angle getInnerPointLongitude(Sector sector, double u)
  {
    return Angle.linearInterpolation(sector._lower._longitude, sector._upper._longitude, u);
  }
  public final Angle getInnerPointLatitude(Sector sector, double v)
  {
    return Angle.linearInterpolation(sector._lower._latitude, sector._upper._latitude, 1.0 - v);
  }

  public final Geodetic2D createInnerPoint(Sector sector, double u, double v)
  {
    return new Geodetic2D(getInnerPointLatitude(sector, v), getInnerPointLongitude(sector, u));
  }

  public final Vector2D getUV(Sector sector, Geodetic2D p)
  {
    final double v = (sector._upper._latitude._radians - p._latitude._radians) / (sector._upper._latitude._radians - sector._lower._latitude._radians);
  
  
    final double u = (sector._upper._longitude._radians - p._longitude._radians) / (sector._upper._longitude._radians - sector._lower._longitude._radians);
  
    return new Vector2D(u,v);
  }

}
