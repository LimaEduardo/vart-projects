#include <vart/scene.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>

using namespace std;
using namespace VART;

// Define the click handler
class ClickHandlerClass : public VART::MouseControl::ClickHandler {
    public:
        ClickHandlerClass() {
        }
        virtual ~ClickHandlerClass() {};
        virtual void OnClick() {
            if (mouseCtrlPtr->LastClickIsDown()) {
            }
        }
};

class Pyramid : public VART::MeshObject {
  public:
    Pyramid(){
      double vertices[] = {
        0,0,0,
        4,0,0,
        4,4,0,
        0,4,0,
        2,2,2
      };
      cout << vertices << endl;
      double* endVertices = vertices + sizeof(vertices)/sizeof(double);

      double baseVertices[] = {0,3,2,1};
      double* endBaseVertices = baseVertices + sizeof(baseVertices)/sizeof(double);

      double trianguleFaces[] = {
        0,1,4,
        1,2,4,
        2,3,4,
        3,0,4
      };
      double* endTriangleFaces = trianguleFaces + sizeof(trianguleFaces)/(sizeof(double));


      //Create pytramid base
      Mesh baseMesh;
      vertCoordVec.assign(vertices,endVertices);
      baseMesh.type = Mesh::QUADS;
      baseMesh.indexVec.assign(baseVertices, endBaseVertices);
      baseMesh.material = Material::PLASTIC_RED();
      meshList.push_back(baseMesh);

      //Create sides of pyramid
      Mesh triangles;
      triangles.type = Mesh::TRIANGLES;
      triangles.indexVec.assign(trianguleFaces, endTriangleFaces);
      triangles.material = Material::PLASTIC_BLUE();
      meshList.push_back(triangles);

      ComputeVertexNormals();
      ComputeBoundingBox();
      ComputeRecursiveBoundingBox();
    }
};

// The application itself:
int main(int argc, char* argv[]){
  VART::ViewerGlutOGL::Init(&argc, argv);
  static VART::Scene scene;
  static VART::ViewerGlutOGL viewer;

  VART::Camera camera(VART::Point4D(10,6,0),VART::Point4D::ORIGIN(),VART::Point4D::Z());

  Pyramid pyramid;

  scene.AddObject(&pyramid);

  scene.AddLight(VART::Light::BRIGHT_AMBIENT());
  scene.AddCamera(&camera);

  viewer.SetTitle("Pyramid example");
  viewer.SetScene(scene);

  scene.DrawLightsOGL();
  VART::ViewerGlutOGL::MainLoop();

  return 0;
}



    // VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    // static VART::Scene scene; // create a scene
    // static VART::ViewerGlutOGL viewer; // create a viewer (application window)
    // // create a camera (scene observer)
    // VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D::ORIGIN(),VART::Point4D::Y());

    // // Create some objects
    // VART::Arrow arrowX(2);
    // VART::Arrow arrowY(VART::Point4D::ORIGIN(), VART::Point4D::Y()*2);
    // VART::Arrow arrowZ(VART::Point4D::ORIGIN(), VART::Point4D::Z()*2);

    // // Initialize scene objects
    // arrowX.SetMaterial(VART::Material::PLASTIC_RED());
    // arrowY.SetMaterial(VART::Material::PLASTIC_GREEN());
    // arrowZ.SetMaterial(VART::Material::PLASTIC_BLUE());

    // // Build the scene graph
    // scene.AddObject(&arrowX);
    // scene.AddObject(&arrowY);
    // scene.AddObject(&arrowZ);

    // // Add lights and cameras to the scene
    // scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    // scene.AddCamera(&camera);

    // // Set up the viewer
    // viewer.SetTitle("V-ART arrow example");
    // viewer.SetScene(scene); // attach the scene

    // // Run application
    // scene.DrawLightsOGL(); // Set OpenGL's lights' state
    // VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
    // return 0;
// }
