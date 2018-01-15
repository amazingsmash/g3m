//
//  PlanetTileTessellator.cpp
//  G3MiOSSDK
//
//  Created by Agustin Trujillo Pino on 12/07/12.
//

#include "PlanetTileTessellator.hpp"

#include "Tile.hpp"
#include "G3MContext.hpp"
#include "IndexedMesh.hpp"
#include "TextureMapping.hpp"
#include "TexturedMesh.hpp"
#include "ShortBufferBuilder.hpp"
#include "FloatBufferBuilderFromGeodetic.hpp"
#include "GLConstants.hpp"
#include "Color.hpp"
#include "Planet.hpp"
#include "IFloatBuffer.hpp"
#include "ElevationData.hpp"
#include "MercatorUtils.hpp"
#include "FloatBufferBuilderFromCartesian2D.hpp"
#include "FloatBufferBuilderFromCartesian3D.hpp"
#include "IndexedGeometryMesh.hpp"
#include "IShortBuffer.hpp"
#include "CompositeMesh.hpp"
#include "PlanetRenderContext.hpp"
#include "LayerTilesRenderParameters.hpp"
#include "NormalsUtils.hpp"
#include "Sphere.hpp"
#include "Vector2S.hpp"
#include "G3MRenderContext.hpp"
#include "DEMGridUtils.hpp"
#include "Geodetic3D.hpp"
#include "Vector2F.hpp"
#include "IMathUtils.hpp"
#include "Vector2D.hpp"
#include "DEMGrid.hpp"
#include "Projection.hpp"
#include "WebMercatorProjection.hpp"
#include "WGS84Projection.hpp"


PlanetTileTessellator::PlanetTileTessellator(const bool skirted, const Sector& sector):
_skirted(skirted),
_renderedSector(sector.isEquals(Sector::FULL_SPHERE)? NULL : new Sector(sector))
{
}

PlanetTileTessellator::~PlanetTileTessellator() {
  delete _renderedSector;
#ifdef JAVA_CODE
  super.dispose();
#endif
  
}

Vector2S PlanetTileTessellator::getTileMeshResolution(const G3MRenderContext* rc,
                                                      const PlanetRenderContext* prc,
                                                      const Tile* tile) const {
  Sector sector = getRenderedSectorForTile(tile);
  return calculateResolution(prc, tile, sector);
}

Vector2S PlanetTileTessellator::calculateResolution(const PlanetRenderContext* prc,
                                                    const Tile* tile,
                                                    const Sector& renderedSector) const {
  const Sector   sector     = tile->_sector;
  const Vector2S resolution = prc->_layerTilesRenderParameters->_tileMeshResolution;
  
  const double latRatio = sector._deltaLatitude._degrees  / renderedSector._deltaLatitude._degrees;
  const double lonRatio = sector._deltaLongitude._degrees / renderedSector._deltaLongitude._degrees;
  
  const IMathUtils* mu = IMathUtils::instance();
  
  short resX = (short) mu->ceil((resolution._x / lonRatio));
  if (resX < 2) {
    resX = 2;
  }
  
  short resY = (short) mu->ceil((resolution._y / latRatio) );
  if (resY < 2) {
    resY = 2;
  }
  
  return Vector2S(resX, resY);
}

double PlanetTileTessellator::skirtDepthForSector(const Planet* planet, const Sector& sector) {
  const Vector3D se = planet->toCartesian(sector.getSE());
  const Vector3D nw = planet->toCartesian(sector.getNW());
  const double diagonalLength = nw.sub(se).length();
  const double sideLength = diagonalLength * 0.70710678118;
  //0.707 = 1 / SQRT(2) -> diagonalLength => estimated side length
  return sideLength / 5.0;
}

