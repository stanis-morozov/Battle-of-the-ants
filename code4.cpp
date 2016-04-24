#include "IAntLogic.hpp"

#include <stdlib.h>
#include <string.h>

namespace antlogic
{
    AntAction RandomStep(AntSensor sensors[3][3])
    {
        int ok = 0;
        int d;
        while (!ok) {
            d = rand() & 3;
            ok = 1;
            if (d == MOVE_UP && sensors[1][0].isWall) {
                ok = 0;
            }
            if (d == MOVE_LEFT && sensors[0][1].isWall) {
                ok = 0;
            }
            if (d == MOVE_DOWN && sensors[1][2].isWall) {
                ok = 0;
            }
            if (d == MOVE_RIGHT && sensors[2][1].isWall) {
                ok = 0;
            }
        }
        AntAction act;
        act.actionType = static_cast<AntActionType>(d);
        return act;
    }

    AntAction ToBase(char *memory, AntSensor sensors[3][3])
    {
        int ok = 1;
        if (memory[0] >= 0) {
            if (memory[0] == MOVE_UP && sensors[1][0].isWall) {
                ok = 0;
            }
            if (memory[0] == MOVE_LEFT && sensors[0][1].isWall) {
                ok = 0;
            }
            if (memory[0] == MOVE_DOWN && sensors[1][2].isWall) {
                ok = 0;
            }
            if (memory[0] == MOVE_RIGHT && sensors[2][1].isWall) {
                ok = 0;
            }
            if (!ok) {
                memory[0] = -1;
            }
        }
        if (sensors[1][1].isMyHill) {
            AntAction act;
            act.actionType = PUT;
            return act;
        }
        if (memory[0] >= 0) {
            AntAction act;
            act.actionType = static_cast<AntActionType>(memory[0]);
            return act;
        }
        if (sensors[1][2].isWall && sensors[0][1].isWall) {
            AntAction act;
            act.actionType = MOVE_RIGHT;
            return act;
        }
        if (sensors[2][1].isWall && sensors[1][2].isWall) {
            AntAction act;
            act.actionType = MOVE_UP;
            return act;
        }
        if (sensors[1][0].isWall && sensors[2][1].isWall) {
            AntAction act;
            act.actionType = MOVE_LEFT;
            return act;
        }
        if (sensors[1][0].isWall && sensors[0][1].isWall) {
            AntAction act;
            act.actionType = MOVE_DOWN;
            return act;
        }
        
        if (sensors[0][1].isWall) {
            AntAction act;
            act.actionType = MOVE_DOWN;
            return act;
        }
        if (sensors[1][2].isWall) {
            AntAction act;
            act.actionType = MOVE_RIGHT;
            return act;
        }
        if (sensors[2][1].isWall) {
            AntAction act;
            act.actionType = MOVE_UP;
            return act;
        }
        if (sensors[1][0].isWall) {
            AntAction act;
            act.actionType = MOVE_LEFT;
            return act;
        }
    }

    AntAction GetAction(const Ant &ant, AntSensor sensors[3][3])
    {
        char *memory = ant.getMemory();
        if (ant.hasFood()) {
            return ToBase(memory, sensors);
        }
        if (sensors[1][1].isFood && !sensors[1][1].isMyHill) {
            AntAction act;
            act.actionType = GET;
            memory[0] = rand() & 3;
            return act;
        }
        if (!sensors[0][0].isFriend && !sensors[1][0].isFriend && !sensors[2][0].isFriend && !sensors[0][1].isFriend && !sensors[1][1].isFriend && !sensors[2][1].isFriend && !sensors[0][2].isFriend && !sensors[1][2].isFriend && !sensors[2][2].isFriend) {
            if (sensors[1][0].isEnemy) {
                AntAction act;
                act.actionType = BITE_UP;
                return act;
            }
            if (sensors[0][1].isEnemy) {
                AntAction act;
                act.actionType = BITE_LEFT;
                return act;
            }
            if (sensors[2][1].isEnemy) {
                AntAction act;
                act.actionType = BITE_RIGHT;
                return act;
            }
            if (sensors[1][2].isEnemy) {
                AntAction act;
                act.actionType = BITE_DOWN;
                return act;
            }
        }
        return RandomStep(sensors);
    }
}
