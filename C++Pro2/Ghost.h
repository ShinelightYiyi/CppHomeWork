#ifndef _Ghost_H_
#define _Ghost_H_


#include"Vector2.h"  
#include"Player.h"
#include<queue>
#include<functional>
#include<unordered_set>

enum GhostState
{
	Attack,Run
};



class Ghost
{
public:
	Vector2 position;
	GhostState state;
	Player* player;
	Map* map;

    bool isDead;
    std::string name;

	Ghost(int x, int y, Player* p, Map* m,std::string n) : position(x, y), player(p), map(m),name(n){}  

	virtual void Update() = 0;


    std::vector<Vector2> aStar(Vector2 start, Vector2 goal)
    {
        struct Node
        {
            Vector2 position;
            int gCost, hCost;
            Node* parent;

            Node(Vector2 pos, int g, int h, Node* p)
                : position(pos), gCost(g), hCost(h), parent(p) {}

            int fCost() const { return gCost + hCost; }
        };

        // 方向数组，表示可以移动的方向：上、下、左、右
        std::vector<Vector2> directions = { Vector2(0, -1), Vector2(0, 1), Vector2(-1, 0), Vector2(1, 0) };

        // 优先队列（最小堆）来存储开放列表
        std::priority_queue<Node*, std::vector<Node*>, std::function<bool(Node*, Node*)>> openSet(
            [](Node* a, Node* b) { return a->fCost() > b->fCost(); });
        std::unordered_set<Vector2> closedSet;

        Node* startNode = new Node(start, 0, start.distanceTo(goal), nullptr);
        openSet.push(startNode);

        while (!openSet.empty())
        {
            Node* currentNode = openSet.top();
            openSet.pop();

            // 如果目标已经找到，开始反向构建路径
            if (currentNode->position == goal)
            {
                std::vector<Vector2> path;
                while (currentNode) {
                    path.push_back(currentNode->position);
                    currentNode = currentNode->parent;
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            // 将当前节点加入关闭列表
            closedSet.insert(currentNode->position);

            // 遍历邻居节点
            for (const auto& dir : directions)
            {
                Vector2 neighbor = currentNode->position + dir;

                // 检查邻居是否为墙或者已经在关闭列表中
                if (map->isWall(neighbor.x, neighbor.y) || closedSet.find(neighbor) != closedSet.end() || map->isGhost(neighbor.x, neighbor.y))
                    continue;

                // 计算当前邻居的 G 和 H 值
                int newGCost = currentNode->gCost + 1;  // 假设每步的成本为 1
                int newHCost = neighbor.distanceTo(goal);

                // 创建邻居节点，并将其加入开放列表
                Node* neighborNode = new Node(neighbor, newGCost, newHCost, currentNode);
                openSet.push(neighborNode);
            }
        }


        return {};  // 如果没有找到路径，返回空
    }


    /// <summary>
    /// 为true时，追击玩家，为false时，逃离玩家
    /// </summary>
    /// <param name="o"></param>
    void ChangeState(bool o)
    {
        if (o)
        {
            state = GhostState::Attack;
        }
        else
        {
            state = GhostState::Run;
        }
    }

    void Die()
    {
        isDead = true;
        std::cout << "Ghost" << " " << name << " is Died" << std::endl;
    }


    void CheckCollision()
    {
        switch (state)
        {
        case GhostState::Attack :
            if (position == player->position)
            {
                player->die();
            }
            break;
        case GhostState::Run :
            if (position == player->position)
            {
                Die();
            }
            break;
        }
    }

};   

/// <summary>
/// 红色鬼魂，不断追踪玩家
/// </summary>
class BlinkyGhost : virtual public Ghost
{
public:

    BlinkyGhost(int x,int y,Player* p,Map* m,std::string n) : Ghost(x,y,p,m,n)
    {
        state = GhostState::Attack;
    }

    void Update() override
    {
        map->Clear(position);
        std::vector<Vector2> path = aStar(position, player->position);
        if (!path.empty())
        {
            position = path[1];  // 只走第一步
        }
        map->DrawPlayer(position.x,position.y, 3);
        CheckCollision();  // 检查是否与玩家碰撞

    //    std::cout << "Ghost Position is " << position.x << " " << position.y << std::endl;

    }


};


/// <summary>
/// 粉色鬼魂，追踪玩家正前方前两格
/// </summary>
class PinkyGhost : virtual public Ghost
{
public:

    PinkyGhost(int x, int y, Player* p, Map* m, std::string n) :Ghost(x, y, p, m, n)
    {
        state = GhostState::Attack;
    }

    void Update() override
    {
        map->Clear(position);
        Vector2 target(player->position.x, player->position.y);

        //根据玩家朝向 更改运动方向
        switch (player->flip)
        {
        case PlayerFlip::Right:   target.y += 2;  break; // 假设玩家朝右，目标是玩家前方两格 
        case PlayerFlip::Left: target.y -= 2; break;  
        case PlayerFlip::Up: target.x -= 2; break;  
        case PlayerFlip::Down: target.x += 2; break;
        }

        std::cout << "The target is " << target.x << " " << target.y << std::endl;

        std::vector<Vector2> path;

        if (target.x <= 0 && target.y <= 0 && target.x >= map->width && target.y >= map->height)
        {
             path = aStar(position, player->position);
        }
        else
        {
             path = aStar(position, target);
        }


        if (!path.empty()) 
        {
            position = path[1];  // 只走第一步
        }
        map->DrawPlayer(position.x, position.y, 4);
        CheckCollision();
    }
};  




/// <summary>
/// 蓝色鬼魂，追踪红色鬼魂，以主角朝向为翻转
/// </summary>
class InkyGhost : virtual public Ghost
{
public:
    BlinkyGhost* blinky;

    InkyGhost(int x, int y, Player* p, Map* m, std::string n,Ghost* o) : Ghost(x, y, p, m, n),blinky(dynamic_cast<BlinkyGhost*>(o))
    {
        state = GhostState::Attack;
    }



    void Update() override
    {
        Vector2 target = player->position;


        std::cout << "Target Position is " << target.x << " " << target.y << std::endl;

        map->Clear(position);  


        switch (player->flip)
        {
        case PlayerFlip::Right:   target.y -= 2;  break; // 假设玩家朝右，目标是玩家前方两格 
        case PlayerFlip::Left: target.y += 2; break;
        case PlayerFlip::Up: target.x += 2; break;
        case PlayerFlip::Down: target.x -= 2; break;
        }

        std::vector<Vector2> path = aStar(position, target);
        if (!path.empty())
        {
            position = path[1];  // 只走第一步
        }
        map->DrawPlayer(position.x, position.y, 5);
        CheckCollision();  // 检查是否与玩家碰撞
    }
};  




/// <summary>
/// 橙色鬼魂，和主角保持一定距离，离主角较远时追击主角
/// </summary>
class ClydeGhost : virtual public Ghost
{
public:

    ClydeGhost(int x, int y, Player* p, Map* m, std::string n) : Ghost(x, y, p, m, n)
    {
        state = GhostState::Attack;
    }


    void Update() override 
    {
        int dist = position.distanceTo(player->position);

        std::cout << "与玩家距离为" << " " << dist << std::endl;

        map->Clear(position);

        if (dist > 10)
        {
            std::vector<Vector2> path = aStar(position, player->position);
            std::cout << "现在是追击" << std::endl;
            if (!path.empty()) 
            {
                position = path[1];  // 只走第一步
            }
        }
        else 
        {
            std::cout << "现在是远离" << std::endl;
            // 如果距离较近，往地图角落走

            Vector2 target(18, 18);  // 地图角落

            if (position == target)
            {
                target.x -= 1;
            }

            std::vector<Vector2> path = aStar(position, target);
            if (!path.empty()) 
            {
                position = path[1];  // 只走第一步
            }
        }
        map->DrawPlayer(position.x, position.y, 6);
        CheckCollision();  // 检查是否与玩家碰撞
    }
};

#endif // !_Ghost_H_