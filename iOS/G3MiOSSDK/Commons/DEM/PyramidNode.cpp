//
//  PyramidNode.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 10/20/16.
//
//

#include "PyramidNode.hpp"

#include "PyramidDEMProvider.hpp"
#include "DEMGrid.hpp"
#include "ErrorHandling.hpp"
#include "DEMSubscription.hpp"
#include "DEMGridUtils.hpp"
#include "ILogger.hpp"

//#warning TODO: REMOVE
//static int NPYRAMIDNODES = 0;


PyramidNode::PyramidNode(PyramidNode*  parent,
                         const size_t        childID,
                         const Sector&       sector,
                         const int           z,
                         const int           x,
                         const int           y,
                         PyramidDEMProvider* pyramidDEMProvider) :
_parent(parent),
//_childID(childID),
_sector(sector),
_resolution(sector._deltaLatitude.div(pyramidDEMProvider->_tileExtent._y),
            sector._deltaLongitude.div(pyramidDEMProvider->_tileExtent._x)),
_z(z), _x(x), _y(y),
_pyramidDEMProvider(pyramidDEMProvider),
_grid(NULL),
//_stickyGrid(false),
_children(NULL),
_childrenSize(0),
_subscriptions(NULL),
_dataRequestPending(false),
_dataRequestID(-1)
{
//  NPYRAMIDNODES++;
//  printf("N Pyramid Nodes = %d\n", NPYRAMIDNODES);
}

PyramidNode::~PyramidNode() {
  if (_grid != NULL) {
    _grid->_release();
  }
  
  if (_children != NULL) {
    for (size_t i = 0; i < _childrenSize; i++) {
      PyramidNode* child = _children->at(i);
      delete child;
    }
    delete _children;
  }
  
  if (_subscriptions != NULL) {
    const size_t subscriptionsSize = _subscriptions->size();
    for (size_t i = 0; i < subscriptionsSize; i++) {
      DEMSubscription* subscription = _subscriptions->at(i);
      subscription->_release();
    }
    delete _subscriptions;
  }
  
  if (_dataRequestPending && _pyramidDEMProvider != NULL){
    _pyramidDEMProvider->cancelDataRequest(_dataRequestID);
  }
  
//  NPYRAMIDNODES--;
//  printf("N Pyramid Nodes = %d\n", NPYRAMIDNODES);
}

std::vector<PyramidNode*>* PyramidNode::getChildren() {
  if (_children == NULL) {
    _children = new std::vector<PyramidNode*>();
    _childrenSize = 4;
    for (size_t i = 0; i < _childrenSize; i++) {
      PyramidNode* child = _pyramidDEMProvider->createNode(this, i);
      _children->push_back(child);
    }
  }
  return _children;
}

bool PyramidNode::insertGrid(int z,
                             int x,
                             int y,
                             DEMGrid* grid,
                             const bool stickyGrid) {
  if (z < _z) {
    THROW_EXCEPTION("Logic error!");
  }
  else if (z == _z) {
    if ((x == _x) && (y == _y)) {
      
      if (!_sector.isEquals(grid->getSector())){
        ILogger::instance()->logWarning("PyramidNode and Grid do not match exactly. %s != %s\n",
                                        grid->getSector().description().c_str(),
                                        _sector.description().c_str());
      }
      
      if (_grid != NULL){
        ILogger::instance()->logError("Storing a grid twice.");
        _grid->_release();
      }
      
      _grid = grid;
      _dataRequestPending = false;
      notifySubtreeSubscriptors(grid);
      
      _stickyGrid = stickyGrid;
      return true;
    }
    return false;
  }
  
  //If grid is sticky (forced) children must be created
  //Otherwise do not store grids that don't have subscribers
  std::vector<PyramidNode*>* children = stickyGrid? getChildren() : _children;
  if (children != NULL){
    for (size_t i = 0; i < _childrenSize; i++) {
      PyramidNode* child = children->at(i);
      if (child->insertGrid(z, x, y,
                            grid, stickyGrid)) {
        return true;
      }
    }
  }
  
  
  
  return false;
}

