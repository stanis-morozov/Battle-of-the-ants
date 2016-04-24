#include "IAntLogic.hpp"

#include <stdlib.h>
#include <string.h>

namespace antlogic
{
    AntAction GetAction(const Ant &ant, AntSensor sensors[3][3])
    {
        char *memory = ant.getMemory();
        AntAction action;
        if (memory[1] == 0 && sensors[1][1].isFood && !sensors[1][1].isMyHill) {
          action.actionType = GET;
          memory[1] = 1;
          return action;
        }
        memory[1] = 0;
        if (ant.hasFood() && sensors[1][1].isMyHill) {
          action.actionType = PUT;
          return action;
        }          
        if (memory[0] == 0 && sensors[2][1].isWall)
          memory[0] = 1;        
        else if (memory[0] == 1 && sensors[1][2].isWall)
          memory[0] = 2;        
        else if (memory[0] == 2 && sensors[0][1].isWall)
          memory[0] = 3;        
        else if (memory[0] == 3 && sensors[1][0].isWall)
          memory[0] = 0;
        if (memory[0] == 0)
          action.actionType = MOVE_RIGHT;
        else if (memory[0] == 1)
          action.actionType = MOVE_DOWN;
        else if (memory[0] == 2)
          action.actionType = MOVE_LEFT;
        else 
          action.actionType = MOVE_UP;
        return action;
    }
}
