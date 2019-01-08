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
#include "vtkObjectFactory.h"
#include <vtkImageProperty.h>

class customMouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static customMouseInteractorStyle* New();
vtkTypeMacro(customMouseInteractorStyle, vtkInteractorStyleTrackballCamera);


};

vtkStandardNewMacro(customMouseInteractorStyle); // vtkObjectFactory.h



int main(int argc, char *argv[])
{
    // Verify command line arguments
//    if(argc < 2)
//    {
//        std::cout << "Usage: " << argv[0]
//                  << " InputFilename" << std::endl;
//        return EXIT_FAILURE;
//    }

    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();

    // Parse command line arguments
    std::string inputFilename("/home/penglei/图片/jpg/linux.jpg");

    // Read file
    vtkSmartPointer<vtkImageReader2Factory> readerFactory =
            vtkSmartPointer<vtkImageReader2Factory>::New();
    vtkImageReader2 *imageReader = readerFactory->CreateImageReader2(inputFilename.c_str());
    imageReader->SetFileName(inputFilename.c_str());
    imageReader->Update();

    // Create an actor
    vtkSmartPointer<vtkImageActor> actor =
            vtkSmartPointer<vtkImageActor>::New();
    actor->GetMapper()->SetInputConnection(
            imageReader->GetOutputPort());

    actor->GetProperty()->SetInterpolationTypeToNearest();

    auto bounds_0 = actor->GetBounds();
    std::cout << bounds_0[0] << " " << bounds_0[1] << " " << bounds_0[2] << " "
              << bounds_0[3] << " " << bounds_0[4] << std::endl;

    auto bounds_1 = actor->GetDisplayBounds();
    std::cout << bounds_1[0] << " " << bounds_1[1] << " " << bounds_1[2] << " "
              << bounds_1[3] << " " << bounds_1[4] << std::endl;

    auto bounds_2 = actor->GetDisplayExtent();
    std::cout << bounds_2[0] << " " << bounds_2[1] << " " << bounds_2[2] << " "
              << bounds_2[3] << " " << std::endl;


    auto bounds = renderer->ComputeVisiblePropBounds();
    std::cout << bounds[0] << " " << bounds[1] << " " << bounds[2] << " "
              << bounds[3] << " " << bounds[4] << std::endl;



    renderer->AddActor(actor);
    renderer->ResetCamera();

    std::cout << bounds_1[0] << " " << bounds_1[1] << " " << bounds_1[2] << " "
              << bounds_1[3] << " " << bounds_1[4] << std::endl;

    //////////////////////////////////////////////////////////////////////

    vtkImageReader2 *imageReader2 = readerFactory->CreateImageReader2(
            "/home/penglei/图片/jpg/niu.jpg");
    imageReader2->SetFileName("/home/penglei/图片/jpg/niu.jpg");
    imageReader2->Update();

    vtkSmartPointer<vtkImageActor> actor1 =
            vtkSmartPointer<vtkImageActor>::New();
    actor1->GetMapper()->SetInputConnection(
            imageReader2->GetOutputPort());
    //actor1->SetPosition(0,281, 0);
    actor1->GetProperty()->SetInterpolationTypeToNearest();

    auto bounds0 = renderer->ComputeVisiblePropBounds();
    std::cout << bounds0[0] << " " << bounds0[1] << " " << bounds0[2] << " "
              << bounds0[3] << " " << bounds0[4] << std::endl;

    actor1->SetPosition(0,bounds[3]-actor1->GetBounds()[3], 0);
    renderer->AddActor(actor1);
    ////////////////////////////////////////////////////



    vtkImageReader2 *imageReader3 = readerFactory->CreateImageReader2(
            "/home/penglei/图片/jpg/067.png");
    imageReader3->SetFileName("/home/penglei/图片/jpg/067.png");
    //imageReader3->SetDataExtent(20, 40, 20, 40, 0, 0);
    imageReader3->Update();

    vtkSmartPointer<vtkImageActor> actor2 =
            vtkSmartPointer<vtkImageActor>::New();
    actor2->GetMapper()->SetInputConnection(
            imageReader3->GetOutputPort());
    //actor2->SetPosition(0,281, 0);

    auto bounds1 = renderer->ComputeVisiblePropBounds();
    std::cout << bounds1[0] << " " << bounds1[1] << " " << bounds1[2] << " "
              << bounds1[3] << " " << bounds1[4] << std::endl;

    actor2->SetPosition(0, bounds1[3] - actor2->GetBounds()[3], 0);
    //actor2->SetOpacity(0.2);
    actor2->GetProperty()->SetInterpolationTypeToNearest();

    actor2->SetDisplayExtent(20, 90, 20, 100, 0, 0);
    actor2->Update();

    renderer->AddActor(actor2);

    ///////////////////////////////////////////////////////////

    // Setup renderer
    vtkSmartPointer<vtkNamedColors> colors =
            vtkSmartPointer<vtkNamedColors>::New();


    //renderer->AddActor(actor2);
    //renderer->AddActor(actor);
    //renderer->AddActor(actor1);
    //renderer->ResetCamera();
    renderer->GetActiveCamera()->Zoom(2);
    bounds_1 = actor->GetDisplayBounds();
    std::cout << bounds_1[0] << " " << bounds_1[1] << " " << bounds_1[2] << " "
              << bounds_1[3] << " " << bounds_1[4] << std::endl;



    //renderer->GetActiveCamera()->Zoom(2);
    //renderer->ResetCamera();

    renderer->SetBackground(colors->GetColor3d("lamp_black").GetData());

    // Setup render window1.5
    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // Setup render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style =
            vtkSmartPointer<vtkInteractorStyleImage>::New();

    renderWindowInteractor->SetInteractorStyle(style);

    auto cam = renderer->GetActiveCamera();//SetPosition(1, 1, 1);
    //cam->SetFocalPoint(100, 0, 0);
    //cam->SetPosition(100, 0, 0);
    //cam->SetParallelProjection(1);
    //renderer->ResetCamera();

    // Render and start interaction
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->Initialize();

    renderWindowInteractor->Start();

    imageReader->Delete();
    return EXIT_SUCCESS;
}