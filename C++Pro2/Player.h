#pragma once  
#include"Map.h"
#include<iostream>


enum PlayerFlip
{
    Right,Left,Up,Down
};

class Player
{
public:

	/// <summary>
	/// x为纵方向,y为横方向
	/// </summary>
	Vector2 position;

	bool isDead;
    PlayerFlip flip;

    int score;


    Player(int x, int y) : position(x, y), isDead(false) { score = 0; }

    // 玩家死亡
    void die() 
    {
        isDead = true;
        std::cout << "Player is dead!" << std::endl;
    }


    // 玩家移动
    void Move(char c, Map* map)
    {
        int newX = position.x;  // 纵向坐标
        int newY = position.y;  // 横向坐标

        switch (c)
        {
        case 'a': // 左
            newY -= 1;
            flip = Left;
            break;
        case 'd': // 右
            newY += 1;
            flip = Right;
            break;
        case 'w': // 上
            newX -= 1;
            flip = Up;
            break;
        case 's': // 下
            newX += 1;
            flip = Down;
            break;
        default:
            return; // 如果输入不是w/a/s/d，返回
        }


        // 检查新位置是否为墙壁
        if (!map->isWall(newX, newY))  // 如果新位置不是墙壁
        {
            map->Clear(position);  // 清除当前玩家的位置
            map->ClearSocre(position);
            position.x = newX;    // 更新玩家横向坐标
            position.y = newY;    // 更新玩家纵向坐标
          //  std::cout << "Player Position is " << position.x << " " << position.y << std::endl;

            map->DrawPlayer(position.x,position.y ,2); 
        }
        std::cout << "Player Position is " << position.x << " " << position.y << std::endl;
    }

    // 判断是否碰到墙
    bool NotWall(Map* map, int x, int y)
    {
        if (map->grid[x][y] == 1)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    void CheckCollision(Map* map)
    {
        if (map->scoreGrid[position.x][position.y] == 1)
        {
            score++;
            std::cout << "分数加一" << std::endl;
        }
    }

    void DisPlayScore()
    {
        std::cout << "本局分数为" << " " << score << std::endl;
    }



    void Update(int deltaTime)
    {
    }
};