Mesh* PlanetTileTessellator::createTileMesh(const G3MRenderContext* rc,
                                            const PlanetRenderContext* prc,
                                            Tile* tile,
                                            const ElevationData* elevationData,
                                            const DEMGrid* grid,
                                            TileTessellatorMeshData& tileTessellatorMeshData) const {
//#warning UNCOMMENT FOR GRID CLOUD
//  if (grid != NULL) {
//    const Vector3D minMaxAverageElevations = DEMGridUtils::getMinMaxAverageElevations(grid);
//    tileTessellatorMeshData._minHeight     = minMaxAverageElevations._x;
//    tileTessellatorMeshData._maxHeight     = minMaxAverageElevations._y;
//    // tileTessellatorMeshData._averageHeight = minMaxAverageElevations._z;
//    tileTessellatorMeshData._averageHeight = 0;
//    tileTessellatorMeshData._needsTexturizing = false;
//    
//    return DEMGridUtils::createDebugMesh(grid,
//                                         rc->getPlanet(),
//                                         prc->_verticalExaggeration,
//                                         Geodetic3D::zero(), // offset
//                                         -11000,             // minElevation
//                                         9000,               // maxElevation
//                                         15                  // pointSize
//                                         );
//  }
  
  const Sector tileSector = tile->_sector;
  const Sector meshSector = getRenderedSectorForTile(tile);
  const Vector2S meshResolution = calculateResolution(prc, tile, meshSector);
  
  const Planet* planet = rc->getPlanet();
  
  FloatBufferBuilderFromGeodetic* vertices = FloatBufferBuilderFromGeodetic::builderWithGivenCenter(planet, meshSector._center);
  ShortBufferBuilder indices;
  FloatBufferBuilderFromCartesian2D* textCoords = new FloatBufferBuilderFromCartesian2D();
  
  
  double minElevation;
  if (grid == NULL){
    
    minElevation = createSurface(tileSector,
                                 meshSector,
                                 meshResolution,
                                 elevationData,
                                 prc->_verticalExaggeration,
                                 tile->_mercator,
                                 vertices,
                                 indices,
                                 *textCoords,
                                 tileTessellatorMeshData);
  } else{
    minElevation = createSurface(tileSector,
                                 grid,
                                 prc->_verticalExaggeration,
                                 tile->_mercator,
                                 vertices,
                                 indices,
                                 *textCoords,
                                 tileTessellatorMeshData);
  }
  
  if (_skirted) {
    const double relativeSkirtHeight = minElevation - skirtDepthForSector(planet, tileSector);
    
    double absoluteSkirtHeight = 0;
    if (_renderedSector != NULL) {
#ifdef C_CODE
      absoluteSkirtHeight = -skirtDepthForSector(planet, *_renderedSector);
#endif
#ifdef JAVA_CODE
      absoluteSkirtHeight = -skirtDepthForSector(planet, _renderedSector);
#endif
    }
    
    createEastSkirt(planet,
                    tileSector,
                    meshSector,
                    meshResolution,
                    needsEastSkirt(tileSector) ? relativeSkirtHeight : absoluteSkirtHeight,
                    vertices,
                    indices,
                    *textCoords);
    
    createNorthSkirt(planet,
                     tileSector,
                     meshSector,
                     meshResolution,
                     needsNorthSkirt(tileSector) ? relativeSkirtHeight : absoluteSkirtHeight,
                     vertices,
                     indices,
                     *textCoords);
    
    createWestSkirt(planet,
                    tileSector,
                    meshSector,
                    meshResolution,
                    needsWestSkirt(tileSector) ? relativeSkirtHeight : absoluteSkirtHeight,
                    vertices,
                    indices,
                    *textCoords);
    
    createSouthSkirt(planet,
                     tileSector,
                     meshSector,
                     meshResolution,
                     needsSouthSkirt(tileSector) ? relativeSkirtHeight : absoluteSkirtHeight,
                     vertices,
                     indices,
                     *textCoords);
  }
  
  //Storing textCoords in Tile
  tile->setPlanetTileTessellatorData(new PlanetTileTessellatorData(textCoords));
  
  IFloatBuffer* verticesB = vertices->create();
  IShortBuffer* indicesB  = indices.create();
  IFloatBuffer* normals = NULL;
  //#warning Testing_Terrain_Normals;
  //  IFloatBuffer* normals = NormalsUtils::createTriangleStripSmoothNormals(verticesB, indicesB);
  
  Mesh* result = new IndexedGeometryMesh(GLPrimitive::triangleStrip(),
                                         vertices->getCenter(),
                                         verticesB, true,
                                         normals,   true,
                                         indicesB,  true);
  
  delete vertices;
  
  return result;
}

