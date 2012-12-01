package org.glob3.mobile.generated; 
//
//  GEO2DMultiLineStringGeometry.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/30/12.
//
//

//
//  GEO2DMultiLineStringGeometry.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/30/12.
//
//



//C++ TO JAVA CONVERTER NOTE: Java has no need of forward class declarations:
//class Geodetic2D;

public class GEO2DMultiLineStringGeometry extends GEOMultiLineStringGeometry
{
  private java.util.ArrayList<java.util.ArrayList<Geodetic2D>> _coordinatesArray;

  protected final Mesh createMesh(G3MRenderContext rc)
  {
	CompositeMesh composite = new CompositeMesh();
	final int coordinatesArrayCount = _coordinatesArray.size();
	for (int i = 0; i < coordinatesArrayCount; i++)
	{
	  java.util.ArrayList<Geodetic2D> coordinates = _coordinatesArray.get(i);
  
	  composite.addMesh(create2DBoundaryMesh(coordinates, rc));
	}
	return composite;
  }


  public GEO2DMultiLineStringGeometry(java.util.ArrayList<java.util.ArrayList<Geodetic2D>> coordinatesArray)
  {
	  _coordinatesArray = coordinatesArray;

  }

  public void dispose()
  {
	final int coordinatesArrayCount = _coordinatesArray.size();
	for (int i = 0; i < coordinatesArrayCount; i++)
	{
	  java.util.ArrayList<Geodetic2D> coordinates = _coordinatesArray.get(i);
	  final int coordinatesCount = coordinates.size();
	  for (int j = 0; j < coordinatesCount; j++)
	  {
		Geodetic2D coordinate = coordinates.get(j);
		if (coordinate != null)
			coordinate.dispose();
	  }
	  coordinates = null;
	}
  
	_coordinatesArray = null;
  }

}