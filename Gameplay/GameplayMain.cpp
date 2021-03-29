#include "Gameplay/GameplayMain.h"
#include "CrossPlatform/Primitive/Primitive2D.h"


namespace Gameplay {
    using Primitive::Primitive2D;

    void GameplayMain()
    {
        Primitive2D::get().DrawLine(Vector2i(10, 10), Vector2i(20, 50));
        Primitive2D::get().DrawLine(Vector2i(10, 10), Vector2i(50, 20));
        Primitive2D::get().DrawLine(Vector2i(10, 50), Vector2i(50, 20));
        Primitive2D::get().DrawLine(Vector2i(90, 90), Vector2i(70, 60));
    }

}