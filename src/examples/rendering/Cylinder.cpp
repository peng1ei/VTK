//
// Created by penglei on 18-12-26.
//
#include "vtkCylinderSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkSmartPointer.h"

#include <iostream>

int main() {
    vtkCylinderSource *cylinder = vtkCylinderSource::New();
    cylinder->SetResolution(1024);

    vtkPolyDataMapper *cylinder_mapper = vtkPolyDataMapper::New();
    cylinder_mapper->SetInputConnection(cylinder->GetOutputPort());

    vtkSmartPointer<vtkActor> cylinder_actor = vtkSmartPointer<vtkActor>::New();
    cylinder_actor->SetMapper(cylinder_mapper);
    cylinder_actor->GetProperty()->SetColor(0.0, 0.3882, 0.2784);
    cylinder_actor->RotateX(30.0);
    cylinder_actor->RotateY(-45.0);

    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> render_window =
            vtkSmartPointer<vtkRenderWindow>::New();
    render_window->AddRenderer(render);
    vtkSmartPointer<vtkRenderWindowInteractor> render_window_iteror =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    render_window_iteror->SetRenderWindow(render_window);

    render->AddActor(cylinder_actor);
    render->SetBackground(0.5, 0.2, 0.4);
    render_window->SetSize(400, 300);

    int *size = render_window->GetScreenSize();
    std::cout << size[0] << ", " << size[1] << std::endl;
    render_window->SetPosition(400, 200);

    render->ResetCamera();
    render->GetActiveCamera()->Zoom(1.25);
    render_window->Render();

    render_window_iteror->Start();

    cylinder->Delete();
    cylinder_mapper->Delete();

    return 0;
}
