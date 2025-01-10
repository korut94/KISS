#include "KC_ClearCanvasSystem.h"

#include "KC_Canvas.h"

void KC_ClearCanvasSystem::Run()
{
    for (KC_Entity entity : myEntitySet)
    {
        GetComponent<KC_Canvas>(entity).myVertexes.clear();
    }
}
