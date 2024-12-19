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

        // �������飬��ʾ�����ƶ��ķ����ϡ��¡�����
        std::vector<Vector2> directions = { Vector2(0, -1), Vector2(0, 1), Vector2(-1, 0), Vector2(1, 0) };

        // ���ȶ��У���С�ѣ����洢�����б�
        std::priority_queue<Node*, std::vector<Node*>, std::function<bool(Node*, Node*)>> openSet(
            [](Node* a, Node* b) { return a->fCost() > b->fCost(); });
        std::unordered_set<Vector2> closedSet;

        Node* startNode = new Node(start, 0, start.distanceTo(goal), nullptr);
        openSet.push(startNode);

        while (!openSet.empty())
        {
            Node* currentNode = openSet.top();
            openSet.pop();

            // ���Ŀ���Ѿ��ҵ�����ʼ���򹹽�·��
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

            // ����ǰ�ڵ����ر��б�
            closedSet.insert(currentNode->position);

            // �����ھӽڵ�
            for (const auto& dir : directions)
            {
                Vector2 neighbor = currentNode->position + dir;

                // ����ھ��Ƿ�Ϊǽ�����Ѿ��ڹر��б���
                if (map->isWall(neighbor.x, neighbor.y) || closedSet.find(neighbor) != closedSet.end() || map->isGhost(neighbor.x, neighbor.y))
                    continue;

                // ���㵱ǰ�ھӵ� G �� H ֵ
                int newGCost = currentNode->gCost + 1;  // ����ÿ���ĳɱ�Ϊ 1
                int newHCost = neighbor.distanceTo(goal);

                // �����ھӽڵ㣬��������뿪���б�
                Node* neighborNode = new Node(neighbor, newGCost, newHCost, currentNode);
                openSet.push(neighborNode);
            }
        }


        return {};  // ���û���ҵ�·�������ؿ�
    }


    /// <summary>
    /// Ϊtrueʱ��׷����ң�Ϊfalseʱ���������
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
/// ��ɫ��꣬����׷�����
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
            position = path[1];  // ֻ�ߵ�һ��
        }
        map->DrawPlayer(position.x,position.y, 3);
        CheckCollision();  // ����Ƿ��������ײ

    //    std::cout << "Ghost Position is " << position.x << " " << position.y << std::endl;

    }


};


/// <summary>
/// ��ɫ��꣬׷�������ǰ��ǰ����
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

        //������ҳ��� �����˶�����
        switch (player->flip)
        {
        case PlayerFlip::Right:   target.y += 2;  break; // ������ҳ��ң�Ŀ�������ǰ������ 
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
            position = path[1];  // ֻ�ߵ�һ��
        }
        map->DrawPlayer(position.x, position.y, 4);
        CheckCollision();
    }
};  




/// <summary>
/// ��ɫ��꣬׷�ٺ�ɫ��꣬�����ǳ���Ϊ��ת
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
        case PlayerFlip::Right:   target.y -= 2;  break; // ������ҳ��ң�Ŀ�������ǰ������ 
        case PlayerFlip::Left: target.y += 2; break;
        case PlayerFlip::Up: target.x += 2; break;
        case PlayerFlip::Down: target.x -= 2; break;
        }

        std::vector<Vector2> path = aStar(position, target);
        if (!path.empty())
        {
            position = path[1];  // ֻ�ߵ�һ��
        }
        map->DrawPlayer(position.x, position.y, 5);
        CheckCollision();  // ����Ƿ��������ײ
    }
};  




/// <summary>
/// ��ɫ��꣬�����Ǳ���һ�����룬�����ǽ�Զʱ׷������
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

        std::cout << "����Ҿ���Ϊ" << " " << dist << std::endl;

        map->Clear(position);

        if (dist > 10)
        {
            std::vector<Vector2> path = aStar(position, player->position);
            std::cout << "������׷��" << std::endl;
            if (!path.empty()) 
            {
                position = path[1];  // ֻ�ߵ�һ��
            }
        }
        else 
        {
            std::cout << "������Զ��" << std::endl;
            // �������Ͻ�������ͼ������

            Vector2 target(18, 18);  // ��ͼ����

            if (position == target)
            {
                target.x -= 1;
            }

            std::vector<Vector2> path = aStar(position, target);
            if (!path.empty()) 
            {
                position = path[1];  // ֻ�ߵ�һ��
            }
        }
        map->DrawPlayer(position.x, position.y, 6);
        CheckCollision();  // ����Ƿ��������ײ
    }
};

#endif // !_Ghost_H_