const Vector2F PlanetTileTessellator::getTextCoord(const Tile* tile,
                                                   const Angle& latitude,
                                                   const Angle& longitude) const {
  const Sector sector = tile->_sector;
  
  const Vector2F linearUV = sector.getUVCoordinatesF(latitude, longitude);
  if (!tile->_mercator) {
    return linearUV;
  }
  
  const double lowerGlobalV = MercatorUtils::getMercatorV(sector._lower._latitude);
  const double upperGlobalV = MercatorUtils::getMercatorV(sector._upper._latitude);
  const double deltaGlobalV = lowerGlobalV - upperGlobalV;
  
  const double globalV = MercatorUtils::getMercatorV(latitude);
  const double localV  = (globalV - upperGlobalV) / deltaGlobalV;
  
  return Vector2F(linearUV._x, (float) localV);
}

IFloatBuffer* PlanetTileTessellator::createTextCoords(const Vector2S& rawResolution,
                                                      const Tile* tile) const {
  PlanetTileTessellatorData* data = tile->getPlanetTileTessellatorData();
  if ((data == NULL) || (data->_textCoords == NULL)) {
    ILogger::instance()->logError("Logic error on PlanetTileTessellator::createTextCoord");
    return NULL;
  }
  return data->_textCoords->create();
}

Mesh* PlanetTileTessellator::createTileDebugMesh(const G3MRenderContext* rc,
                                                 const PlanetRenderContext* prc,
                                                 const Tile* tile,
                                                 const DEMGrid* demGrid) const {
  const Sector meshSector = getRenderedSectorForTile(tile);
  const Vector2S meshResolution = calculateResolution(prc, tile, meshSector);
  
  FloatBufferBuilderFromGeodetic* vertices = FloatBufferBuilderFromGeodetic::builderWithFirstVertexAsCenter(rc->getPlanet());
  TileTessellatorMeshData tileTessellatorMeshData;
  
  if (demGrid != NULL){
    std::vector<const Geodetic2D *> verticesArray;
    createSurfaceVerticesFromDEMGrid(demGrid,
                                     prc->_verticalExaggeration,
                                     vertices,
                                     tileTessellatorMeshData,
                                     verticesArray);
    for (size_t i = 0; i < verticesArray.size(); i++) {
      delete verticesArray[i];
    }
    
  } else{
    createSurfaceVertices(meshResolution,
                          meshSector,
                          tile->getElevationData(),
                          prc->_verticalExaggeration,
                          vertices,
                          tileTessellatorMeshData);
  }
  
  //INDEX OF BORDER
  ShortBufferBuilder indicesBorder;
  for (short j = 0; j < meshResolution._x; j++) {
    indicesBorder.add(j);
  }
  
  for (short i = 2; i < meshResolution._y+1; i++) {
    indicesBorder.add((short)((i * meshResolution._x)-1));
  }
  
  for (short j = (short)(meshResolution._x*meshResolution._y-2);
       j >= (meshResolution._x*(meshResolution._y-1));
       j--) {
    indicesBorder.add(j);
  }
  
  for (short j = (short)(meshResolution._x*(meshResolution._y-1)-meshResolution._x);
       j >= 0;
       j-=meshResolution._x) {
    indicesBorder.add(j);
  }
  
  //INDEX OF GRID
  ShortBufferBuilder indicesGrid;
  for (short i = 0; i < meshResolution._y-1; i++) {
    short rowOffset = (short)(i * meshResolution._x);
    
    for (short j = 0; j < meshResolution._x; j++) {
      indicesGrid.add((short)(rowOffset + j));
      indicesGrid.add((short)(rowOffset + j+meshResolution._x));
    }
    for (short j = (short)((2*meshResolution._x)-1); j >= meshResolution._x; j--) {
      indicesGrid.add((short)(rowOffset + j));
    }
    
  }
  
  const Color levelColor = Color::BLUE.wheelStep(5, tile->_level % 5);
  const float gridLineWidth = tile->isElevationDataSolved() || (tile->getElevationData() == NULL) ? 1.0f : 3.0f;
  
  IndexedMesh* border = new IndexedMesh(GLPrimitive::lineStrip(),
                                        vertices->getCenter(),
                                        vertices->create(),
                                        true,
                                        indicesBorder.create(),
                                        true,
                                        2.0f,
                                        1.0f,
                                        Color::newFromRGBA(1.0f, 0.0f, 0.0f, 1.0f),
                                        NULL,
                                        false,
                                        NULL,
                                        true, 1.0f, 1.0f);
  
  IndexedMesh* grid = new IndexedMesh(GLPrimitive::lineStrip(),
                                      vertices->getCenter(),
                                      vertices->create(),
                                      true,
                                      indicesGrid.create(),
                                      true,
                                      gridLineWidth,
                                      1.0f,
                                      new Color(levelColor),
                                      NULL,
                                      false,
                                      NULL,
                                      true, 1.0f, 1.0f);
  
  delete vertices;
  
  CompositeMesh* c = new CompositeMesh();
  c->addMesh(grid);
  c->addMesh(border);
  
  return c;
}

