#include <vart/scene.h>
#include <vart/light.h>
#include <vart/camera.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/meshobject.h>

#include <iostream>
#include <cmath>

using namespace std;


int main(int argc, char* argv[]){
  if (argc < 2){
    cout << "Insert the object filename";
  }

  VART::ViewerGlutOGL::Init(&argc,argv);
  static VART::ViewerGlutOGL viewer;

  list<VART::MeshObject*> sceneObjects;
  VART::MeshObject::ReadFromOBJ(argv[1], &sceneObjects);

  VART::MeshObject* wheel;
  VART::MeshObject* support;
  VART::MeshObject* chair;

  list<VART::MeshObject*>::iterator it;

  for(it = sceneObjects.begin(); it != sceneObjects.end(); ++it){
    if ( (*it)->GetDescription() == "support"){
      support = *it;
    } else if ( (*it)->GetDescription() == "wheel"){
      wheel = *it;
    } else if ( (*it)->GetDescription() == "chair"){
      chair = *it;
    }
  }

  static VART::Scene scene;

  scene.AddObject(support);
  scene.AddObject(wheel);
  scene.AddObject(chair);

  VART::Camera camera(VART::Point4D(200,0,0), VART::Point4D::ORIGIN(), VART::Point4D::Y());
  camera.SetFarPlaneDistance(300.0f);

  scene.AddLight(VART::Light::BRIGHT_AMBIENT());
  scene.AddCamera(&camera);

  viewer.SetTitle("My new ferris-wheel");
  viewer.SetScene(scene);

  scene.DrawLightsOGL();
  VART::ViewerGlutOGL::MainLoop();

  return 0;

}

