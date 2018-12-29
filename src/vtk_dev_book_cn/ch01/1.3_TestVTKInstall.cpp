//
// Created by penglei on 18-12-27.
//

#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

int main() {
    vtkSmartPointer<vtkRenderWindow> ren_win =
            vtkSmartPointer<vtkRenderWindow>::New();
    ren_win->Render();

    std::cin.get();
    return 0;
}
