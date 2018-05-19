#include <vart/scene.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/mesh.h>
#include <vart/arrow.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>

using namespace std;
using namespace VART;

class Terrain: public VART::MeshObject {
  public:
    Terrain(){
      
      std::vector<double> vertices;
      int lines = 20;
      int columns = 20;
      
      for(int line = 0; line < lines; line++){
        for(int column = 0; column < columns; column++){
          cout << line << column << endl;
          vertices.push_back(line);
          vertices.push_back(column);
          vertices.push_back(randomHeight());
        }
      }

      vertCoordVec = vertices;

      for (int i =0; i < lines - 1; i++){
        vector<unsigned int> f1;
        for (int j = 0; j < columns; j++){
          f1.push_back(i * columns + j);
          f1.push_back((i+1) * columns + j);
        }

        Mesh floor;
        floor.type = Mesh::TRIANGLE_STRIP;
        floor.material = Material::PLASTIC_GREEN();
        floor.indexVec = f1;

        meshList.push_back(floor);
      }

      ComputeVertexNormals();
      ComputeBoundingBox();
      ComputeRecursiveBoundingBox();
    }

    double randomHeight(){
      int random = rand();
      return (random % (int)(0.5 * tan(M_PI/6)*101 ) / 100.0);
    }
};

int main(int argc, char* argv[]){
  VART::ViewerGlutOGL::Init(&argc, argv);

  static VART::Scene scene;
  static VART::ViewerGlutOGL viewer;

  VART::Camera camera(VART::Point4D(10,2,2), VART::Point4D(2,2,2), VART::Point4D::Z());
  camera.SetFarPlaneDistance(50.0);	

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


  scene.AddLight(VART::Light::BRIGHT_AMBIENT());
  scene.AddCamera(&camera);

  Terrain terrain;
  scene.AddObject(&terrain);

  viewer.SetTitle("Terrain");
  viewer.SetScene(scene);

  scene.DrawLightsOGL();
  VART::ViewerGlutOGL::MainLoop();

  return 0;

}