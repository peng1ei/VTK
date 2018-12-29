//
// Created by penglei on 18-12-29.
//

#include <vtkSmartPointer.h>
#include <vtkCoordinate.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

int main(int, char *[])
{
  vtkSmartPointer<vtkRenderWindow> rendererWindow =
    vtkSmartPointer<vtkRenderWindow>::New();

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  rendererWindow->AddRenderer(renderer);
  rendererWindow->Render();

  vtkSmartPointer<vtkCoordinate> coordinate =
    vtkSmartPointer<vtkCoordinate>::New();
  coordinate->SetCoordinateSystemToDisplay();
  coordinate->SetValue(6,6,0);
  std::cout << *coordinate << std::endl;
  std::cout << coordinate->GetCoordinateSystemAsString() << std::endl;

  double* val;
  val = coordinate->GetComputedWorldValue(renderer);
  std::cout << "Val: " << val[0] << " " << val[1] << std::endl;

  return EXIT_SUCCESS;
}