Sector PlanetTileTessellator::getRenderedSectorForTile(const Tile* tile) const {
  if (_renderedSector == NULL) {
    return tile->_sector;
  }
#ifdef C_CODE
  return tile->_sector.intersection(*_renderedSector);
#endif
#ifdef JAVA_CODE
  return tile._sector.intersection(_renderedSector);
#endif
}

double PlanetTileTessellator::createSurfaceVerticesFromDEMGrid(const DEMGrid* grid,
                                                               float verticalExaggeration,
                                                               FloatBufferBuilderFromGeodetic* vertices,
                                                               TileTessellatorMeshData& tileTessellatorMeshData,
                                                               std::vector<const Geodetic2D*>& verticesArray) const{
  const IMathUtils* mu = IMathUtils::instance();
  double minElevation = mu->maxDouble();
  double maxElevation = mu->minDouble();
  double sumElevation = 0;
  
  const int mrx = grid->getExtent()._x;
  const int mry = grid->getExtent()._y;
  Sector sector = grid->getSector();
  
//  printf("--------------------------------------------------------\n");
  
  for (int j = mry-1; j > -1; j--) { //Moving on lat
    
    for (int i = 0; i < mrx; i++) { //Moving on lon
      
      const Geodetic2D position = grid->getInnerPoint(i, j); //Starting at max lat, min lon
      verticesArray.push_back(new Geodetic2D(position));
      
      const double rawElevation = grid->getElevation(i, j);
     
//      if (rawElevation > 400.0){
//        printf("%f\n", rawElevation);
//        grid->getElevation(i, j);
//      }
      
      double elevation = ISNAN(rawElevation)? 0 : rawElevation * verticalExaggeration;
      
      if (elevation < minElevation) {
        minElevation = elevation;
      }
      
      if (elevation > maxElevation) {
        maxElevation = elevation;
      }
      
      sumElevation += elevation;
      
      vertices->add(position, elevation );
    }
  }
  
  if (minElevation == mu->maxDouble()) {
    minElevation = 0;
  }
  if (maxElevation == mu->minDouble()) {
    maxElevation = 0;
  }
  
  tileTessellatorMeshData._minHeight = minElevation;
  tileTessellatorMeshData._maxHeight = maxElevation;
  tileTessellatorMeshData._averageHeight = sumElevation / (mrx * mry);
  
  return minElevation;
  
  
}

