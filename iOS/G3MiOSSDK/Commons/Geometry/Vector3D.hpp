//
//  Vector3D.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 31/05/12.
//  Copyright (c) 2012 IGO Software SL. All rights reserved.
//

#ifndef G3MiOSSDK_Vector3D_hpp
#define G3MiOSSDK_Vector3D_hpp

#include <math.h>

#include "MutableMatrix44D.hpp"

class MutableVector3D;

class Vector3D {
private:
  const double _x;
  const double _y;
  const double _z;
  
public:
  
  Vector3D(const double x,
           const double y,
           const double z): _x(x), _y(y), _z(z) {
    
  }
  
  Vector3D(const Vector3D &v): _x(v._x), _y(v._y), _z(v._z) {
    
  }
  
  static Vector3D nan() {
    return Vector3D(NAN, NAN, NAN);
  }
  
  bool isNan() const {
    return isnan(_x*_y*_z);
  }
  
  Vector3D normalized() const;
    
  double length() const {
    return sqrt(squaredLength());
  }
  
  double squaredLength() const {
    return _x * _x + _y * _y + _z * _z;
  }
  
  double dot(const Vector3D& v) const {
    return _x * v._x + _y * v._y + _z * v._z;
  }
  
  Vector3D add(const Vector3D& v) const {
    return Vector3D(_x + v._x,
                    _y + v._y,
                    _z + v._z);
  }
  
  Vector3D sub(const Vector3D& v) const {
    return Vector3D(_x - v._x,
                    _y - v._y,
                    _z - v._z);
  }
  
  Vector3D times(const Vector3D& v) const {
    return Vector3D(_x * v._x,
                    _y * v._y,
                    _z * v._z);
  }
  
  Vector3D times(const double magnitude) const {
    return Vector3D(_x * magnitude,
                    _y * magnitude,
                    _z * magnitude);
  }
  
  Vector3D div(const Vector3D& v) const {
    return Vector3D(_x / v._x,
                    _y / v._y,
                    _z / v._z);
  }
  
  Vector3D div(const double v) const {
    return Vector3D(_x / v,
                    _y / v,
                    _z / v);
  }
  
  Vector3D cross(const Vector3D& other) const {
    return Vector3D(_y * other._z - _z * other._y,
                    _z * other._x - _x * other._z,
                    _x * other._y - _y * other._x);
  }
  
  Angle angleBetween(const Vector3D& other) const;
    
  Vector3D rotateAroundAxis(const Vector3D& axis, double theta) const;
  
  double x() const {
    return _x;
  }
  
  double y() const {
    return _y;
  }
  
  double z() const {
    return _z;
  }
  
  Vector3D applyTransform(const MutableMatrix44D &m) const
  {
    //const double * M = m.getMatrix();
    
    Vector3D v(_x * m.get(0) + _y * m.get(4) + _z * m.get(8) + m.get(12),
               _x * m.get(1) + _y * m.get(5) + _z * m.get(9) + m.get(13),
               _x * m.get(2) + _y * m.get(6) + _z * m.get(10) + m.get(14));
        
    return v;
  }
  
  MutableVector3D asMutableVector3D() const;
  
};


#endif
