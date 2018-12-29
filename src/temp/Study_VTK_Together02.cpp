//
// Created by penglei on 18-12-27.
//

#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>

int main() {
    vtkConeSource *cone = vtkConeSource::New();
    cone->SetResolution(8);
    cone->SetHeight(10);
    cone->SetRadius(2);

    vtkPolyDataMapper *cone_mapper = vtkPolyDataMapper::New();
    cone_mapper->SetInputConnection(cone->GetOutputPort());

    vtkSmartPointer<vtkActor> cone_actor =
            vtkSmartPointer<vtkActor>::New();
    cone_actor->SetMapper(cone_mapper);

    vtkRenderer *renderer = vtkRenderer::New();
    renderer->AddActor(cone_actor);
    renderer->SetBackground(0.0, 0.0, 0.4);

    vtkRenderWindow *ren_win = vtkRenderWindow::New();
    ren_win->SetSize(400, 300);
    ren_win->SetPosition(400, 100);
    ren_win->AddRenderer(renderer);

//    for (int i = 0; i < 360000; ++i) {
//        ren_win->Render();
//        renderer->GetActiveCamera()->Azimuth(1);
//    }
    ren_win->Render();
    std::cin.get();

    cone->Delete();
    cone_mapper->Delete();
    renderer->Delete();
    ren_win->Delete();

    return 0;
}

