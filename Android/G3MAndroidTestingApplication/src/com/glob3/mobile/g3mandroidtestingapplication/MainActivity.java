

package com.glob3.mobile.g3mandroidtestingapplication;

import org.glob3.mobile.generated.AltitudeMode;
import org.glob3.mobile.generated.Angle;
import org.glob3.mobile.generated.CameraDoubleDragHandler;
import org.glob3.mobile.generated.CameraRenderer;
import org.glob3.mobile.generated.CameraSingleDragHandler;
import org.glob3.mobile.generated.Geodetic2D;
import org.glob3.mobile.generated.Geodetic3D;
import org.glob3.mobile.generated.ILogger;
import org.glob3.mobile.generated.LayerSet;
import org.glob3.mobile.generated.OSMLayer;
import org.glob3.mobile.generated.Planet;
import org.glob3.mobile.generated.SGShape;
import org.glob3.mobile.generated.ShapeLoadListener;
import org.glob3.mobile.generated.ShapesRenderer;
import org.glob3.mobile.generated.TimeInterval;
import org.glob3.mobile.generated.URL;
import org.glob3.mobile.generated.Vector3D;
import org.glob3.mobile.specific.G3MBuilder_Android;
import org.glob3.mobile.specific.G3MWidget_Android;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;


public class MainActivity
         extends
            Activity {

   private G3MWidget_Android _g3mWidget;


   Geodetic2D _merida = Geodetic2D.fromDegrees(38.915736, -6.3571647);
   Geodetic2D _museoMerida = Geodetic2D.fromDegrees(38.917146, -6.339565);
   

   @Override
   protected void onCreate(final Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);

      requestWindowFeature(Window.FEATURE_NO_TITLE);
      getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

      setContentView(R.layout.activity_main);

      //_g3mWidget = createWidgetWithModelOnMerida();
      _g3mWidget = createWidgetWithModelAtCenter();
      
      final RelativeLayout placeHolder = (RelativeLayout) findViewById(R.id.g3mWidgetHolder);

      placeHolder.addView(_g3mWidget);

      
//      _g3mWidget.setAnimatedCameraPosition(new Geodetic3D(_merida, 5000.0), 
//      										TimeInterval.fromSeconds(7.5));
      
      _g3mWidget.getG3MWidget().getPlanetRenderer().setEnable(false);
      
      // Camera position=(lat=38.908400780766385d, lon=-6.345933272891653d, height=664.4377440058383)
      
   }

   private G3MWidget_Android createWidgetWithModelOnMerida() {
      final G3MBuilder_Android builder = new G3MBuilder_Android(this);
      

      final LayerSet layerSet = new LayerSet();
      layerSet.addLayer(new OSMLayer(TimeInterval.fromDays(30)));
      builder.getPlanetRendererBuilder().setLayerSet(layerSet);
      
      ShapesRenderer sr = new ShapesRenderer();
      
      sr.loadJSONSceneJS(new URL(URL.FILE_PROTOCOL + "models/1/1.json"),
              1000,
              TimeInterval.forever(),
              true,
              URL.FILE_PROTOCOL + "models/1/",
              false,
              new Geodetic3D(_museoMerida, 100.0),
              AltitudeMode.ABSOLUTE,
              new ShapeLoadListener(){

				@Override
				public void dispose() {
					// TODO Auto-generated method stub
					
				}

				@Override
				public void onBeforeAddShape(SGShape shape) {
					// TODO Auto-generated method stub
					shape.setScale(10);
					shape.setPitch(Angle.fromDegrees(90));
					ILogger.instance().logError("MODEL ADDED");
				}

				@Override
				public void onAfterAddShape(SGShape shape) {
				}
    	  
      },
              true);
      
      builder.addRenderer(sr);
      
      return builder.createWidget();
   }
   

   private G3MWidget_Android createWidgetWithModelAtCenter() {
      final G3MBuilder_Android builder = new G3MBuilder_Android(this);
      
      Planet planet = Planet.createSphericalEarth();
      builder.setPlanet(planet);
      final double radius = planet.getRadii().maxAxis();

      final LayerSet layerSet = new LayerSet();
      layerSet.addLayer(new OSMLayer(TimeInterval.fromDays(30)));
      builder.getPlanetRendererBuilder().setLayerSet(layerSet);
      
      Geodetic3D g = Geodetic3D.fromDegrees(0, 0, -radius);
      
      ShapesRenderer sr = new ShapesRenderer();
      
      sr.loadJSONSceneJS(new URL(URL.FILE_PROTOCOL + "models/1/1.json"),
              1000,
              TimeInterval.forever(),
              true,
              URL.FILE_PROTOCOL + "models/1/",
              false,
              g,
              AltitudeMode.ABSOLUTE,
              new ShapeLoadListener(){

				@Override
				public void dispose() {
					// TODO Auto-generated method stub
					
				}

				@Override
				public void onBeforeAddShape(SGShape shape) {
					double modelSize = shape.getMax().distanceTo(shape.getMin());
					shape.setScale(radius*2 / modelSize);
					shape.setPitch(Angle.fromDegrees(90));

					shape.setTranslation(shape.getCenter().times(-1));
					Vector3D c = shape.getCenter();

					ILogger.instance().logError("MODEL ADDED AND CENTERED %s", c.description());
				}

				@Override
				public void onAfterAddShape(SGShape shape) {
				}
    	  
      },
              true);
      
      builder.addRenderer(sr);
      
      //Controlling model with 1 or 2 fingers
      CameraRenderer cameraRenderer = new CameraRenderer();
      boolean useInertia = true;
      cameraRenderer.addHandler(new CameraSingleDragHandler(useInertia));
      cameraRenderer.addHandler(new CameraDoubleDragHandler());
      builder.setCameraRenderer(cameraRenderer);
      
      return builder.createWidget();
   }
}
