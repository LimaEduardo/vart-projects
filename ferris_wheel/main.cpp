// main.cpp - from V-ART template application

// This example shows how to use the Arrow class.

// Changelog
// Oct 19, 2012 - Bruno de Oliveira Schneider
// - Removed the unused keyboard handler.
// - Added more arrows.
// Sep 25, 2008 - Bruno de Oliveira Schneider
// - Application created.

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/transform.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>
#include <cmath>

using namespace std;

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

// Define idle handler
class IndleHandlerClass : public VART::ViewerGlutOGL::IdleHandler{
    public:
        virtual void OnIdle() {
            selfWheelRotationRadians += 0.01;
            selfWheelRotation->MakeXRotation(selfWheelRotationRadians);
            
            double angleChair = 0.0;
            int numberOfChairs = 10;
            int radius = 70;
            double angleBetweenChairs = M_PI/5;
            VART::Transform transform;
            
            list<VART::MeshObject*>::iterator it;
            for (it = chairs.begin(); it != chairs.end(); ++it){
                transform.MakeTranslation(0, radius * sin(angleChair + selfWheelRotationRadians), -radius * cos(angleChair + selfWheelRotationRadians));
                // transform.MakeTranslation(0, sin(angleChair + selfWheelRotationRadians) * radius, -cos(angleChair + selfWheelRotationRadians) * radius);
                (*it)->ApplyTransform(transform);
                angleChair += angleBetweenChairs;
            }

            viewerPtr->PostRedisplay();
        }
    
    VART::Transform* selfWheelRotation;
    list<VART::MeshObject*> chairs;



    private:
        float selfWheelRotationRadians;
        // vector<VART::Transform*> chairsTransform;
};

list<VART::MeshObject*> CreateChairs(VART::MeshObject* chair){
    list<VART::MeshObject*> chairs;
    VART::Transform transform;

    int numberOfChairs = 10;
    int radius = 70;
    double angleBetweenChairs = M_PI/5;

    double actualAngle = angleBetweenChairs;

    for(int i = 0; i < numberOfChairs; i++){
        VART::MeshObject* insertedChair = dynamic_cast<VART::MeshObject*> (chair->Copy());
        transform.MakeTranslation(0, radius * sin(actualAngle), -radius * cos(actualAngle));
        insertedChair->ApplyTransform(transform);
        actualAngle += angleBetweenChairs;
        chairs.push_back(insertedChair);
    }

    transform.MakeTranslation(0, radius * sin(actualAngle), -radius * cos(actualAngle));
    chair->ApplyTransform(transform);
    chairs.push_back(chair);

    return chairs;

}

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT

    if (argc < 2){
        cerr << "No filename given";
        return 0;
    }
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)  
    // create a camera (scene observer)
    list <VART::MeshObject*> objects;
    VART::MeshObject::ReadFromOBJ(argv[1], &objects);


    VART::MeshObject* wheel;
    VART::MeshObject* support;
    list<VART::MeshObject*> chairs;

    VART::BoundingBox boundingBox;

    list<VART::MeshObject*>::iterator it = objects.begin();

    for(; it != objects.end(); ++it){
        if((*it)->GetDescription() == "wheel"){
            wheel = *it;
        } else if ((*it)->GetDescription() == "support"){
            support = *it;
        } else if ((*it)->GetDescription() == "chair"){
            chairs = CreateChairs(*it);
        }

        boundingBox = (*it)->GetBoundingBox();

    }

    // VART::Transform wheelScale;
    // wheelScale.MakeScale(0.01,0.01,0.01);
    // wheelScale.AddChild(*wheel);

    // VART::Transform supportScale;
    // supportScale.MakeScale(0.01,0.01,0.01);
    // supportScale.AddChild(*support);

    VART::Transform wheelRotation;
    wheelRotation.AddChild(*wheel);
    

    scene.AddObject(support);
    scene.AddObject(&wheelRotation);

    it = chairs.begin();

    for(; it != chairs.end(); ++it){
        scene.AddObject(*it);
    }

    IndleHandlerClass idleHandler;
    idleHandler.selfWheelRotation = &wheelRotation;
    idleHandler.chairs = chairs;
    // scene.AddObject(chairs);
    
    VART::Camera camera(VART::Point4D(200,0,0),VART::Point4D::ORIGIN(),VART::Point4D::Y());
    camera.SetFarPlaneDistance(300.0f);

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

    // Add lights and cameras to the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    // Set up the viewer
    viewer.SetTitle("V-ART arrow example");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler);

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
    return 0;
}
