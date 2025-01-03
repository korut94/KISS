#include "MC_Game.h"

#include "KC_GameManager.h"

#if IS_EDITOR_BUILD
#include "KT_Editor.h"
#define GameManager KC_GameManager<KT_Editor<MC_Game>>
#else
#define GameManager KC_GameManager<MC_Game>
#endif

int main()
{
   GameManager gameManager;
   gameManager.Run();

   return 0;
}
