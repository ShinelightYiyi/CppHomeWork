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
	/// xΪ�ݷ���,yΪ�᷽��
	/// </summary>
	Vector2 position;

	bool isDead;
    PlayerFlip flip;

    int score;


    Player(int x, int y) : position(x, y), isDead(false) { score = 0; }

    // �������
    void die() 
    {
        isDead = true;
        std::cout << "Player is dead!" << std::endl;
    }


    // ����ƶ�
    void Move(char c, Map* map)
    {
        int newX = position.x;  // ��������
        int newY = position.y;  // ��������

        switch (c)
        {
        case 'a': // ��
            newY -= 1;
            flip = Left;
            break;
        case 'd': // ��
            newY += 1;
            flip = Right;
            break;
        case 'w': // ��
            newX -= 1;
            flip = Up;
            break;
        case 's': // ��
            newX += 1;
            flip = Down;
            break;
        default:
            return; // ������벻��w/a/s/d������
        }


        // �����λ���Ƿ�Ϊǽ��
        if (!map->isWall(newX, newY))  // �����λ�ò���ǽ��
        {
            map->Clear(position);  // �����ǰ��ҵ�λ��
            map->ClearSocre(position);
            position.x = newX;    // ������Һ�������
            position.y = newY;    // ���������������
          //  std::cout << "Player Position is " << position.x << " " << position.y << std::endl;

            map->DrawPlayer(position.x,position.y ,2); 
        }
        std::cout << "Player Position is " << position.x << " " << position.y << std::endl;
    }

    // �ж��Ƿ�����ǽ
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
            std::cout << "������һ" << std::endl;
        }
    }

    void DisPlayScore()
    {
        std::cout << "���ַ���Ϊ" << " " << score << std::endl;
    }



    void Update(int deltaTime)
    {
    }
};
