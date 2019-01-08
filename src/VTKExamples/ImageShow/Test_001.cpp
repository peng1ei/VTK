//
// Created by penglei on 19-1-7.
//
#include <vtkSmartPointer.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkNamedColors.h>
#include <vtkCamera.h>
#include <vtkObjectFactory.h>
#include <vtkImageProperty.h>

void PrintDataExtent(int *extent) {
    std::cout << "Data Extent: " << extent[0] << " " <<
        extent[1] << " " << extent[2] << " " << extent[3] << " " <<
        extent[4] << " " << extent[5] << std::endl;
}

void PrintDataBounds(double *bounds) {
    std::cout << "Data Bounds: " << bounds[0] << " " <<
        bounds[1] << " " << bounds[2] << " " << bounds[3] << " " <<
        bounds[4] << " " << bounds[5] << std::endl;
}

int main() {
    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->GetActiveCamera()->ParallelProjectionOn(); // 开启平行投影

    // the height of the viewport in world-coordinate distances.
    // The default is 1.
    // Note that the "scale" parameter works as an "inverse scale" —
    // larger numbers produce smaller images. This method has no effect
    // in perspective projection mode.
    //renderer->GetActiveCamera()->SetParallelScale(3000);
    renderer->GetActiveCamera()->Print(std::cout);

    /////////////////////////////////////////////////////////////
    //              可视化管道
    /////////////////////////////////////////////////////////////

    vtkSmartPointer<vtkImageReader2Factory> readerFactory =
            vtkSmartPointer<vtkImageReader2Factory>::New();

    /////////////////////////////////////////////////////////////
    // begin linux.jpg
    std::string filename_linux("/home/penglei/图片/jpg/linux.jpg");
    vtkImageReader2 *imageReader = readerFactory->CreateImageReader2(filename_linux.c_str());
    imageReader->SetFileName(filename_linux.c_str());
    imageReader->UpdateInformation();
    //imageReader->SetDataExtent(0, 300, 0, 300, 0, 0);

    vtkSmartPointer<vtkImageActor> actor_linux =
            vtkSmartPointer<vtkImageActor>::New();
    actor_linux->GetMapper()->SetInputConnection(
            imageReader->GetOutputPort());

    actor_linux->GetProperty()->SetInterpolationTypeToNearest();



    renderer->AddActor(actor_linux);
    //renderer->ResetCamera();
    //renderer->ResetCamera(200, 400, 100, 300, 0, 0);

    // In perspective mode, decrease the view angle by the specified factor.
    // In parallel mode, decrease the parallel scale by the specified factor.
    // A value greater than 1 is a zoom-in, a value less than 1 is a zoom-out.
    //renderer->GetActiveCamera()->Zoom(0.5);
    renderer->GetActiveCamera()->SetPosition(110, 73, 1);
    renderer->GetActiveCamera()->SetFocalPoint(110, 73, 0);

    // Distance 必须在 近平面和远平面之间 [zNear, zFar]
    // 对于平行投影，Distance只要在范围之内，多少都不影响投影效果
    //renderer->GetActiveCamera()->SetDistance(20);
    renderer->GetActiveCamera()->SetParallelScale(150);

    //renderer->GetActiveCamera()->Print(std::cout);
    //renderer->GetActiveCamera()->SetParallelProjection(1);

    //actor_linux->Print(std::cout);
    // end linux.jpg
    /////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////
    //              渲染引擎
    /////////////////////////////////////////////////////////////

    // Setup renderer
    vtkSmartPointer<vtkNamedColors> colors =
            vtkSmartPointer<vtkNamedColors>::New();
    renderer->SetBackground(colors->GetColor3d("white").GetData());

    // Setup render window
    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);



    // Setup render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style =
            vtkSmartPointer<vtkInteractorStyleImage>::New();

    renderWindow->SetPosition(renderWindow->GetScreenSize()[0]/2 - 300,
                              renderWindow->GetScreenSize()[1]/2 - 200);
    renderWindow->SetSize(600, 400);
    renderWindowInteractor->SetInteractorStyle(style);

    renderer->GetActiveCamera()->Print(std::cout);

    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->Initialize();

    renderWindowInteractor->Start();

    /////////////////////////////////////////////////////////////
    //              资源释放
    /////////////////////////////////////////////////////////////
    imageReader->Delete();

    return EXIT_SUCCESS;
}
