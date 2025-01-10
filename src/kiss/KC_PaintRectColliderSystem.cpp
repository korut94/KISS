#include "KC_PaintRectColliderSystem.h"

void KC_PaintRectColliderSystem::Run()
{
    for (KC_Entity entity : myEntitySet)
    {
        KC_Canvas& canvas = GetComponent<KC_Canvas>(entity);
        const KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_RectCollider& collider = GetComponent<KC_RectCollider>(entity);
        const KC_RectColliderPalette& palette = GetComponent<KC_RectColliderPalette>(entity);

        const KC_FloatRect bound = transform * collider.myBound;
        DrawRectangle(canvas, bound.GetCenter(), bound.GetSize(), palette.myColor);
    }
}
