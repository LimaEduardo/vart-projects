// Esse código é a implementação de uma roda gigante usando o framework V-ART

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/camera.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/meshobject.h>

#include <iostream>
#include <cmath>

using namespace std;

// Aqui definimos algumas variáveis que serão usadas em pontos diferentes do código.

// Aqui definimos a quantidade de cadeiras (baseado no modelo que foi oferecido)
#define NUMBER_OF_CHAIRS 10
// O angulo das cadeiras deve ser definido em radianos
// A circunferência da roda é de 2pi. Se queremos dividir a roda em 10 partes,
// devemos fazer a conta 2pi/10 que é igual a pi/5
#define ANGLE_BETWEEN_CHAIRS M_PI/5
// Angulo da roda que foi dado no exercício.
#define WHEEL_RADIUS 70


// Essa classe é extendida de IdleHandler do VART.
// Ela vai ser responsável por constantemente atualizar a rotação
// e a translação das cadeiras.
class IdleHandlerClass : public VART::ViewerGlutOGL::IdleHandler {
  public:
    // Essa transformação é responsável pela rotação da roda principal.
    VART::Transform* wheelRotation;
    // Este é um vetor com as transformações que serão aplicadas na cadeira.
    vector<VART::Transform*> chairsTranslation;

    virtual void OnIdle(){
      // Esse angulo foi definido de forma geral para evitar redundância, 
      // visto que a cadeira e a roda devem estar na mesma 'velocidade'
      angleInRadians += 0.01;
      // Aplicamos a rotação com o ângulo aumentado
      wheelRotation->MakeXRotation(angleInRadians);

      // Aqui cuidaremos da translação da cadeira...
      angleChair = 0.0;
      // Para a quantidade de cadeiras definidas, fazemos::
      for(int i = 0; i < NUMBER_OF_CHAIRS; i++){
        // Calculamos os pontos Y e Z que iremos fazer a translação
        double pointY = WHEEL_RADIUS * sin(angleChair + angleInRadians);
        double pointZ = -WHEEL_RADIUS * cos(angleChair + angleInRadians);
        // Aplicamos a translação para a transformação na posição atual do vetor.
        chairsTranslation[i]->MakeTranslation(0, pointY, pointZ);
        // Aumentamos o angulo da cadeira para aplicar transformação na "próxima cadiera"
        angleChair += ANGLE_BETWEEN_CHAIRS;
      }
      // Atualizamos o display
      viewerPtr->PostRedisplay();
    }

  private:
    double angleInRadians;
    double angleChair;
};

// Essa função é responsável por criar um vector de transformações e aplicar
// cada transformação a cadiera.
vector<VART::Transform*> createChairsTransforms(VART::MeshObject* chair){
  // Cria o vetor de transformações;
  vector<VART::Transform*> chairTransforms;
  // Recebe o angulo das cadeiras (para ir para a "primeira cadeira")
  double actualAngle = ANGLE_BETWEEN_CHAIRS;
  for(int i = 0; i < NUMBER_OF_CHAIRS; i++){
    // Cria uma nova transformação
    VART::Transform* transform = new VART::Transform();
    transform->MakeTranslation(0, WHEEL_RADIUS * sin(actualAngle), -WHEEL_RADIUS * cos(actualAngle));
    //Adiciona uma cópia da cadeira como filha de transform;
    transform->AddChild(*(dynamic_cast<VART::MeshObject*>(chair->Copy())));
    // Adiciona transform no vetor de transformações
    chairTransforms.push_back(transform);
    //"Anda com o angulo para a próxima cadeira"
    actualAngle += ANGLE_BETWEEN_CHAIRS;
  }

  return chairTransforms;
}


int main(int argc, char* argv[]){
  if (argc < 2){
    cout << "Missing arguments" << endl;
    return 0;
  }

  //Iniciar o viewer glut
  VART::ViewerGlutOGL::Init(&argc,argv);
  static VART::ViewerGlutOGL viewer;

  // Cria uma lista com os objetos da cena e lê do arquivo.
  // A lista é composta por uma cadeira, uma roda e um suporte.
  list<VART::MeshObject*> sceneObjects;
  VART::MeshObject::ReadFromOBJ(argv[1], &sceneObjects);

  // Criamos um mesh object para cada objeto do arquivo
  VART::MeshObject* wheel;
  VART::MeshObject* support;
  VART::MeshObject* chair;

  // Iterator para percorrer a lista.
  list<VART::MeshObject*>::iterator it;

  // Verificamos cada item. Pra diferenciar os itens, usamos a função GetDesciption
  for(it = sceneObjects.begin(); it != sceneObjects.end(); ++it){
    if ( (*it)->GetDescription() == "support"){
      support = *it;
    } else if ( (*it)->GetDescription() == "wheel"){
      wheel = *it;
    } else if ( (*it)->GetDescription() == "chair"){
      chair = *it;
    }
  }

  // Vetor que ira receber as transformações e colocar elas "em seu lugar"
  vector<VART::Transform*> chairTransforms;
  chairTransforms = createChairsTransforms(chair);
  
  // Transformação que vai fazer a roda girar em torno do próprio eixo.
  VART::Transform wheelRotation;
  wheelRotation.AddChild(*wheel);

  // Cria uma instancia de idle handler e atribui as transformações
  // que deverão mudar com o tempo.
  IdleHandlerClass idleHandler;
  idleHandler.wheelRotation = &wheelRotation;
  idleHandler.chairsTranslation = chairTransforms;

  // Instancia a cena.
  static VART::Scene scene;

  // Adiciona diretamente o suporte. Ele não sofre transformação
  scene.AddObject(support);
  // Adicionar a rotação da roda principal.
  scene.AddObject(&wheelRotation);
  // Adiciona cada transformação (que já possuí a cadeira como filha) na cena.
  for (int i = 0; i < NUMBER_OF_CHAIRS; i++){
    scene.AddObject(chairTransforms[i]);
  }

  // Adiciona a camera bem longe, pois o objeto é grande (Evita de mexer na escala do objeto)
  VART::Camera camera(VART::Point4D(200,0,0), VART::Point4D::ORIGIN(), VART::Point4D::Y());
  // Pra ver mais longe
  camera.SetFarPlaneDistance(300.0f);

  // Adiciona luz clara.
  scene.AddLight(VART::Light::BRIGHT_AMBIENT());
  // Adiciona camera.
  scene.AddCamera(&camera);

  // Seta title, cena e idle handler para o viewerglut
  viewer.SetTitle("My new ferris-wheel");
  viewer.SetScene(scene);
  viewer.SetIdleHandler(&idleHandler);

  scene.DrawLightsOGL();
  VART::ViewerGlutOGL::MainLoop();

  return 0;

}

