//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "Game.h"
//---------------------------------------------------------------------
//---------------------------------------------------------------------
int main(){
    Game game;
    game.Run();
#ifdef _WIN32 //32-bit and 64-bit Windows
    system("pause");
#endif
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------