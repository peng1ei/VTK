//
// Created by penglei on 19-1-8.
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
#include <vtkLightCollection.h>
#include <vtkLight.h>

#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

#include "ImageShowCore.h"

int main() {

    /////////////////////////////////////////////////////////////
    //           一 准备渲染引擎
    /////////////////////////////////////////////////////////////
    /// 1 Setup renderer
    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();

    // 开启平行投影相机模式
    renderer->GetActiveCamera()->ParallelProjectionOn();

    // 关闭所有的灯光，对于 2D 图形系统来说不必要
    // 或者直接移除
    //renderer->RemoveAllLights();
//    auto lights = renderer->GetLights();
//    while (auto light = lights->GetNextItem()) {
//        light->SwitchOff();
//    }

    // 设置 renderer 背景颜色
    vtkSmartPointer<vtkNamedColors> colors =
            vtkSmartPointer<vtkNamedColors>::New();
    renderer->SetBackground(colors->GetColor3d("white").GetData());

    /// 2 Setup render window
    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();

    renderWindow->SetPosition(renderWindow->GetScreenSize()[0]/2 - g_kWindowWidth/2,
                              renderWindow->GetScreenSize()[1]/2 - g_kWindowHeight/2);
    renderWindow->SetSize(g_kWindowWidth, g_kWindowHeight);

    renderWindow->AddRenderer(renderer);


    /// 3 Setup render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();

    // TODO 实现自己的 style
    vtkSmartPointer<vtkInteractorStyleImage> style =
            vtkSmartPointer<vtkInteractorStyleImage>::New();

    renderWindowInteractor->SetInteractorStyle(style);
    renderWindowInteractor->SetRenderWindow(renderWindow);


    /////////////////////////////////////////////////////////////
    //           二 可视化管道
    /////////////////////////////////////////////////////////////

    vtkSmartPointer<vtkImageReader2Factory> readerFactory =
            vtkSmartPointer<vtkImageReader2Factory>::New();

    //----------------------------------------------------------
    // begin linux actor
    std::string filenameLinux("/home/penglei/图片/jpg/cmu213.png"); // cmu213.png
    vtkImageReader2 *imageReader = readerFactory->CreateImageReader2(filenameLinux.c_str());
    imageReader->SetFileName(filenameLinux.c_str());
    //imageReader->UpdateInformation(); // 对于 vtkGDAL 可能适用
    imageReader->Update();


    int extent[6];
    imageReader->GetOutput()->GetExtent(extent);
    imageReader->UpdateExtent(extent);

    double origin[3];
    imageReader->GetOutput()->GetOrigin(origin);

    double spacing[3];
    imageReader->GetOutput()->GetSpacing(spacing);


    vtkSmartPointer<vtkImageActor> actorLinux =
            vtkSmartPointer<vtkImageActor>::New();
    actorLinux->GetMapper()->SetInputConnection(
            imageReader->GetOutputPort());

    // 设置 actor 放大缩小时的重采样方法
    // VTK_NEAREST_INTERPOLATION - 最邻近
    // VTK_LINEAR_INTERPOLATION - 线性差值
    // VTK_CUBIC_INTERPOLATION - 三次卷积
    actorLinux->SetInterpolate(VTK_NEAREST_INTERPOLATION);
    // 或者可以通过 actor 的 Property 进行设置
    // actorLinux->GetProperty()->SetInterpolationTypeToNearest();

    // 设置 Camera 位置及焦点
    double xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
    double yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];

    auto camera = renderer->GetActiveCamera();
    camera->SetParallelScale(0.5 * g_kWindowHeight); // 设置为窗体的一半时，为 1:1 显示？？？
    camera->SetFocalPoint(xc, yc, g_kCameraFocalPointZ);
    camera->SetPosition(xc, yc, g_kCameraPositionPointZ);


    // Create the outline 创建影像的外边框
    vtkSmartPointer<vtkOutlineFilter> outline =
            vtkSmartPointer<vtkOutlineFilter>::New();
    outline->SetInputConnection(imageReader->GetOutputPort());

    vtkSmartPointer<vtkPolyDataMapper> outlineMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    outlineMapper->SetInputConnection(outline->GetOutputPort());

    // 默认情况下，Mapper输入的标量数据会对Actor进行着色，
    // 而Actor的颜色设置会被忽略。如果要忽略这些标量数据，
    // 可以使用方法 ScalarVisibilityOff()
    outlineMapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> outlineActor =
            vtkSmartPointer<vtkActor>::New();
    outlineActor->SetMapper(outlineMapper);
    outlineActor->GetProperty()->SetColor(0, 1, 1);
    outlineActor->GetProperty()->SetLineWidth(1.25);
    //outlineActor->GetProperty()->SetOpacity(1.0); // 设置不透明度, 0 - 完全透明, 1 - 完全不透明



    // 重点！！！ 注意 outline actor 的添加顺序
    // image actor 应该先加入渲染，然后 outline actor 再加入渲染
    renderer->AddActor(actorLinux);
    // outline actor 应该在影像后面添加，不然可能会出现边框颜色变浅
    renderer->AddActor(outlineActor);

    // end linux actor
    //----------------------------------------------------------

    //----------------------------------------------------------
    //















    /////////////////////////////////////////////////////////////
    //            三 开启渲染引擎，并支持用户交互
    /////////////////////////////////////////////////////////////
    renderWindowInteractor->Initialize();
    renderWindowInteractor->Start();

    /////////////////////////////////////////////////////////////
    //            四 资源释放
    /////////////////////////////////////////////////////////////
    imageReader->Delete();

    return EXIT_SUCCESS;
}

