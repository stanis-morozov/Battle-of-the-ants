#include "IAntLogic.hpp"

#include <stdlib.h>
#include <string.h>
#include <math.h>

namespace antlogic
{
    AntAction GetAction(const Ant &ant, AntSensor sensors[3][3])
    {
        char *memory = ant.getMemory();
        AntAction action;
        if (memory[0] == 0)
        {
            if (sensors[2][1].isWall && sensors[1][0].isWall && sensors[1][1].isMyHill)
              memory[0] = 1;
            else if (sensors[1][2].isWall && sensors[2][1].isWall && sensors[1][1].isMyHill)
              memory[0] = 2;
            else if (sensors[0][1].isWall && sensors[1][2].isWall && sensors[1][1].isMyHill)
              memory[0] = 3;
            else if (sensors[1][0].isWall && sensors[0][1].isWall && sensors[1][1].isMyHill)
              memory[0] = 4;
        }
        if (sensors[1][1].isMyHill && sensors[1][1].smell != 78 && !sensors[1][1].isFood)
            action.putSmell = true;
            action.smell = 78;
        if (sensors[1][1].isMyHill && sensors[1][1].smell == 78 && (sensors[1][1].smellIntensity == 92 || sensors[1][1].smellIntensity == 91 ) ) memory[2] = 3;
        if (memory[2] == 3)
        {
            memory[4] = 1;
            memory[5] = 1;
            if (memory[6] < memory[4])
                {
                    if (memory[0] == 1 || memory[0] == 2) action.actionType = MOVE_LEFT;
                    else action.actionType = MOVE_RIGHT;
                    memory[6] +=1;
                return action;
                }
            if (memory[7] < memory[5])
                {
                    if (memory[0] == 2 || memory[0] == 3) action.actionType = MOVE_UP;
                    else action.actionType = MOVE_DOWN;
                    memory[7] +=1;
                return action;
                }
            int ra = rand();
            if (sensors[2][1].isEnemy && ra%2 == 0)
                {action.actionType = BITE_RIGHT;return action;}
            else if (sensors[1][2].isEnemy && ra%2 == 1)
                {action.actionType = BITE_DOWN;return action;}
            else if (sensors[0][1].isEnemy && ra%2 == 0)
                {action.actionType = BITE_LEFT;return action;}
            else if (sensors[1][0].isEnemy && ra%2 == 1)
                {action.actionType = BITE_UP;return action;}
            else if (sensors[2][1].isEnemy)
                {action.actionType = BITE_RIGHT;return action;}
            else if (sensors[1][2].isEnemy)
                {action.actionType = BITE_DOWN;return action;}
            else if (sensors[0][1].isEnemy)
                {action.actionType = BITE_LEFT;return action;}
            else
                {action.actionType = BITE_UP;return action;}

        }
        int ra = rand();
        if (sensors[2][1].isEnemy && ra%3 == 0)
            {action.actionType = BITE_RIGHT;return action;}
        else if (sensors[1][2].isEnemy && ra%3 == 1)
            {action.actionType = BITE_DOWN;return action;}
        else if (sensors[0][1].isEnemy && ra%3 == 0)
            {action.actionType = BITE_LEFT;return action;}
        else if (sensors[1][0].isEnemy && ra%3 == 1)
            {action.actionType = BITE_UP;return action;}


        if (memory[2]%2 == 0)
        {
            if (!ant.hasFood() && sensors[1][1].isFood && !sensors[1][1].isMyHill) {
              action.actionType = GET;
              return action;
            }
            if (ant.hasFood() && sensors[1][1].isMyHill) {
              action.actionType = PUT;
              if (memory[2] == 8 || memory[4] > 15 || memory[5] > 15) memory[2] = 1;
              else memory[2] += 2;
              return action;
            }
            if (!ant.hasFood())
            {
                if (sensors[1][0].isFood && !sensors[1][0].isMyHill)
                {
                    if (memory[0] == 2 || memory[0] == 3) memory[5] = memory[7] + 1;
                    else memory[5] = memory[7] - 1;
                }
                else if (sensors[2][1].isFood && !sensors[2][1].isMyHill)
                {
                    if (memory[0] == 3 || memory[0] == 4) memory[4] = memory[6] + 1;
                    else memory[4] = memory[6] - 1;
                }
                else if (sensors[1][2].isFood && !sensors[1][2].isMyHill)
                {
                    if (memory[0] == 2 || memory[0] == 3) memory[5] = memory[7] - 1;
                    else memory[5] = memory[7] + 1;
                }
                else if (sensors[0][1].isFood && !sensors[0][1].isMyHill)
                {
                    if (memory[0] == 3 || memory[0] == 4) memory[4] = memory[6] - 1;
                    else memory[4] = memory[6] + 1;
                }
                else if (sensors[1][1].isFood && !sensors[1][1].isMyHill)
                {
                    memory[4] = memory[6]; memory[5] = memory[7];
                }
                else if (sensors[0][0].isFood && !sensors[0][0].isMyHill)
                {
                    if (memory[0] == 1){memory[4] = memory[6] + 1; memory[5] = memory[7] - 1;}
                    else if (memory[0] == 2){memory[4] = memory[6] + 1; memory[5] = memory[7] + 1;}
                    else if (memory[0] == 3){memory[4] = memory[6] - 1; memory[5] = memory[7] + 1;}
                    else {memory[4] = memory[6] - 1; memory[5] = memory[7] - 1;}
                }
                else if (sensors[2][0].isFood && !sensors[2][0].isMyHill)
                {
                    if (memory[0] == 1){memory[4] = memory[6] - 1; memory[5] = memory[7] - 1;}
                    else if (memory[0] == 2){memory[4] = memory[6] - 1; memory[5] = memory[7] + 1;}
                    else if (memory[0] == 3){memory[4] = memory[6] + 1; memory[5] = memory[7] + 1;}
                    else {memory[4] = memory[6] + 1; memory[5] = memory[7] - 1;}
                }
                else if (sensors[2][2].isFood && !sensors[2][2].isMyHill)
                {
                    if (memory[0] == 1){memory[4] = memory[6] - 1; memory[5] = memory[7] + 1;}
                    else if (memory[0] == 2){memory[4] = memory[6] - 1; memory[5] = memory[7] - 1;}
                    else if (memory[0] == 3){memory[4] = memory[6] + 1; memory[5] = memory[7] - 1;}
                    else {memory[4] = memory[6] + 1; memory[5] = memory[7] + 1;}
                }
                else if (sensors[0][2].isFood && !sensors[0][2].isMyHill)
                {
                    if (memory[0] == 1){memory[4] = memory[6] + 1; memory[5] = memory[7] + 1;}
                    else if (memory[0] == 2){memory[4] = memory[6] + 1; memory[5] = memory[7] - 1;}
                    else if (memory[0] == 3){memory[4] = memory[6] - 1; memory[5] = memory[7] - 1;}
                    else {memory[4] = memory[6] - 1; memory[5] = memory[7] + 1;}
                }

                if ( (memory[4] == 0 && memory[5] == 0) || (memory[6] == memory[4] && memory[7] == memory[5] && !sensors[1][1].isFood) )
                {
                    memory[4] = 0;
                    memory[5] = 0;

                    int ryt = random();
                    if (memory[0] == 1)
                    {
                        if (ryt % 2 == 0 && !sensors[0][1].isWall) {action.actionType = MOVE_LEFT; memory[6] +=1;}
                        else {action.actionType = MOVE_DOWN; memory[7] +=1;}
                    }
                    else if (memory[0] == 2)
                    {
                        if (ryt % 2 == 0 && !sensors[0][1].isWall) {action.actionType = MOVE_LEFT; memory[6] +=1;}
                        else {action.actionType = MOVE_UP; memory[7] +=1;}
                    }
                    else if (memory[0] == 3)
                    {
                        if (ryt % 2 == 0 && !sensors[2][1].isWall) {action.actionType = MOVE_RIGHT; memory[6] +=1;}
                        else {action.actionType = MOVE_UP; memory[7] +=1;}
                    }
                    else if (memory[0] == 4)
                    {
                        if (ryt % 2 == 0 && !sensors[2][1].isWall) {action.actionType = MOVE_RIGHT; memory[6] +=1;}
                        else {action.actionType = MOVE_DOWN; memory[7] +=1;}
                    }
                    return action;
                }
                if (memory[4] != 0 || memory[5] != 0)
                {
                    if (memory[6] < memory[4] && abs(memory[6]-memory[4]) >= abs(memory[7] - memory[5]))
                    {
                        if (memory[0] == 1 || memory[0] == 2) action.actionType = MOVE_LEFT;
                        else action.actionType = MOVE_RIGHT;
                         memory[6] +=1;
                    }
                    else if (memory[6] > memory[4])
                    {
                        if (memory[0] == 1 || memory[0] == 2) action.actionType = MOVE_RIGHT;
                        else action.actionType = MOVE_LEFT;
                        memory[6] -=1;
                    }
                    else if (memory[7] < memory[5])
                    {
                        if (memory[0] == 2 || memory[0] == 3) action.actionType = MOVE_UP;
                        else action.actionType = MOVE_DOWN;
                        memory[7] +=1;
                    }
                    else if (memory[7] > memory[5])
                    {
                        if (memory[0] == 2 || memory[0] == 3) action.actionType = MOVE_DOWN;
                        else action.actionType = MOVE_UP;
                        memory[7] -=1;
                    }
                    return action;
                }

            }
            else
            {
                if (memory[6] > 0 && memory[6] > memory[7])
                {
                    if (memory[0] == 1 || memory[0] == 2){action.actionType = MOVE_RIGHT; memory[6] -=1;}
                    else {action.actionType = MOVE_LEFT; memory[6] -=1;}
                }
                else if (memory[7] > 0)
                {
                    if (memory[0] == 2 || memory[0] == 3){action.actionType = MOVE_DOWN; memory[7] -=1;}
                    else {action.actionType = MOVE_UP; memory[7] -=1;}
                }
                return action;
            }
        }
        else if (memory[2] == 1)
        {
            memory[2] = 1;
            int reg;
            if (memory[9] != 0) reg = memory[9];
            else if (memory[10] != 0) reg = memory[10];
            else {while (reg == 0)reg = random();}
            memory[9] = reg%2 + 2;
            if (!ant.hasFood() && sensors[1][1].isFood && !sensors[1][1].isMyHill) {
              action.actionType = GET;
              return action;
            }
            if (ant.hasFood() && sensors[1][1].isMyHill) {
              action.actionType = PUT;
              memory[2] = 4;
              memory[9] = 0;
              return action;
            }
            else if (!ant.hasFood() && !sensors[1][1].isFood && sensors[1][1].isEnemyHill) {
                memory[10] = 1 + memory[9];
            }

            if ( ((memory[0] == 4 && ant.hasFood() == 0) || (memory[0] == 2 && ant.hasFood() == 1)) && reg%2 == 0)
              action.actionType = MOVE_RIGHT;
            else if ( ((memory[0] == 1 && ant.hasFood() == 0) || (memory[0] == 3 && ant.hasFood() == 1)) && reg%2 ==0)
              action.actionType = MOVE_DOWN;
            else if ( ((memory[0] == 2 && ant.hasFood() == 0) || (memory[0] == 4 && ant.hasFood() == 1)) && reg%2 == 0)
              action.actionType = MOVE_LEFT;
            else if ( ((memory[0] == 3 && ant.hasFood() == 0)  || (memory[0] == 1 && ant.hasFood() == 1)) && reg%2 == 0)
              action.actionType = MOVE_UP;
            else if ( ((memory[0] == 3 && ant.hasFood() == 0) || (memory[0] == 1 && ant.hasFood() == 1)) && reg%2 == 1)
              action.actionType = MOVE_RIGHT;
            else if ( ((memory[0] == 4 && ant.hasFood() == 0) || (memory[0] == 2 && ant.hasFood() == 1)) && reg%2 ==1)
              action.actionType = MOVE_DOWN;
            else if ( ((memory[0] == 1 && ant.hasFood() == 0) || (memory[0] == 3 && ant.hasFood() == 1)) && reg%2 == 1)
              action.actionType = MOVE_LEFT;
            else if ( ((memory[0] == 2 && ant.hasFood() == 0)  || (memory[0] == 4 && ant.hasFood() == 1)) && reg%2 == 1)
              action.actionType = MOVE_UP;
            return action;
        }
    }
}

