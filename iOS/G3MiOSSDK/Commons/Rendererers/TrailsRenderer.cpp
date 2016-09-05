//
//  TrailsRenderer.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 10/23/12.
//
//

#include "TrailsRenderer.hpp"

#include "Mesh.hpp"
#include "DirectMesh.hpp"
#include "Planet.hpp"
#include "GLConstants.hpp"
#include "GLGlobalState.hpp"
#include "IFloatBuffer.hpp"
#include "Camera.hpp"
#include "FloatBufferBuilderFromCartesian3D.hpp"

#define MAX_POSITIONS_PER_SEGMENT 64

TrailSegment::~TrailSegment() {
  delete _previousSegmentLastPosition;
  delete _nextSegmentFirstPosition;

  delete _mesh;

  const size_t positionsSize = _positions.size();
  for (size_t i = 0; i < positionsSize; i++) {
    const Geodetic3D* position = _positions[i];
    delete position;
  }
}

Mesh* TrailSegment::getMesh(const Planet* planet) {
  if (_positionsDirty || (_mesh == NULL)) {
    delete _mesh;
    _mesh = createMesh(planet);
    _positionsDirty = false;
  }
  return _mesh;
}

Mesh* TrailSegment::createMesh(const Planet* planet) {
  const size_t positionsSize = _positions.size();

  if (positionsSize < 2) {
    return NULL;
  }


  std::vector<double> anglesInRadians = std::vector<double>();
  for (size_t i = 1; i < positionsSize; i++) {
    const Geodetic3D* current  = _positions[i];
    const Geodetic3D* previous = _positions[i - 1];

    const double angleInRadians = Geodetic2D::bearingInRadians(previous->_latitude,
                                                               previous->_longitude,
                                                               current->_latitude,
                                                               current->_longitude);
    if (i == 1) {
      if (_previousSegmentLastPosition == NULL) {
        anglesInRadians.push_back(angleInRadians);
        anglesInRadians.push_back(angleInRadians);
      }
      else {
        const double angle2InRadians = Geodetic2D::bearingInRadians(_previousSegmentLastPosition->_latitude,
                                                                    _previousSegmentLastPosition->_longitude,
                                                                    previous->_latitude,
                                                                    previous->_longitude);
        const double avr = (angleInRadians + angle2InRadians) / 2.0;

        anglesInRadians.push_back(avr);
        anglesInRadians.push_back(avr);
      }
    }
    else {
      anglesInRadians.push_back(angleInRadians);
      const double avr = (angleInRadians + anglesInRadians[i - 1]) / 2.0;
      anglesInRadians[i - 1] = avr;
    }
  }

  if (_nextSegmentFirstPosition != NULL) {
    const size_t lastPositionIndex = positionsSize - 1;
    const Geodetic3D* lastPosition = _positions[lastPositionIndex];
    const double angleInRadians =  Geodetic2D::bearingInRadians(lastPosition->_latitude,
                                                                lastPosition->_longitude,
                                                                _nextSegmentFirstPosition->_latitude,
                                                                _nextSegmentFirstPosition->_longitude);

    const double avr = (angleInRadians + anglesInRadians[lastPositionIndex]) / 2.0;
    anglesInRadians[lastPositionIndex] = avr;
  }

  
  const Vector3D offsetP(_ribbonWidth/2, 0, 0);
  const Vector3D offsetN(-_ribbonWidth/2, 0, 0);

  FloatBufferBuilderFromCartesian3D* vertices = FloatBufferBuilderFromCartesian3D::builderWithFirstVertexAsCenter();


  const Vector3D rotationAxis = Vector3D::downZ();
  for (size_t i = 0; i < positionsSize; i++) {
    const Geodetic3D* position = _positions[i];

    const MutableMatrix44D rotationMatrix = MutableMatrix44D::createRotationMatrix(Angle::fromRadians(anglesInRadians[i]),
                                                                                   rotationAxis);
    const MutableMatrix44D geoMatrix = planet->createGeodeticTransformMatrix(*position);
    const MutableMatrix44D matrix = geoMatrix.multiply(rotationMatrix);

    vertices->add(offsetN.transformedBy(matrix, 1));
    vertices->add(offsetP.transformedBy(matrix, 1));
  }

  Mesh* surfaceMesh = new DirectMesh(GLPrimitive::triangleStrip(),
                                     true,
                                     vertices->getCenter(),
                                     vertices->create(),
                                     1,
                                     1,
                                     new Color(_color),
                                     NULL,  // colors
                                     0.0f,  // colorsIntensity
                                     true   // depthTest
                                     );

  delete vertices;

  return surfaceMesh;
}

