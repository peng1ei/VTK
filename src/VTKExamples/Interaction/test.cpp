//
// Created by penglei on 18-12-29.
//
// 坐标转换

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPointPicker.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkPropPicker.h>
#include <vtkCoordinate.h>
#include <vtkGeoProjection.h>

/*
// 坐标转换：Display -> World
double* DisplayToWorldCoordinate(int display_x, int display_y, vtkViewport *viewport) {
    //assert(viewport != nullptr);

    vtkSmartPointer<vtkCoordinate> coordinate =
            vtkSmartPointer<vtkCoordinate>::New();
    coordinate->SetCoordinateSystemToDisplay();
    coordinate->SetValue(display_x, display_y, 0);

    return coordinate->GetComputedWorldValue(viewport);
}

int* WorldToDisplayCoordinate(double world_x, double world_y, vtkViewport *viewport) {
    //assert(viewport != nullptr);

    vtkSmartPointer<vtkCoordinate> coordinate =
            vtkSmartPointer<vtkCoordinate>::New();
    coordinate->SetCoordinateSystemToWorld();
    coordinate->SetValue(world_x, world_y, 0);

    return coordinate->GetComputedDisplayValue(viewport);
}
*/

// Define interaction style
class customMouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static customMouseInteractorStyle* New();
vtkTypeMacro(customMouseInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown()
    {
        std::cout << "Pressed left mouse button." << std::endl;

        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    virtual void OnMiddleButtonDown()
    {
        std::cout << "Pressed middle mouse button." << std::endl;
        // Forward events
        vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
    }

    virtual void OnRightButtonDown()
    {
        std::cout << "Pressed right mouse button." << std::endl;

        // Forward events
        vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }

    virtual void OnMouseMove() {
        // 获取并激活发生该事件的 Renderer，因为一个 RenderWindow 中可以有多个视口，即多个 Renderers
        int* move_pos = this->Interactor->GetEventPosition();
        this->FindPokedRenderer(move_pos[0], move_pos[1]);
        if (this->CurrentRenderer == nullptr) {
            return;
        }

        std::cout << "[Mouse Move] Display pos: " << move_pos[0] << " " << move_pos[1] << std::endl;

        double *world = DisplayToWorldCoordinate(move_pos[0], move_pos[1]);
        std::cout << "[Mouse Move] World pos: " << world[0] << " " << world[1] << std::endl;

        double *cammera_pos = this->CurrentRenderer->GetActiveCamera()->GetPosition();
        std::cout << "[Camera] pos: " << cammera_pos[0] << " " << cammera_pos[1] << std::endl;

        // 获取当前 Viewport 的中心位置和大小
        int *origin = this->CurrentRenderer->GetOrigin();
        int *size = this->CurrentRenderer->GetSize();
        std::cout << "[Viewport] origin: " << origin[0] << " " << origin[1] << std::endl;
        std::cout << "[Viewport] size: " << size[0] << " " << size[1] << std::endl;


        double *center_display_pos = this->CurrentRenderer->GetCenter();
        std::cout << "[Center] pos: " << center_display_pos[0] << " " << center_display_pos[1] << std::endl;



        UpdateViewportWorldBound();
        std::cout << "-------------------------------------------------------\n" << std::endl;

        // Forward events
        vtkInteractorStyleTrackballCamera::OnMouseMove();
    }

    double* DisplayToWorldCoordinate(double display_x, double display_y) {
        this->CurrentRenderer->SetDisplayPoint(display_x, display_y, 0);
        this->CurrentRenderer->DisplayToWorld();
        return this->CurrentRenderer->GetWorldPoint();
    }

    double* WorldToDisplayCoordinate(double world_x, int world_y) {
        this->CurrentRenderer->SetWorldPoint(world_x, world_y, 0, 0);
        this->CurrentRenderer->WorldToDisplay();
        return this->CurrentRenderer->GetDisplayPoint();
    }

    void UpdateViewportWorldBound() {
        if (this->CurrentRenderer == nullptr) {
            return;
        }

        int *origin = this->CurrentRenderer->GetOrigin();
        double *bottom_left = DisplayToWorldCoordinate(origin[0], origin[1]);
        current_viewport_world_bound_[0] = bottom_left[0];
        current_viewport_world_bound_[1] = bottom_left[1];

        int *size = this->CurrentRenderer->GetSize();
        double *top_right = DisplayToWorldCoordinate(size[0], size[1]);
        current_viewport_world_bound_[2] = top_right[0];
        current_viewport_world_bound_[3] = top_right[1];

        std::cout << "[Current Viewport World Bound]: (" <<
                  current_viewport_world_bound_[0] << ", " <<
                  current_viewport_world_bound_[1] << ", " <<
                  current_viewport_world_bound_[2] << ", " <<
                  current_viewport_world_bound_[3] << ")"
                  << std::endl;
    }

protected:
    // (xmin, ymin, xmax, ymax)
    double current_viewport_world_bound_[4];
    double center_world[2];
};

vtkStandardNewMacro(customMouseInteractorStyle);

int main(int, char *[])
{

    vtkGeoProjection *proj = vtkGeoProjection::New();
    std::cout << "number : "<< proj->GetNumberOfProjections() << std::endl;
    for (int i = 0; i < proj->GetNumberOfProjections(); i++) {
        std::cout << proj->GetProjectionName(i) << std::endl;
        std::cout << proj->GetProjectionDescription(i) << std::endl << std::endl;
    }


    vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(5.0, 0.0, 0.0);
    sphereSource->SetRadius(5.0);
    sphereSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphereSource->GetOutputPort());

    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);


    vtkSmartPointer<vtkNamedColors> colors =
            vtkSmartPointer<vtkNamedColors>::New();

    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(colors->GetColor3d("Slate_grey").GetData());
    renderer->AddActor(actor);
    renderer->SetBackground(0.2,0.3,0.3);

    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    //renderWindow->Render();

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow ( renderWindow );

    renderer->GetActiveCamera()->SetParallelProjection(true);


    vtkSmartPointer<customMouseInteractorStyle> style =
            vtkSmartPointer<customMouseInteractorStyle>::New();
    renderWindowInteractor->SetInteractorStyle( style );

    renderWindowInteractor->Initialize();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}