double PlanetTileTessellator::createSurfaceVertices(const Vector2S& meshResolution, //Mesh resolution
                                                    const Sector& meshSector,
                                                    const ElevationData* elevationData,
                                                    float verticalExaggeration,
                                                    FloatBufferBuilderFromGeodetic* vertices,
                                                    TileTessellatorMeshData& tileTessellatorMeshData) const {
  
  const IMathUtils* mu = IMathUtils::instance();
  double minElevation = mu->maxDouble();
  double maxElevation = mu->minDouble();
  double sumElevation = 0;
  
  for (int j = 0; j < meshResolution._y; j++) { //Moving in lat
    const double v = (double) j / (meshResolution._y - 1);
    
    for (int i = 0; i < meshResolution._x; i++) { //Moving on lon
      const double u = (double) i / (meshResolution._x - 1);
      const Geodetic2D position = meshSector.getInnerPoint(u, v); //Starting at max lat, min lon
      double elevation = 0;
      
      if (elevationData != NULL) {
        const double rawElevation = elevationData->getElevationAt(position);
        
        elevation = ISNAN(rawElevation)? 0 : rawElevation * verticalExaggeration;
        
#warning JM: REMOVE THIS - JUST FOR TESTS
        if (elevation < 0.0){
          elevation = 0.0;
        }
        
        if (elevation < minElevation) {
          minElevation = elevation;
        }
        
        if (elevation > maxElevation) {
          maxElevation = elevation;
        }
        
        sumElevation += elevation;
      }
      
      vertices->add( position, elevation );
    }
  }
  
  if (minElevation == mu->maxDouble()) {
    minElevation = 0;
  }
  if (maxElevation == mu->minDouble()) {
    maxElevation = 0;
  }
  
  tileTessellatorMeshData._minHeight = minElevation;
  tileTessellatorMeshData._maxHeight = maxElevation;
  tileTessellatorMeshData._averageHeight = sumElevation / (meshResolution._x * meshResolution._y);
  
  return minElevation;
}

void PlanetTileTessellator::createSurfaceTextureCoordinates(bool mercator,
                                                            const Vector2S& meshResolution,
                                                            const Sector& tileSector,
                                                            const Sector& meshSector,
                                                            FloatBufferBuilderFromCartesian2D& textCoords) const{
  
  if (mercator) {
    const double mercatorLowerGlobalV = MercatorUtils::getMercatorV(tileSector._lower._latitude);
    const double mercatorUpperGlobalV = MercatorUtils::getMercatorV(tileSector._upper._latitude);
    const double mercatorDeltaGlobalV = mercatorLowerGlobalV - mercatorUpperGlobalV;
    
    for (int j = 0; j < meshResolution._y; j++) {
      const double v = (double) j / (meshResolution._y - 1);
      
      for (int i = 0; i < meshResolution._x; i++) {
        const double u = (double) i / (meshResolution._x - 1);
        
        const Angle lat = Angle::linearInterpolation( meshSector._lower._latitude,  meshSector._upper._latitude,  1.0 - v );
        const Angle lon = Angle::linearInterpolation( meshSector._lower._longitude, meshSector._upper._longitude,       u );
        
        //U
        const double m_u = tileSector.getUCoordinate(lon);
        
        //V
        const double mercatorGlobalV = MercatorUtils::getMercatorV(lat);
        const double m_v = (mercatorGlobalV - mercatorUpperGlobalV) / mercatorDeltaGlobalV;
        
        textCoords.add((float)m_u, (float)m_v);
      }
    }
    
  }
  else {
    for (int j = 0; j < meshResolution._y; j++) {
      const double v = (double) j / (meshResolution._y - 1);
      for (int i = 0; i < meshResolution._x; i++) {
        const double u = (double) i / (meshResolution._x - 1);
//        textCoords.add((float)u, (float)v);
          
          
          ////
          const Angle lat = Angle::linearInterpolation( meshSector._lower._latitude,  meshSector._upper._latitude,  1.0 - v );
          const Angle lon = Angle::linearInterpolation( meshSector._lower._longitude, meshSector._upper._longitude,       u );
          
          //U
          const double m_u = tileSector.getUCoordinate(lon);
          
          //V
          const double m_v = tileSector.getVCoordinate(lat);
          
          textCoords.add((float)m_u, (float)m_v);
      }
    }
  }
}

void PlanetTileTessellator::createSurfaceIndices(const Vector2S& meshResolution,
                                                 ShortBufferBuilder& indices) const{
  //INDEX
  for (short j = 0; j < (meshResolution._y-1); j++) {
    const short jTimesResolution = (short)(j*meshResolution._x);
    if (j > 0) {
      indices.add(jTimesResolution);
    }
    for (short i = 0; i < meshResolution._x; i++) {
      indices.add((short)(jTimesResolution + i));
      indices.add((short)(jTimesResolution + i + meshResolution._x));
    }
    indices.add((short)(jTimesResolution + 2*meshResolution._x - 1));
  }
}