void Trail::clear() {
  const size_t segmentsSize = _segments.size();
  for (size_t i = 0; i < segmentsSize; i++) {
    TrailSegment* segment = _segments[i];
    delete segment;
  }
  _segments.clear();
}

void Trail::addPosition(const Angle& latitude,
                        const Angle& longitude,
                        const double height) {
  const size_t segmentsSize = _segments.size();

  TrailSegment* currentSegment;
  if (segmentsSize == 0) {
    TrailSegment* newSegment = new TrailSegment(_color, _ribbonWidth);
    _segments.push_back(newSegment);
    currentSegment = newSegment;
  }
  else {
    TrailSegment* previousSegment = _segments[segmentsSize - 1];
    if (previousSegment->getSize() >= MAX_POSITIONS_PER_SEGMENT) {
      TrailSegment* newSegment = new TrailSegment(_color, _ribbonWidth);

      previousSegment->setNextSegmentFirstPosition(latitude,
                                                   longitude,
                                                   height  + _heightDelta);
      newSegment->setPreviousSegmentLastPosition( previousSegment->getPreLastPosition() );
      newSegment->addPosition( previousSegment->getLastPosition() );

      _segments.push_back(newSegment);
      currentSegment = newSegment;
    }
    else {
      currentSegment = previousSegment;
    }
  }

  currentSegment->addPosition(latitude,
                              longitude,
                              height  + _heightDelta);
}

Trail::~Trail() {
  const size_t segmentsSize = _segments.size();
  for (size_t i = 0; i < segmentsSize; i++) {
    TrailSegment* segment = _segments[i];
    delete segment;
  }
}


void TrailsRenderer::updateGLState(const Camera* camera) {
  if (_projection == NULL) {
    _projection = new ProjectionGLFeature(camera->getProjectionMatrix44D());
    _glState->addGLFeature(_projection, true);
  }
  else {
    _projection->setMatrix(camera->getProjectionMatrix44D());
  }

  if (_model == NULL) {
    _model = new ModelGLFeature(camera->getModelMatrix44D());
    _glState->addGLFeature(_model, true);
  }
  else {
    _model->setMatrix(camera->getModelMatrix44D());
  }
}

void TrailSegment::render(const G3MRenderContext* rc,
                          const Frustum* frustum, const GLState* state) {
  Mesh* mesh = getMesh(rc->getPlanet());
  if (mesh != NULL) {
    BoundingVolume* bounding = mesh->getBoundingVolume();
    if (bounding != NULL) {
      if (bounding->touchesFrustum(frustum)) {
        mesh->render(rc, state);
      }
    }
  }
}


void Trail::render(const G3MRenderContext* rc,
                   const Frustum* frustum,
                   const GLState* state) {
  if (_visible) {
    const size_t segmentsSize = _segments.size();
    for (size_t i = 0; i < segmentsSize; i++) {
      TrailSegment* segment = _segments[i];
      segment->render(rc, frustum, state);
    }
  }
}

#pragma mark TrailsRenderer

TrailsRenderer::~TrailsRenderer() {
  const size_t trailsCount = _trails.size();
  for (size_t i = 0; i < trailsCount; i++) {
    Trail* trail = _trails[i];
    delete trail;
  }
  _trails.clear();

  _glState->_release();
}

void TrailsRenderer::addTrail(Trail* trail) {
  if (trail != NULL) {
    _trails.push_back(trail);
  }
}

void TrailsRenderer::render(const G3MRenderContext* rc, GLState* glState) {
  const size_t trailsCount = _trails.size();
  if (trailsCount > 0) {
    const Camera* camera = rc->getCurrentCamera();

    updateGLState(camera);

    const Frustum* frustum = camera->getFrustumInModelCoordinates();
    for (size_t i = 0; i < trailsCount; i++) {
      Trail* trail = _trails[i];
      if (trail != NULL) {
        trail->render(rc, frustum, _glState);
      }
    }
  }
}


void TrailsRenderer::removeTrail(Trail* trail,
                                 bool deleteTrail) {
  const size_t trailsCount = _trails.size();
  for (size_t i = 0; i < trailsCount; i++) {
    Trail* each = _trails[i];
    if (trail == each) {
#ifdef C_CODE
      _trails.erase(_trails.begin() + i);
#endif
#ifdef JAVA_CODE
      _trails.remove(i);
#endif
      if (deleteTrail) {
        delete trail;
      }
      break;
    }
  }
}

void TrailsRenderer::removeAllTrails(bool deleteTrails) {
  if (deleteTrails) {
    const size_t trailsCount = _trails.size();
    for (size_t i = 0; i < trailsCount; i++) {
      Trail* trail = _trails[i];
      delete trail;
    }
  }
  _trails.clear();
}
