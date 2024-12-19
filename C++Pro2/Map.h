#ifndef _MAP_H_
#define _MAP_H_


#include"Ghost.h"
#include<vector>
#include<iostream>
#include<memory>
#include"Player.h"



class Map
{
public:
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<int>> scoreGrid;
    int width, height;

    Map(int w, int h) : width(w), height(h) 
    {
        grid.resize(h, std::vector<int>(w, 0));  // 初始化为全可通行区域
        scoreGrid.resize(h, std::vector<int>(w, 0));
    }


    /// <summary>
    /// 设置地图
    /// </summary>
    /// <param name="v"></param>
    void SetMap(std::vector<std::vector<int>>& v)
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                grid[i][j] = v[i][j];
            }
        }
    }


    /// <summary>
    /// 设置分数地图(分数的位置)
    /// </summary>
    /// <param name="v"></param>
    void SetScoreMap(std::vector<std::vector<int>>& v)
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                scoreGrid[i][j] = v[i][j];
            }
        }
    }

    void addWall(int x, int y) 
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
            grid[y][x] = 1;  // 1 表示墙壁
    }


    /// <summary>
    /// 检查是否为墙面
    /// </summary>
    /// <param name="纵向坐标"></param>
    /// <param name="横向坐标"></param>
    /// <returns></returns>
    bool isWall(int x, int y) const 
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return true;  // 超出边界也视为墙壁
        return grid[x][y] == 1;
    }


    bool isGhost(int x,int y) const
    {
        return grid[x][y] == 3 || grid[x][y] == 4 || grid[x][y] == 5;
    }


    /// <summary>
    /// 绘制单位，2为玩家，3为红鬼，4为粉鬼，5为蓝鬼，6为橙鬼
    /// </summary>
    /// <param name="纵向坐标"></param>
    /// <param name="横向坐标"></param>
    /// <param name="单位数据"></param>
    void DrawPlayer(int x, int y,int o)
    {
        switch (o)
        {
        case 2: grid[x][y] = 2; break;  
        case 3:grid[x][y] = 3; break;
        case 4:grid[x][y] = 4; break;
        case 5:grid[x][y] = 5; break;
        case 6:grid[x][y] = 6; break;
       }
    }


    void Clear(Vector2 v)
    {
        grid[v.x][v.y] = 0;
    }

    void ClearSocre(Vector2 v)
    {
        scoreGrid[v.x][v.y] = 0;
    }

    void printMap() const
    {
        bool isGhost = false;
        for (int x = 0; x < height; x++)
        {
            for (int y = 0; y < width; y++)
            {
              //  std::cout << (grid[y][x] == 1 ? '#' : '.') << " "; // '#' 表示墙壁，'.' 表示通行区域  

                if (grid[x][y] == 1)
                {
                    std::cout << "#" << " ";
                    isGhost = true;
                }
                else if (grid[x][y] == 2)  //*表示玩家
                {
                    std::cout << "*" << " ";
                }
                else if (grid[x][y] == 3) //鬼
                {
                    std::cout << "+" << " ";
                    isGhost = true;
                }
                else if (grid[x][y] == 4) //鬼
                {
                    std::cout << "-" << " ";
                    isGhost = true;
                }
                else if (grid[x][y] == 5) //鬼
                {
                    std::cout << "=" << " ";
                    isGhost = true;
                }
                else if (grid[x][y] == 6) //鬼
                {
                    std::cout << "!" << " ";
                    isGhost = true;
                }
                else if (scoreGrid[x][y] == 1 && !isGhost)
                {
                    std::cout << "0" << " ";
                }
                else
                {
                    std::cout << "." << " ";
                }

                isGhost = false;
            }
            std::cout << std::endl;
        }
    }

};


#endif // !_MAP_H_
