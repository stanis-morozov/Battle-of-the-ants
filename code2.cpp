#include "IAntLogic.hpp"

#include <stdlib.h>
#include <string.h>
#include <bitset>
#include <cmath>
#include <iostream>

using namespace std;
//Like bees, but ants
namespace antlogic
{
    //const char GUARD_LIMIT = 500;
    //const char SABOTAGE_LIMIT = 3;

    AntAction GetAction(const Ant &ant, AntSensor wrongsensors[3][3])
    {
        char *memory = ant.getMemory();
        int X_LIMIT = 15;
        int Y_LIMIT = 15;
        AntActionType up, down, left, right;
        int bite = 4;
        AntSensor sensors[3][3];
        sensors[1][1] = wrongsensors[1][1];
        switch (ant.getTeamId())
        {
            case 0:
                sensors[0][0] = wrongsensors[0][0];
                sensors[0][1] = wrongsensors[0][1];
                sensors[0][2] = wrongsensors[0][2];
                sensors[1][0] = wrongsensors[1][0];
                sensors[1][2] = wrongsensors[1][2];
                sensors[2][0] = wrongsensors[2][0];
                sensors[2][1] = wrongsensors[2][1];
                sensors[2][2] = wrongsensors[2][2];
                up = MOVE_UP;
                down = MOVE_DOWN;
                left = MOVE_LEFT;
                right = MOVE_RIGHT;
                break;
            case 1:
                sensors[0][0] = wrongsensors[0][2];
                sensors[0][1] = wrongsensors[1][2];
                sensors[0][2] = wrongsensors[2][2];
                sensors[1][0] = wrongsensors[0][1];
                sensors[1][2] = wrongsensors[2][1];
                sensors[2][0] = wrongsensors[0][2];
                sensors[2][1] = wrongsensors[1][2];
                sensors[2][2] = wrongsensors[2][2];
                up = MOVE_RIGHT;
                down = MOVE_LEFT;
                left = MOVE_UP;
                right = MOVE_DOWN;
                break;
            case 2:
                sensors[0][0] = wrongsensors[2][0];
                sensors[0][1] = wrongsensors[1][0];
                sensors[0][2] = wrongsensors[0][0];
                sensors[1][0] = wrongsensors[2][1];
                sensors[1][2] = wrongsensors[0][1];
                sensors[2][0] = wrongsensors[2][2];
                sensors[2][1] = wrongsensors[1][2];
                sensors[2][2] = wrongsensors[0][2];
                up = MOVE_LEFT;
                down = MOVE_RIGHT;
                left = MOVE_DOWN;
                right = MOVE_UP;
                break;
            case 3:
                sensors[0][0] = wrongsensors[2][2];
                sensors[0][1] = wrongsensors[1][2];
                sensors[0][2] = wrongsensors[2][0];
                sensors[1][0] = wrongsensors[1][2];
                sensors[1][2] = wrongsensors[1][0];
                sensors[2][0] = wrongsensors[0][2];
                sensors[2][1] = wrongsensors[0][1];
                sensors[2][2] = wrongsensors[0][0];
                up = MOVE_DOWN;
                down = MOVE_UP;
                left = MOVE_RIGHT;
                right = MOVE_LEFT;
                break;
        }
        AntAction action;
        bitset<8> classmem (memory[0]);
        char x = memory[1];
        char y = memory[2];
        if (classmem[0] == 0)
        {
            switch(sensors[1][1].smellIntensity)
            {
                case 0:
                    action.putSmell = true;
                    action.smell = 303;
                    classmem[1] = 1;
                    classmem[2] = 1;
                    classmem[3] = 1;
                    break;
                case 98:
                    classmem[2] = 1;
                    break;
                case 97:
                    classmem[2] = 1;
                    classmem[3] = 1;
                    break;
                default:
                    classmem[1] = 1;
                    classmem[2] = 1;
                    classmem[3] = 1;

            }

            classmem[0] = 1;
            action.actionType = right;
            x = 0;
            y = 0;
        }
        else
        {


        /** �����, ������������ � ������������.
          * � ������� �� ������� �������, ����� �������� ���� ����� �� ������� � �������� ���.
          * �� �� ����� ��������� ����� ������ - � ������ ���������� ��� �� ����� �������� ����� ��������� �������� ������.
          * � ����� ������ ������� ���� �� ����������, ������� ���������� ����� �� ����� ��� ����������.
          * ������� ����� ������ ���� ��������� BFS-��, ����� ��������� ���������� ����� ������� ��� � ����, ����� ���� �� ������������ �������� �����������.
          * ��������� ����� ����� ������ ������ ����� ���, ����������� �������� ��.
          * ������ �� �� ������.
          */

            /*if(classmem[1] && !classmem[2]) //��� ������
            {
                action.actionType = right;
            }*/

        /** ���������: �����(classmem[3] = 1) � ��������� (classmem[3] = 0).
          * ����� ����� ����� � �����������.
          * ��������� �� �������� �� ��� ����� GUARD_LIMIT ����� (���� �� �������� �����) � ���� ������ ������.
          * ���������� ����������� ���, ��� ������� ��� �������, � �� �������� ����.
          * ����������������:
          * ��������� ��������, ���� ��� ������, ����� ����� ������ � �� �������� ��� ������ �� �� �������
          * � �����, ��������, �����.
          */


            if (!classmem[1] && classmem[2]) //��� ���������
            {
                if (classmem[3] && sensors[1][1].smellIntensity > 0)
                {
                    action.putSmell = true;
                    action.smell = sensors[1][1].smell;
                }
                if (x < 1)
                {
                    action.actionType = right;
                }
                else if (y < 1)
                {
                    action.actionType = down;
                }
                else
                {
                    if(classmem[3])
                    {
                        if (sensors[1][2].isEnemy && !sensors[0][1].isEnemy)
                        {
                            action.actionType = static_cast<AntActionType>(right + bite);
                        }
                        action.actionType = static_cast<AntActionType>(up + bite);
                    }
                    else
                    {
                        if (sensors[2][1].isEnemy && !sensors[1][0].isEnemy)
                        {
                            action.actionType = static_cast<AntActionType>(down + bite);
                        }
                        action.actionType = static_cast<AntActionType>(left + bite);
                    }
                }
            }



        /** ���� - ����� ������������ ����������.
          * � ����� �������, �� ��������� � ������, ��� �������� ����, � ��� ����, ����� ��������, ����� ������ ���.
          * ����� �� ����� ������ �������� ����������, ������������ �� ������ ��� ��������������.
          * ������ �� ���� ��������� ������� ����� �������, ��� �� ������ ������� �������� 120 �����, �� ������� ����� �������� �������������� 7,5 �� ���.
          * ��� ����� ���� ����� ������� �� ����� ���� ������ ����������� �������.
          *
          * ���� ������� ��������� ����, ��������� ��, � ������� ��� ����������� ����������� ��������.
          * classmem[3] � classmem[4] - ����� ����, � ������� ���� ������������.
          * classmem[6] � classmem[7] - ����� �� �������� ��������� ��� ���������� �����.
          * ����� ����, ����������� ������ � ������������ ����������� ������, ��������� ��� ������ �� ������� 3X3 � ������ ������������ ����� �� ���������, ���� ��� SABOTAGE_LIMIT �� ��������.
          */

            /*else if (!classmem[1] && !classmem[2]) // ��� ����
            {
            bitset<8> spymem (memory[3]);
                if (sensors[1][1].smell % 10 != 3 || sensors[1][1].smell / 100 % 10 != 3)
                {
                    memory[4] = sensors[1][1].smell;

                }
                if (classmem[3] == 0 && classmem[4] == 0)
                {
                    if (x < 1)
                        action.actionType = right;
                    else if (y < 1)
                        action.actionType = down;
                    else if (y > 1)
                        action.actionType = up;
                    else if (x > 1)
                        action.actionType = left;
                    else
                    {
                        if (classmem[5] == 0)
                            classmem[4] = 1;
                        else
                        {
                            classmem[1] = 1;
                            classmem[2] = 0;
                        }
                        action.actionType = PUT;
                    }
                }
                else if (classmem[3] == 0 && classmem[4] == 1)
                {
                    if (x < 29)
                        action.actionType = right;
                    else if (y < 1)
                        action.actionType = down;
                    else if (y > 1)
                        action.actionType = up;
                    else if (x > 29)
                        action.actionType = left;
                    else
                    {
                        classmem[3] = 1;
                        action.actionType = PUT;
                    }
                }
                else if (classmem[3] == 1 && classmem[4] == 1)
                {
                    if (x < 29)
                        action.actionType = right;
                    else if (y < 29)
                        action.actionType = down;
                    else if (y > 29)
                        action.actionType = up;
                    else if (x > 29)
                        action.actionType = left;
                    else
                    {
                        classmem[4] = 0;
                        action.actionType = PUT;
                    }
                }
                else if (classmem[3] == 1 && classmem[4] == 0)
                {
                    if (x < 1)
                        action.actionType = right;
                    else if (y < 29)
                        action.actionType = down;
                    else if (y > 29)
                        action.actionType = up;
                    else if (x > 1)
                        action.actionType = left;
                    else
                    {
                        classmem[3] = 0;
                        classmem[5] = 1;
                        action.actionType = PUT;
                    }
                }
                if (!spymem[0])
                {
                    if (sensors[1][2].isEnemy)
                    {
                        action.actionType = static_cast<AntActionType>(right + bite);
                        spymem[0] = true;
                    }
                    else if (sensors[1][0].isEnemy)
                    {
                        action.actionType = static_cast<AntActionType>(left + bite);
                        spymem[0] = true;
                    }
                    else if (sensors[0][1].isEnemy)
                    {
                        action.actionType = static_cast<AntActionType>(up + bite);
                        spymem[0] = true;
                    }
                    else if (sensors[2][1].isEnemy)
                    {
                        action.actionType = static_cast<AntActionType>(down + bite);
                        spymem[0] = true;
                    }
                    else
                    {
                        spymem[0] = false;
                    }
                }
                memory[3] = static_cast<char>(spymem.to_ulong());
            }
            */




            else // ��� ��������
            {
                if (sensors[0][1].isEnemy && !classmem[6])
                {
                    action.actionType = static_cast<AntActionType>(up + bite);
                    classmem[6] = 1;
                }
                else if (sensors[2][1].isEnemy && !classmem[6])
                {
                    action.actionType = static_cast<AntActionType>(down + bite);
                    classmem[6] = 1;
                }
                else if (sensors[1][0].isEnemy && !classmem[6])
                {
                    action.actionType = static_cast<AntActionType>(left + bite);
                    classmem[6] = 1;
                }
                else if (sensors[1][2].isEnemy && !classmem[6])
                {
                    action.actionType = static_cast<AntActionType>(right + bite);
                    classmem[6] = 1;
                }
                else
                {
                    classmem[6] = 0;
                    if (ant.hasFood() || classmem[5])
                    {
                        cerr << "Back to the anthill" << endl;
                        if (sensors[1][1].isFood &&  !sensors[1][1].isMyHill)
                        {
                            if (!ant.hasFood())
                            {
                                action.actionType = GET;
                            }
                            else
                            {
                                memory[3] = x;
                                memory[5] = y;
                            }
                        }
                        if (x > 0)
                        {
                            action.actionType = left;
                        }
                        else if (y > 0)
                        {
                            action.actionType = up;
                        }
                        else
                        {
                            action.actionType = PUT;
                            classmem[5] = 0;
                            classmem[4] = 0;
                        }
                    }
                    else if (sensors[1][1].isMyHill)
                    {

                        cerr << "At the anthill" << endl;
                        classmem[5] = 0;
                        if (ant.hasFood())
                        {
                            action.actionType = PUT;
                            classmem[4] = 0;
                        }
                        else
                        {
                            action.actionType = right;
                        }
                    }
                    else if (classmem[4])
                    {
                        cerr << "Gathering..." << endl;
                        if (memory[4] == 0 && memory[6] == 0)
                        {
                            if (sensors[1][1].isFood)
                            {
                                action.actionType = GET;
                            }
                            /*else if (wrongsensors[1][0].isFood)
                            {
                                action.actionType = MOVE_LEFT;
                            }
                            else if (wrongsensors[1][2].isFood)
                            {
                                action.actionType = MOVE_RIGHT;
                            }
                            else if (wrongsensors[2][1].isFood)
                            {
                                action.actionType = MOVE_DOWN;
                            }
                            else if (wrongsensors[0][1].isFood)
                            {
                                action.actionType = MOVE_UP;
                            }*/
                            else if (x >= X_LIMIT && y >= Y_LIMIT)
                            {
                                classmem[5] = 1;
                                action.actionType = GET;
                            }
                            else if (x >= X_LIMIT ||(rand() % 2 == 0 && y < Y_LIMIT))
                            {
                                action.actionType = down;
                            }
                            else
                            {
                                action.actionType = right;
                            }

                        }
                        else
                        {
                            cerr << "Sure where to go" << endl;
                            if (sensors[1][1].isFood)
                            {
                                action.actionType = GET;
                            }
                            else if (x < memory[4])
                            {
                                action.actionType = right;
                            }
                            else if (x > memory[4])
                            {
                                action.actionType = left;
                            }
                            else if (y < memory[6])
                            {
                                action.actionType = down;
                            }
                            else if (y > memory[6])
                            {
                                action.actionType = up;
                            }
                            else if (sensors[1][1].isFood)
                            {
                                action.actionType = GET;
                            }
                            else
                            {
                                memory[4] = 0;
                                memory[6] = 0;
                                classmem[7] = 1;
                                action.actionType = PUT;
                            }
                        }
                    }
                    else
                    {
                        cerr << "Checking in at the HQ" << endl;
                        if (x < 1)
                        {
                            action.actionType = right;
                        }
                        else if (y < 1)
                        {
                            action.actionType = down;
                        }
                        else if (y > 1)
                        {
                            action.actionType = up;
                        }
                        else if (x > 1)
                        {
                            action.actionType = left;
                        }
                        else
                        {
                            memory[4] = 0;
                            memory[6] = 0;
                            classmem[4] = 1;
                            if (classmem[7])
                            {
                                action.putSmell = true;
                                action.smell = 0;
                                classmem[7] = 0;
                            }
                            if ((memory[3] > 0 || memory[5] > 0) && (sensors[1][1].smellIntensity == 0 || sensors[1][1].smell == 0 || sensors[1][1].smell / 30 + sensors[1][1].smell % 30 > memory[3] + memory[5]))
                            {
                                cerr << "Put smell for " <<  memory[3] << " " << memory[5] << endl;
                                action.putSmell = true;
                                action.smell = memory[3] * 30 + memory[5];
                                memory[4] = memory[3];
                                memory[6] = memory[5];
                            }
                            else if (sensors[1][1].smellIntensity > 0 && sensors[1][1].smell > 0)
                            {
                                memory[4] = sensors[1][1].smell / 30;
                                memory[6] = sensors[1][1].smell % 30;
                            }
                            action.actionType = right;
                        }
                    }
                }
            }





        }
        if (action.actionType == right)
        {
            x++;
        }
        else if (action.actionType == down)
        {
            y++;
        }
        else if (action.actionType == up)
        {
            y--;
        }
        else if (action.actionType == left)
        {
            x--;
        }
        memory[0] = static_cast<char>(classmem.to_ulong());
        memory[1] = x;
        memory[2] = y;


        return action;
    }
}