bool PyramidNode::addSubscription(DEMGrid* grid,
                                  DEMSubscription* subscription) {
  const bool gridBelongsToSubTree = subscription->_sector.sharesAreaWith(_sector);
  
  //if (subscription->_sector.touchesWith(_sector)) {
  if (gridBelongsToSubTree) {
    
    Sector intersection = subscription->_sector.intersection(_sector);
    
    
    DEMGrid* bestGrid = (_grid == NULL) ? grid : _grid;
    
    const bool notEnoughResolution = (_resolution._latitude.greaterThan ( subscription->_resolution._latitude  ) ||
                                      _resolution._longitude.greaterThan( subscription->_resolution._longitude ) );
    
    if (notEnoughResolution) { //Passing subscriptor to child
      std::vector<PyramidNode*>* children = getChildren();
      for (size_t i = 0; i < _childrenSize; i++) {
        PyramidNode* child = children->at(i);
        if (child->addSubscription(bestGrid, subscription)){
          return true;
        }
      }
      ILogger::instance()->logError("PyramidNode::addSubscription logic error.");
    }
    else {
      if (_subscriptions == NULL) {
        _subscriptions = new std::vector<DEMSubscription*>();
      }
      subscription->_retain();
      _subscriptions->push_back(subscription);
      if (bestGrid != NULL) {
        DEMGrid* selectedGrid = DEMGridUtils::bestGridFor(bestGrid,
                                                          subscription->_sector,
                                                          subscription->_extent);
        if (selectedGrid != NULL) {
          subscription->onGrid(selectedGrid);
        } else{
          ILogger::instance()->logWarning("No GRID returned after subscription.");
        }
        
        if (_grid == NULL && !_dataRequestPending){ //There's no data for this node yet
          _dataRequestPending = true;
          _dataRequestID = _pyramidDEMProvider->requestDataFor(this);
        }
      }
    }
  }
  
  return gridBelongsToSubTree;
}

void PyramidNode::removeSubscription(DEMSubscription* subscription) {
  if (_subscriptions != NULL) {
    const size_t subscriptionsSize = _subscriptions->size();
    for (size_t i = 0; i < subscriptionsSize; i++) {
      if (_subscriptions->at(i) == subscription) {
        subscription->_release();
#ifdef C_CODE
        _subscriptions->erase(_subscriptions->begin() + i);
#endif
#ifdef JAVA_CODE
        _subscriptions.remove(i);
#endif
        break;
      }
    }
    if (_subscriptions->empty()) {
      delete _subscriptions;
      _subscriptions = NULL;
      
    }
  }
  
  if (_children != NULL) {
    for (size_t i = 0; i < _childrenSize; i++) {
      PyramidNode* child = _children->at(i);
      child->removeSubscription(subscription);
    }
  }
  
  if (_subscriptions == NULL && _parent != NULL){
    _parent->pruneChildrenIfPossible();
  }
}

void PyramidNode::notifySubtreeSubscriptors(DEMGrid* grid) const{
  if (_subscriptions != NULL){
    const size_t subscriptionsSize = _subscriptions->size();
    for (size_t i = 0; i < subscriptionsSize; i++) {
      
      //Geenerated best grid response for each subscription
      DEMSubscription* subscription = _subscriptions->at(i);
      DEMGrid* selectedGrid = DEMGridUtils::bestGridFor(grid,
                                                        subscription->_sector,
                                                        subscription->_extent);
      if (selectedGrid != NULL) {
        subscription->onGrid(selectedGrid);
      }
    }
  }
  
  if (_children != NULL){
    for (size_t i = 0; i < _childrenSize; i++) {
      _children->at(i)->notifySubtreeSubscriptors(grid);
    }
  }
}

bool PyramidNode::subtreeHasSubscriptions() const{
  if (_subscriptions != NULL && !_subscriptions->empty()){
    return true;
  } else{
    if (_children != NULL){
      for (size_t i = 0; i < _childrenSize; i++) {
        if (_children->at(i)->subtreeHasSubscriptions()){
          return true;
        }
      }
    }
  }
  return false;
}

void PyramidNode::pruneChildrenIfPossible(){
  if (_children != NULL){
    for (size_t i = 0; i < _childrenSize; i++) {
      if (_children->at(i)->subtreeHasSubscriptions() || _children->at(i)->_stickyGrid){
        return;
      }
    }
    
    for (size_t i = 0; i < _childrenSize; i++) {
      delete _children->at(i);
    }
    delete _children;
    _children = NULL;
    _childrenSize = 0;
  }
}

void PyramidNode::onDEMProviderRemoved(){
  _pyramidDEMProvider = NULL;
  
  if (_children != NULL){
    for (size_t i = 0; i < _childrenSize; i++) {
      _children->at(i)->onDEMProviderRemoved();
    }
  }
}
