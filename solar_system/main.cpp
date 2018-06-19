#include <vart/scene.h>
#include <vart/arrow.h>
#include <vart/sphere.h>
#include <vart/texture.h>
#include <vart/transform.h>
#include <vart/contrib/viewerglutogl.h>

#include <iostream>

using namespace std;

class IdleHandler : public VART::ViewerGlutOGL::IdleHandler {
  public:
    VART::Transform* earthSelfAxisRotation = new VART::Transform();
    VART::Transform* moonSelfAxisRotation = new VART::Transform();
    VART::Transform* sunSelfAxisRotation = new VART::Transform();

    VART::Transform* earthRotationAroundSun = new VART::Transform();
    VART::Transform* moonRotationAroundEarth = new VART::Transform();


    virtual void OnIdle(){
      earthSelfAxisRotationRadians += 0.02;
      moonSelfAxisRotationRadians += 3;
      sunSelfAxisRotationRadians += 0.01;

      earthSelfAxisRotation->MakeYRotation(earthSelfAxisRotationRadians);
      moonSelfAxisRotation->MakeYRotation(moonSelfAxisRotationRadians);
      sunSelfAxisRotation->MakeYRotation(sunSelfAxisRotationRadians);

      earthRotationAroundSunRadians += 0.01;
      earthRotationAroundSun->MakeRotation(VART::Point4D(0,0,0), VART::Point4D::Y(), earthRotationAroundSunRadians);

      moonRotationAroundEarthRadians += 0.01;
      moonRotationAroundEarth->MakeRotation(VART::Point4D(0,0,0), VART::Point4D::Y(), moonRotationAroundEarthRadians);


      

      viewerPtr->PostRedisplay();

    }

  private:
    double earthSelfAxisRotationRadians;
    double moonSelfAxisRotationRadians;
    double sunSelfAxisRotationRadians;

    double earthRotationAroundSunRadians;
    double moonRotationAroundEarthRadians;
};

// 1)Fazer girar a lua em torno da Terra

int main(int argc, char* argv[]) {
  VART::ViewerGlutOGL::Init(&argc, argv);
  
  static VART::ViewerGlutOGL viewer;
  static VART::Scene scene;

  VART::Sphere earth;
  VART::Transform earthScale;
  VART::Transform earthTranslation;
  VART::Transform earthSelfRotation;
  VART::Transform earthRotationAroundSun;

  VART::Sphere sun;
  VART::Transform sunScale;
  VART::Transform sunTranslation;
  VART::Transform sunSelfRotation;

  VART::Sphere moon;
  
  VART::Transform moonScale;
  VART::Transform moonTranslation;
  VART::Transform moonSelfRotation;
  VART::Transform moonRotationAroundEarth;

  VART::Transform moonAndEarthTranslation;

  VART::Texture texture;
  VART::Material material;

  if(texture.LoadFromFile("lua.jpg")){
    material.SetTexture(texture);
    moon.SetMaterial(material);
    
    // moonScale.MakeScale(1,1,1);
    // moonTranslation.MakeTranslation(16,0,0);

    // moonScale.AddChild(moon);
    // moonSelfRotation.AddChild(moonScale);
    // moonTranslation.AddChild(moonSelfRotation);
    // moonSelfRotation.AddChild(moonScale);

  } else {
    cerr << "missing moon texture" << endl;
    return 0;
  }

  if(texture.LoadFromFile("terra.jpg")){
    material.SetTexture(texture);
    earth.SetMaterial(material);
    
    // earthScale.MakeScale(2,2,2);
    // earthTranslation.MakeTranslation(12,0,0);


    // earthScale.AddChild(earth);
    // earthSelfRotation.AddChild(earthScale);
    // earthTranslation.AddChild(earthSelfRotation);
    // earthRotationAroundSun.AddChild(earthTranslation);
    
  } else {
    cerr << "missing earth texture" << endl;
    return 0;
  }

  if(texture.LoadFromFile("sol.jpg")){
    material.SetTexture(texture);
    sun.SetMaterial(material);
    
    // sunScale.MakeScale(4,4,4);
    // sunTranslation.MakeTranslation(0,0,0);

    // sunScale.AddChild(sun);
    // sunSelfRotation.AddChild(sunScale);
    // sunTranslation.AddChild(sunSelfRotation);

  } else {
    cerr << "missing sun texture" << endl;
    return 0;
  }

  earthScale.MakeScale(2,2,2);
  
  moonScale.MakeScale(1,1,1);
  moonTranslation.MakeTranslation(6,0,0);

  moonAndEarthTranslation.MakeTranslation(16,0,0);

  sunScale.MakeScale(6,6,6);
  sunTranslation.MakeTranslation(0,0,0);



  earthScale.AddChild(earth);
  earthSelfRotation.AddChild(earthScale);
  
  moonScale.AddChild(moon);
  moonSelfRotation.AddChild(moonScale);
  moonTranslation.AddChild(moonSelfRotation);
  moonRotationAroundEarth.AddChild(moonTranslation);

  sunScale.AddChild(sun);
  sunTranslation.AddChild(sunScale);
  sunSelfRotation.AddChild(sunTranslation);

  moonAndEarthTranslation.AddChild(moonRotationAroundEarth);
  moonAndEarthTranslation.AddChild(earthSelfRotation);

  earthRotationAroundSun.AddChild(moonAndEarthTranslation);
  earthRotationAroundSun.AddChild(sunSelfRotation);
  
  scene.AddObject(&earthRotationAroundSun);


  IdleHandler idleHandler;
  idleHandler.sunSelfAxisRotation = &sunSelfRotation;
  idleHandler.moonSelfAxisRotation = &moonSelfRotation;
  idleHandler.earthSelfAxisRotation = &earthSelfRotation;

  idleHandler.earthRotationAroundSun = &earthRotationAroundSun;
  idleHandler.moonRotationAroundEarth = &moonRotationAroundEarth;

  // Create some objects
  VART::Arrow arrowX(2);
  VART::Arrow arrowY(VART::Point4D::ORIGIN(), VART::Point4D::Y()*2);
  VART::Arrow arrowZ(VART::Point4D::ORIGIN(), VART::Point4D::Z()*2);

  // Initialize scene objects
  arrowX.SetMaterial(VART::Material::PLASTIC_RED());
  arrowY.SetMaterial(VART::Material::PLASTIC_GREEN());
  arrowZ.SetMaterial(VART::Material::PLASTIC_BLUE());

  // Build the scene graph
  scene.AddObject(&arrowX);
  scene.AddObject(&arrowY);
  scene.AddObject(&arrowZ);


  //Add a camera to a scene and configure it.
  VART::Camera camera (VART::Point4D(0,0,30), VART::Point4D::ORIGIN(), VART::Point4D::Y());
  camera.SetDescription( "camera" );
  scene.AddCamera( &camera );

  //Add a light to the scene.
  scene.AddLight( VART::Light::BRIGHT_AMBIENT() );
  scene.DrawLightsOGL();

  viewer.SetTitle("Solar System");
  viewer.SetScene(scene);
  viewer.SetIdleHandler(&idleHandler);


  //Enable texture in OpenGL
  glEnable( GL_TEXTURE_2D );
  glEnableClientState( GL_TEXTURE_COORD_ARRAY );

  VART::ViewerGlutOGL::MainLoop();


  return 0;
}