double PlanetTileTessellator::createSurface(const Sector& tileSector,
                                            const DEMGrid* demGrid,
                                            float verticalExaggeration,
                                            bool mercator,
                                            FloatBufferBuilderFromGeodetic* vertices,
                                            ShortBufferBuilder& indices,
                                            FloatBufferBuilderFromCartesian2D& textCoords,
                                            TileTessellatorMeshData& tileTessellatorMeshData) const {
  
  std::vector<const Geodetic2D*> verticesArray;
  
  //VERTICES
  const double minElevation = createSurfaceVerticesFromDEMGrid(demGrid,
                                                               verticalExaggeration,
                                                               vertices,
                                                               tileTessellatorMeshData,
                                                               verticesArray);
  
#warning TODO: turn this into parameter
  Projection* textureProjection;
  if (mercator){
    textureProjection = WebMercatorProjection::instance();
  } else{
    textureProjection = WGS84Projection::instance();
  }
  
  for (int i = 0; i < verticesArray.size(); i++){
    Vector2D tc = textureProjection->getUV(tileSector, *verticesArray[i]);
      textCoords.add(1.0 - tc._x, tc._y);
  }
  
  createSurfaceIndices(Vector2S((short)demGrid->getExtent()._x, (short)demGrid->getExtent()._y), indices);
  
  //Deleting vertices
  for (int i = 0; i < verticesArray.size(); i++) {
    delete verticesArray[i];
  }
  
  return minElevation;
}




double PlanetTileTessellator::createSurface(const Sector& tileSector,
                                            const Sector& meshSector,
                                            const Vector2S& meshResolution,
                                            const ElevationData* elevationData,
                                            float verticalExaggeration,
                                            bool mercator,
                                            FloatBufferBuilderFromGeodetic* vertices,
                                            ShortBufferBuilder& indices,
                                            FloatBufferBuilderFromCartesian2D& textCoords,
                                            TileTessellatorMeshData& tileTessellatorMeshData) const {
  
  //VERTICES
  const double minElevation = createSurfaceVertices(Vector2S(meshResolution._x, meshResolution._y),
                                                    meshSector,
                                                    elevationData,
                                                    verticalExaggeration,
                                                    vertices,
                                                    tileTessellatorMeshData);
  
  createSurfaceTextureCoordinates(mercator,
                                  meshResolution,
                                  tileSector,
                                  meshSector,
                                  textCoords);
  
  createSurfaceIndices(meshResolution, indices);
  
  return minElevation;
}

void PlanetTileTessellator::createEastSkirt(const Planet* planet,
                                            const Sector& tileSector,
                                            const Sector& meshSector,
                                            const Vector2S& meshResolution,
                                            double skirtHeight,
                                            FloatBufferBuilderFromGeodetic* vertices,
                                            ShortBufferBuilder& indices,
                                            FloatBufferBuilderFromCartesian2D& textCoords) const {
  
  //VERTICES
  const short firstSkirtVertex = (short)(vertices->size() / 3);
  
  const short southEastCorner =  (short)((meshResolution._x * meshResolution._y) - 1);
  
  short skirtIndex = firstSkirtVertex;
  short surfaceIndex = southEastCorner;
  
  // east side
  for (short j =  (short)(meshResolution._y-1); j >= 0; j--) {
    const double x = 1;
    const double y = (double)j/(meshResolution._y-1);
    const Geodetic2D g = meshSector.getInnerPoint(x, y);
    vertices->add(g, skirtHeight);
    
    //TEXTURE COORDS
    Vector2D uv = textCoords.getVector2D(surfaceIndex);
    textCoords.add(uv);
    
    //INDEX
    indices.add(surfaceIndex);
    indices.add(skirtIndex);
    
    skirtIndex++;
    surfaceIndex -= meshResolution._x;
  }
  //Short casts are needed due to widening primitive conversions in java
  //http://docs.oracle.com/javase/specs/jls/se7/html/jls-5.html#jls-5.6.2
  indices.add((short)(surfaceIndex + meshResolution._x));
  indices.add((short)(surfaceIndex + meshResolution._x));
}

