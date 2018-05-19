#include <vart/scene.h>
#include <vart/light.h>
#include <vart/cylinder.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/transform.h>
#include <vart/mesh.h>
#include <vart/contrib/viewerglutogl.h>

#include <iostream>


using namespace std;
using namespace VART;

Material COLLOR_BROWN(Color(80,20,20));

class Foot : public MeshObject {
  private:
    Cylinder* foot;
    double height;
  public:

    Transform* translation;

    Foot(){
      this->height = 4;
      foot = new Cylinder(this->height, 0.3);
      // Material COLLOR_BROWN();
      this->foot->SetMaterial(COLLOR_BROWN);
      this->foot->SetPartsVisibility(Cylinder::ALL);
      
      translation = new Transform();
      translation->AddChild(*foot);

    }

    Cylinder* getInstance(){
      return this->foot;
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

    VART::Cylinder cylinder(2,0.3);
    cylinder.SetMaterial(VART::Material::PLASTIC_RED());
    VART::Transform transform;
    transform.MakeTranslation(Point4D(0,0,0));
    transform.AddChild(cylinder);

    VART::Transform rotacao;
    rotacao.MakeXRotation(0.785398);
    rotacao.AddChild(transform);

    scene.AddObject(&rotacao);

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

    // Foot foot;
    // foot.translation->MakeTranslation(Point4D(0,0,0));
    // scene.AddObject(&foot);

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
