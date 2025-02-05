#include "core/geometry.hpp"

#include "core/mesh.hpp"
#include "core/scene.hpp"
#include "core/camera.hpp"
#include "integrator/path.hpp"

int main()
{
    // scene setting
    auto scn = gi::MakeShared<gi::Scene>();
    scn->AddObject(gi::MakeShared<gi::Mesh>("../assets/models", "cornell_box", ".obj"));
    // scn->AddObject(gi::MakeShared<gi::Mesh>("../assets/models", "cube", ".obj"));
    scn->Build();

    // camera setting
    auto cam = gi::MakeShared<gi::Camera>();

    // create integrator
    auto integrator = gi::MakeShared<gi::Path>(cam, scn);
    integrator->Render();
    return 0;
}
