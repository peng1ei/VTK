//
// Created by penglei on 18-12-28.
//

#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include <vtkWorldPointPicker.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>

// define interaction style
class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
    static MouseInteractorStyle *New();

vtkTypeMacro(MouseInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown() {
        std::cout << "Picking pixel: " <<
                  this->Interactor->GetEventPosition()[0] << " "
                  << this->Interactor->GetEventPosition()[1] << std::endl << std::endl;

        this->Interactor->GetPicker()->Pick(
                this->Interactor->GetEventPosition()[0],
                this->Interactor->GetEventPosition()[1],
                0, // always zero
                this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()
        );

        double picked[3];
        this->Interactor->GetPicker()->GetPickPosition(picked);
        std::cout << "Picked value: " << picked[0] << " "
                  << picked[1] << " " << picked[2] << std::endl;

        std::cout << "-------------------------------------------\n";

        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

};

vtkStandardNewMacro(MouseInteractorStyle);

int main() {
    vtkSmartPointer<vtkSphereSource> sphere_source =
            vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->Update();

    vtkSmartPointer<vtkWorldPointPicker> world_point_picker =
            vtkSmartPointer<vtkWorldPointPicker>::New();

    // create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphere_source->GetOutputPort());
    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);


    // create a render, render windows, and interactor
    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> render_window =
            vtkSmartPointer<vtkRenderWindow>::New();

    render_window->AddRenderer(renderer);





    vtkSmartPointer<vtkRenderWindowInteractor> render_window_interactor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    render_window_interactor->SetPicker(world_point_picker);
    render_window_interactor->SetRenderWindow(render_window);

    vtkSmartPointer<MouseInteractorStyle> style =
            vtkSmartPointer<MouseInteractorStyle>::New();
    render_window_interactor->SetInteractorStyle(style);

    // Add the actor to the scene
    renderer->AddActor(actor);
    renderer->SetBackground(0, 0, 0.4);

    render_window->SetPosition(100, 100);
    render_window->SetSize(400, 300);

    // render and interact
    render_window->Render();

    int * size = renderer->GetSize();
    std::cout << "Renderer size: " << size[0] << " " << size[1] << std::endl;

    int * orgin = renderer->GetOrigin();
    std::cout << "Renderer orgin: " << orgin[0] << " " << orgin[1] << std::endl;

    render_window_interactor->Start();

    return 0;
}
