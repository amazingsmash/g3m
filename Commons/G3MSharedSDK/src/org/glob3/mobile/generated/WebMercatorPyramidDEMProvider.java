package org.glob3.mobile.generated;
//
//  WebMercatorPyramidDEMProvider.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 10/19/16.
//
//

//
//  WebMercatorPyramidDEMProvider.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 10/19/16.
//
//



public abstract class WebMercatorPyramidDEMProvider extends PyramidDEMProvider
{

  protected WebMercatorPyramidDEMProvider(double deltaHeight, Vector2S tileExtent)
  {
     super(deltaHeight, 1, tileExtent);
  }

  public void dispose()
  {
    super.dispose();
  }


  public final PyramidNode createNode(PyramidNode parent, int childID)
  {
    if (parent == null)
    {
      // creating root node
      return new PyramidNode(null, childID, Sector.FULL_SPHERE, 0, 0, 0, this); // z, x, y -  parent
    }
  
    final int nextZ = parent._z + 1;
    final int x2 = parent._x * 2;
    final int y2 = parent._y * 2;
  
    final Geodetic2D lower = parent._sector._lower;
    final Geodetic2D upper = parent._sector._upper;
    final Angle splitLongitude = Angle.midAngle(lower._longitude, upper._longitude);
    final Angle splitLatitude = MercatorUtils.calculateSplitLatitude(lower._latitude, upper._latitude);
  
    if (childID == 0)
    {
      final Sector sector = new Sector(lower, new Geodetic2D(splitLatitude, splitLongitude));
      return new PyramidNode(parent, childID, sector, nextZ, x2, y2 + 1, this);
    }
    else if (childID == 1)
    {
      final Sector sector = new Sector(new Geodetic2D(lower._latitude, splitLongitude), new Geodetic2D(splitLatitude, upper._longitude));
      return new PyramidNode(parent, childID, sector, nextZ, x2 + 1, y2 + 1, this);
    }
    else if (childID == 2)
    {
      final Sector sector = new Sector(new Geodetic2D(splitLatitude, lower._longitude), new Geodetic2D(upper._latitude, splitLongitude));
      return new PyramidNode(parent, childID, sector, nextZ, x2, y2, this);
    }
    else if (childID == 3)
    {
      final Sector sector = new Sector(new Geodetic2D(splitLatitude, splitLongitude), upper);
      return new PyramidNode(parent, childID, sector, nextZ, x2 + 1, y2, this);
    }
    else
    {
      throw new RuntimeException("Man, isn't it a QuadTree?");
    }
  }

  public abstract long requestDataFor(PyramidNode node);


  //Check out https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#X_and_Y
  
  public final int getSectorLevel(Sector s)
  {
    int z = 0;
    double lon = 360.0;
    while (lon > s._deltaLongitude._degrees)
    {
      lon = lon / 2.0;
      z++;
    }
  
    return z;
  }
  public final Vector2I getSectorXY(Sector s, int z)
  {
  
  //  double zoomDLon = 360.0 / ( IMathUtils::instance()->pow(2.0, z));
  //  double zoomDLat = 170.1022 / ( IMathUtils::instance()->pow(2.0, z));
  //
  //  int x = (int)((s._lower._longitude._degrees + 180.0) / zoomDLon);
  //  int y = (int)((90.0 - s._upper._latitude._degrees) / zoomDLat);
  //
  //  return Vector2I(x,y);
  
    final IMathUtils mu = IMathUtils.instance();
    Geodetic2D m = s.getCenter();
  
    double n = mu.pow(2.0, z);
    int xtile = (int)mu.floor(n * ((m._longitude._degrees + 180.0) / 360));
    int ytile = (int)mu.floor(n * (1.0 - (mu.log(Math.tan(m._latitude._radians) + (1.0/mu.cos(m._latitude._radians))) / DefineConstants.PI)) / 2.0);
    return new Vector2I(xtile,ytile);
  }

  public final Geodetic2D getNWCornerOfTile(int x, int y, int z)
  {
    final IMathUtils mu = IMathUtils.instance();
    double n = mu.pow(2.0, z);
    double lon_deg = x / n * 360.0 - 180.0;
    double lat_rad = mu.atan(mu.sinh(DefineConstants.PI * (1 - 2 * y / n)));
    double lat_deg = ((lat_rad) * (180.0 / 3.14159265358979323846264338327950288));
    return Geodetic2D.fromDegrees(lat_deg, lon_deg);
  }

}
