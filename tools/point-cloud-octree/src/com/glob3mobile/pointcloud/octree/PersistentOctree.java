

package com.glob3mobile.pointcloud.octree;

import java.util.List;


public interface PersistentOctree
         extends
            AutoCloseable {


   public interface Node {
      @Override
      String toString();


      String getID();


      int getPointsCount();


      List<Geodetic3D> getPoints();


      Geodetic3D getAveragePoint();


      Sector getSector();


      int getLevel();
   }


   public interface Visitor {
      void start();


      boolean visit(PersistentOctree.Node node);


      void stop();
   }


   public interface Statistics {
      void show();


      long getPointsCount();


      Sector getSector();


      double getMinHeigth();


      double getMaxHeigth();


      int getMinPointsPerNode();


      int getMaxPointsPerNode();
   }


   void addPoint(Geodetic3D point);


   void optimize();


   void flush();


   @Override
   void close();


   void acceptDepthFirstVisitor(PersistentOctree.Visitor visitor);


   PersistentOctree.Statistics getStatistics(boolean fast,
                                             boolean showProgress);


   String getCloudName();

}