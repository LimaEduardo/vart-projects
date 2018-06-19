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

    virtual void OnIdle(){
      earthSelfAxisRotationRadians += 0.01;
      moonSelfAxisRotationRadians += 0.01;
      sunSelfAxisRotationRadians += 0.01;

      earthSelfAxisRotation->MakeYRotation(earthSelfAxisRotationRadians);
      moonSelfAxisRotation->MakeYRotation(moonSelfAxisRotationRadians);
      sunSelfAxisRotation->MakeYRotation(sunSelfAxisRotationRadians);

      viewerPtr->PostRedisplay();

    }

  private:
    double earthSelfAxisRotationRadians;
    double moonSelfAxisRotationRadians;
    double sunSelfAxisRotationRadians;



};

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
  VART::Transform sunRotationAroundSun;

  VART::Sphere moon;
  VART::Transform moonScale;
  VART::Transform moonTranslation;
  VART::Transform moonSelfRotation;
  VART::Transform moonRotationAroundSun;

  VART::Texture texture;
  VART::Material material;

  if(texture.LoadFromFile("lua.jpg")){
    material.SetTexture(texture);
    moon.SetMaterial(material);
    
    moonScale.MakeScale(1,1,1);
    moonTranslation.MakeTranslation(16,0,0);

    moonScale.AddChild(moon);
    moonSelfRotation.AddChild(moonScale);
    moonTranslation.AddChild(moonSelfRotation);

  } else {
    cerr << "missing moon texture" << endl;
    return 0;
  }

  if(texture.LoadFromFile("terra.jpg")){
    material.SetTexture(texture);
    earth.SetMaterial(material);
    
    earthScale.MakeScale(2,2,2);
    earthTranslation.MakeTranslation(12,0,0);


    earthScale.AddChild(earth);
    earthSelfRotation.AddChild(earthScale);
    earthTranslation.AddChild(earthSelfRotation);
    
  } else {
    cerr << "missing earth texture" << endl;
    return 0;
  }

  if(texture.LoadFromFile("sol.jpg")){
    material.SetTexture(texture);
    sun.SetMaterial(material);
    
    sunScale.MakeScale(4,4,4);
    sunTranslation.MakeTranslation(0,0,0);

    sunScale.AddChild(sun);
    sunSelfRotation.AddChild(sunScale);
    sunTranslation.AddChild(sunSelfRotation);

  } else {
    cerr << "missing sun texture" << endl;
    return 0;
  }

  scene.AddObject(&moonTranslation);
  scene.AddObject(&earthTranslation);
  scene.AddObject(&sunTranslation);

  IdleHandler idleHandler;
  idleHandler.sunSelfAxisRotation = &sunSelfRotation;
  idleHandler.moonSelfAxisRotation = &moonSelfRotation;
  idleHandler.earthSelfAxisRotation = &earthSelfRotation;

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