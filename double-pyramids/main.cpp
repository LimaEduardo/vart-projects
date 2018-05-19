#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>

using namespace std;
using namespace VART;

class Pyramids : public MeshObject{
  public:
    Pyramids(){
      vector<double> vertices({
        0,0,0,
        -1,1,-1,
        1,1,-1,
        1,1,1,
        -1,1,1,
        -1,-1,-1,
        1,-1,-1,
        1,-1,1,
        -1,-1,1,
      });

      vector<unsigned int> base({
        3,2,1,4,
        5,6,7,8
      });

      vector<unsigned int> sides({
        3,4,0,
        4,1,0,
        1,2,0,
        2,3,0,

        7,0,8,
        8,0,5,
        5,0,6,
        6,0,7
      });

      vertCoordVec = vertices;

      VART::Mesh meshBase; 
      meshBase.type = VART::Mesh::QUADS;
      meshBase.indexVec = base;
      meshBase.material = VART::Material::PLASTIC_GREEN();
      meshList.push_back(meshBase);


      VART::Mesh meshSides;
      meshSides.indexVec = sides;
      meshSides.type = VART::Mesh::TRIANGLES;
      meshSides.material = VART::Material::PLASTIC_GREEN();
      meshList.push_back(meshSides);





      ComputeBoundingBox();
      ComputeRecursiveBoundingBox();
      ComputeVertexNormals();

    }
};

// Define the click handler
class ClickHandlerClass : public VART::MouseControl::ClickHandler
{
    public:
        ClickHandlerClass() {
        }
        virtual ~ClickHandlerClass() {};
        virtual void OnClick() {
            if (mouseCtrlPtr->LastClickIsDown()) {
            }
        }
};

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D::ORIGIN(),VART::Point4D::Y());


    Pyramids pyramids;
    scene.AddObject(&pyramids);



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

    

    // Add lights and cameras to the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);


    // Set up the viewer
    viewer.SetTitle("V-ART arrow example");
    viewer.SetScene(scene); // attach the scene

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
    return 0;
}
