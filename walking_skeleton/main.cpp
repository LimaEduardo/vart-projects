#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/box.h>
#include <vart/point4d.h>
#include <vart/sphere.h>
#include <vart/meshobject.h>
#include <vart/contrib/viewerglutogl.h>

#include <iostream>
#include <cmath>

using namespace std;
using namespace VART;

const unsigned int MAX_KEYS = 10; // max number of keys to keep control
enum Key { UP, DOWN, RIGHT, LEFT };
bool keyPressed[MAX_KEYS];

// Define the keyboard handler
class KeyboardHandler : public ViewerGlutOGL::KbHandler
{
    public:
        KeyboardHandler() {
            for (unsigned int i = 0; i < MAX_KEYS; ++i)
                keyPressed[i] = false;
        }
        virtual void OnKeyDown(int key) {
            switch (key) {
                case KEY_LEFT:
                    keyPressed[LEFT] = true;
                    break;
                case KEY_RIGHT:
                    keyPressed[RIGHT] = true;
                    break;
                case KEY_UP:
                    keyPressed[UP] = true;
                    break;
                case KEY_DOWN:
                    keyPressed[DOWN] = true;
                    break;
            }
        }
        virtual void OnKeyUp(int key) {
            switch (key) {
                case KEY_LEFT:
                    keyPressed[LEFT] = false;
                    break;
                case KEY_RIGHT:
                    keyPressed[RIGHT] = false;
                    break;
                case KEY_UP:
                    keyPressed[UP] = false;
                    break;
                case KEY_DOWN:
                    keyPressed[DOWN] = false;
                    break;
            }
        }
    private:
};

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:

        Transform* skeletonTranslation;
        Transform* skeletonRotation;

        virtual void OnIdle() {
            if (keyPressed[UP]) {
                xTranslation += sin(yRotation) * speed;
                zTranslation += cos(yRotation) * speed;
                skeletonTranslation->MakeTranslation(xTranslation, yTranslation, zTranslation);
            }
            if (keyPressed[DOWN]) {
                xTranslation += -sin(yRotation) * speed;
                zTranslation += -cos(yRotation) * speed;
                skeletonTranslation->MakeTranslation(xTranslation, yTranslation, zTranslation);
            }
            if (keyPressed[LEFT]) {
                yRotation += M_PI/20;
                skeletonRotation->MakeYRotation(yRotation);
            }
            if (keyPressed[RIGHT]) {
                yRotation -= M_PI/20;
                skeletonRotation->MakeYRotation(yRotation);
            }
            viewerPtr->PostRedisplay();
        }
    protected:
    private:
        double speed = 0.3;
        double xTranslation = 0.0;
        double yTranslation = 8;
        double zTranslation = 0.0;
        double yRotation = 0.0;
};



// The application itself:
int main(int argc, char* argv[])
{
    ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static Scene scene; // create a scene
    static ViewerGlutOGL viewer; // create a viewer (application window)
    // create a camera (scene observer)
    Camera camera(Point4D(60,20,0), Point4D::ORIGIN(), Point4D::Y());

    list<MeshObject*> skeleton;
    MeshObject::ReadFromOBJ(argv[1], &skeleton);
    list<MeshObject*>::iterator it = skeleton.begin();

    Transform rotateSkeleton;

    for (; it != skeleton.end(); ++it){
        rotateSkeleton.AddChild((**it));
    }
    
    rotateSkeleton.MakeXRotation(-M_PI/2);

    Transform movingSkeletonRotation;
    movingSkeletonRotation.MakeXRotation(0);
    movingSkeletonRotation.AddChild(rotateSkeleton);


    Transform movingSkeletonTransform;
    movingSkeletonTransform.MakeTranslation(0,8,0);
    movingSkeletonTransform.AddChild(movingSkeletonRotation);


    MeshObject box;
    box.MakeBox(-60, 60, 0, 1, -60, 60);
    box.SetMaterial(Material::DARK_PLASTIC_GRAY());
    
    Transform boxTranslation;
    boxTranslation.MakeTranslation(0,-2,0);
    boxTranslation.AddChild(box);

    scene.AddObject(&movingSkeletonTransform);
    scene.AddObject(&boxTranslation);
    scene.AddLight(Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

     // Create some objects
    Arrow arrowX(2);
    Arrow arrowY(Point4D::ORIGIN(), Point4D::Y()*2);
    Arrow arrowZ(Point4D::ORIGIN(), Point4D::Z()*2);

    // Initialize scene objects
    arrowX.SetMaterial(Material::PLASTIC_RED());
    arrowY.SetMaterial(Material::PLASTIC_GREEN());
    arrowZ.SetMaterial(Material::PLASTIC_BLUE());

    // Build the scene graph
    scene.AddObject(&arrowX);
    scene.AddObject(&arrowY);
    scene.AddObject(&arrowZ);


    // Set up the viewer
    viewer.SetTitle("Walking Skeleton");
    viewer.SetScene(scene); // attach the scene
    KeyboardHandler kbh; // create a keyboard handler
    viewer.SetKbHandler(&kbh); // attach the keyboard handler
    MyIHClass idh;
    idh.skeletonTranslation = &movingSkeletonTransform;
    idh.skeletonRotation = &movingSkeletonRotation;
    viewer.SetIdleHandler(&idh);

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