void PlanetTileTessellator::createNorthSkirt(const Planet* planet,
                                             const Sector& tileSector,
                                             const Sector& meshSector,
                                             const Vector2S& meshResolution,
                                             double skirtHeight,
                                             FloatBufferBuilderFromGeodetic* vertices,
                                             ShortBufferBuilder& indices,
                                             FloatBufferBuilderFromCartesian2D& textCoords) const {
  
  //VERTICES
  const short firstSkirtVertex = (short) (vertices->size() / 3);
  const short northEastCorner =  (short)(meshResolution._x - 1);
  
  short skirtIndex = firstSkirtVertex;
  short surfaceIndex = northEastCorner;
  
  indices.add(surfaceIndex);
  
  for (short i = (short)(meshResolution._x-1); i >= 0; i--) {
    const double x = (double)i/(meshResolution._x-1);
    const double y = 0;
    const Geodetic2D g = meshSector.getInnerPoint(x, y);
    vertices->add(g, skirtHeight);
    
    //TEXTURE COORDS
    Vector2D uv = textCoords.getVector2D(surfaceIndex);
    textCoords.add(uv);
    
    //INDEX
    indices.add(surfaceIndex);
    indices.add(skirtIndex);
    
    skirtIndex++;
    surfaceIndex -= 1;
  }
  
  indices.add( (short)(surfaceIndex + 1));
  indices.add( (short)(surfaceIndex + 1));
}

void PlanetTileTessellator::createWestSkirt(const Planet* planet,
                                            const Sector& tileSector,
                                            const Sector& meshSector,
                                            const Vector2S& meshResolution,
                                            double skirtHeight,
                                            FloatBufferBuilderFromGeodetic* vertices,
                                            ShortBufferBuilder& indices,
                                            FloatBufferBuilderFromCartesian2D& textCoords) const {
  
  //VERTICES
  const short firstSkirtVertex = (short) (vertices->size() / 3);
  
  const short northWestCorner = (short)0;
  
  short skirtIndex = firstSkirtVertex;
  short surfaceIndex = northWestCorner;
  
  indices.add(surfaceIndex);
  
  for (short j = 0; j < meshResolution._y; j++) {
    const double x = 0;
    const double y = (double)j/(meshResolution._y-1);
    const Geodetic2D g = meshSector.getInnerPoint(x, y);
    vertices->add(g, skirtHeight);
    
    //TEXTURE COORDS
    Vector2D uv = textCoords.getVector2D(surfaceIndex);
    textCoords.add(uv);
    
    //INDEX
    indices.add(surfaceIndex);
    indices.add(skirtIndex);
    
    skirtIndex++;
    surfaceIndex += meshResolution._x;
  }
  
  indices.add( (short)(surfaceIndex - meshResolution._x));
  indices.add( (short)(surfaceIndex - meshResolution._x));
}

void PlanetTileTessellator::createSouthSkirt(const Planet* planet,
                                             const Sector& tileSector,
                                             const Sector& meshSector,
                                             const Vector2S& meshResolution,
                                             double skirtHeight,
                                             FloatBufferBuilderFromGeodetic* vertices,
                                             ShortBufferBuilder& indices,
                                             FloatBufferBuilderFromCartesian2D& textCoords) const {
  
  //VERTICES
  const short firstSkirtVertex = (short) (vertices->size() / 3);
  
  const short southWestCorner = (short) (meshResolution._x * (meshResolution._y-1));
  
  short skirtIndex = firstSkirtVertex;
  short surfaceIndex = southWestCorner;
  
  indices.add(surfaceIndex);
  
  for (short i = 0; i < meshResolution._x; i++) {
    const double x = (double)i/(meshResolution._x-1);
    const double y = 1;
    const Geodetic2D g = meshSector.getInnerPoint(x, y);
    vertices->add(g, skirtHeight);
    
    //TEXTURE COORDS
    Vector2D uv = textCoords.getVector2D(surfaceIndex);
    textCoords.add((float)uv._x, (float)uv._y);
    
    //INDEX
    indices.add(surfaceIndex);
    indices.add(skirtIndex++);
    surfaceIndex += 1;
  }
  
  indices.add( (short)(surfaceIndex - 1));
  indices.add( (short)(surfaceIndex - 1));
}
