#include "MC_Game.h"

#include "KC_GameManager.h"

#include "KT_Editor.h"

int main()
{
   KC_GameManager<KT_Editor<MC_Game>> gameManager;
   gameManager.Run();

   return 0;
}
