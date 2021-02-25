#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <array>
#include <algorithm>
#include "Actor.h"

const int N = VIEW_HEIGHT/SPRITE_HEIGHT;
const int M = VIEW_HEIGHT/(4*SPRITE_HEIGHT);
const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    // Initialize GhostRacer
    m_player = new GhostRacer(this, 128, 32);
    
    // Initialize BorderLine
        
        //Initialize Left Yellow Borderline
        for(int j = 0; j < N; j++)
        {
            int yValue = j*SPRITE_HEIGHT;
            
            m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE, LEFT_EDGE, yValue));
            
            //numberOfObjects++;
        }
    
        //Initialize Right Yellow Borderline
        for(int j = 0; j < N; j++)
        {
            int yValue = j*SPRITE_HEIGHT;
            
            m_actors.push_front(new BorderLine(this, IID_YELLOW_BORDER_LINE, RIGHT_EDGE, yValue));
            
            //numberOfObjects++;
        }
    
        //Initialize Left White Borderline
        for(int j = 0; j < M; j++)
        {
            int xValue = LEFT_EDGE + ROAD_WIDTH/3;
            int yValue = j*(4*SPRITE_HEIGHT);
            
            m_actors.push_front(new BorderLine(this, IID_WHITE_BORDER_LINE, xValue, yValue));
            
            //numberOfObjects++;
        }
        
        //Initialize Right White Borderline
        for(int j = 0; j < M; j++)
        {
            int xValue = RIGHT_EDGE - ROAD_WIDTH/3;
            int yValue = j*(4*SPRITE_HEIGHT);
            
            m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, xValue, yValue));
            
            //numberOfObjects++;
        }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    // Tell ghost racer to move
    if (m_player->isAlive())
    {
        m_player->doSomething();
    }
    
    // Actor Actions
    list<Actor*>::iterator it = m_actors.begin();
    
    for(; it != m_actors.end(); it++)
    {
        if((*it)->isAlive())
        {
            (*it)->doSomething();
            
            if(isCollisionWorthyObjects(*it))
            {
                if((*it)->isHit())
                {
                    return (*it)->doSomethingWhenHit();
                }
            }
        }
        
        if(!getPlayer()->isAlive())
        {
            return GWSTATUS_PLAYER_DIED;
        }
        
        if(getSoulSaved() == 2*getLevel()+5)
        {
            this->playSound(SOUND_FINISHED_LEVEL);
            resetSoulSaved();
            increaseScore(getBonusPoints());
            resetBonusPoints();
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    //Delete all dead objects
    it = m_actors.begin();
    
    for(; it != m_actors.end(); it++)
    {
        if(!(*it)->isAlive())
        {
            delete *it;
            //numberOfObjects--;
            m_actors.erase(it);
            it--;
        }
    }
    
    //Create new objects
    
    //Borderline
    changeLastY(m_actors.back()->getY());
    
    int new_border_y = VIEW_HEIGHT-SPRITE_HEIGHT;
    int delta_y = new_border_y - lastY;
    
    if(delta_y >= SPRITE_HEIGHT)
    {
        m_actors.push_front(new BorderLine(this, IID_YELLOW_BORDER_LINE, ROAD_CENTER-ROAD_WIDTH/2, new_border_y));
        m_actors.push_front(new BorderLine(this, IID_YELLOW_BORDER_LINE, ROAD_CENTER+ROAD_WIDTH/2, new_border_y));
        
        //numberOfObjects += 2;
    }
    
    if(delta_y >= 4*SPRITE_HEIGHT)
    {
        m_actors.push_front(new BorderLine(this, IID_WHITE_BORDER_LINE, ROAD_CENTER-(ROAD_WIDTH/2)+(ROAD_WIDTH/3), new_border_y));
        m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, ROAD_CENTER+(ROAD_WIDTH/2)-(ROAD_WIDTH/3), new_border_y));
        
        //numberOfObjects += 2;
    }
    
    //Human Pedestrian
    int ChanceHumanPed = max(200 - (getLevel()*10), 30);
    
    int random = randInt(0, ChanceHumanPed-1);
    
    if(random == 0)
    {
        int rand_x = randInt(0, VIEW_WIDTH);
        m_actors.push_front(new HumanPedestrian(this, rand_x, VIEW_HEIGHT));
    }
    
    //Zombie Pedestrian
    int ChanceZombiePed = max(100 - (getLevel()*10), 20);
    
    random = randInt(0, ChanceZombiePed-1);
    
    if(random == 0)
    {
        int rand_x = randInt(0, VIEW_WIDTH);
        m_actors.push_front(new ZombiePedestrian(this, rand_x, VIEW_HEIGHT));
    }
    
    //Oil Slick
    int ChanceOilSlick = max(150 - (getLevel()*10), 40);
    
    random = randInt(0, ChanceOilSlick-1);
    
    if(random == 0)
    {
        int rand_x = randInt(LEFT_EDGE, RIGHT_EDGE);
        m_actors.push_front(new OilSlick(this, rand_x, VIEW_HEIGHT));
    }
    
    //Holy Water Refill Goodies
    
    int ChanceOfHolyWater = 100 + 10 * getLevel();
    random = randInt(0, ChanceOfHolyWater-1);
    
    if(random == 0)
    {
        int rand_x = randInt(LEFT_EDGE, RIGHT_EDGE);
        m_actors.push_front(new HolyWaterGoodie(this, rand_x, VIEW_HEIGHT));
    }
    
    //Lost Soul Goodies
    
    int ChanceOfLostSoul = 100;
    random = randInt(0, ChanceOfLostSoul-1);
    
    if(random == 0)
    {
        int rand_x = randInt(LEFT_EDGE, RIGHT_EDGE);
        m_actors.push_front(new SoulGoodie(this, rand_x, VIEW_HEIGHT));
    }
    
    // Zombie Cab
    
    int ChanceVehicle = max(100 - (getLevel()*10), 20);
    random = randInt(0, ChanceVehicle-1);
    
    if(random == 0)
    {
        determineZombieCabSpawn();
    }
    
    //Set Game Stats
    ostringstream oss;
    oss << "Score: " << getScore() << setw(7) << "Lvl: " << getLevel() << setw(14) << "Souls2Save: " << 2*getLevel()+5 - getSoulSaved() << setw(9) << "Lives: " << getLives() << setw(10) << "Health: " << getPlayer()->getHP() << setw(10) << "Sprays: " << getPlayer()->getSpray() << setw(9) << "Bonus: " << getBonusPoints();
    string s = oss.str();
    
    setGameStatText(s);
    
    reduceBonusPoints();
    
    //Continue Game
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if(m_player != nullptr)
        delete m_player;
    
    list<Actor*>::iterator it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        delete *it;
        //numberOfObjects--;
        //cout << "Number of Objects is " << numberOfObjects << endl;
        it = m_actors.erase(it);
    }
    
    m_player = nullptr;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

GhostRacer* StudentWorld::getPlayer() const
{
    return m_player;
}

void StudentWorld::changeLastY(int new_y)
{
    lastY = new_y;
}

int StudentWorld::getLastY() const
{
    return lastY;
}

bool StudentWorld::overlaps(Actor* subject, Actor* object) const
{
    int delta_x = abs(subject->getX() - object->getX());
    int delta_y = abs(subject->getY() - object->getY());
    
    int sum_radius = subject->getRadius() + object->getRadius();
    
    if(delta_x < sum_radius*0.25 && delta_y < sum_radius*0.6)
    {
        return true;
    }
    
    return false;
    
}

void StudentWorld::recordSoulSaved()
{
    m_soulSaved += 1;
}

int StudentWorld::getSoulSaved() const
{
    return m_soulSaved;
}

void StudentWorld::resetSoulSaved()
{
    m_soulSaved = 0;
}

bool StudentWorld::isCollisionWorthyObjects(Actor* p)
{
    if(p->isCollisionAvoidanceWorthy())
    {
        return true;
    }
    
    return false;
}

bool StudentWorld::sprayFirstAppropriateActor(Actor *p)
{
    list<Actor*>::iterator it = m_actors.begin();
    
    for(; it != m_actors.end(); it++)
    {
        if(overlaps(p, *it))
        {
            if((*it)->beSprayedIfAppropriate())
                return true;
        }
    }
    return false;
}

void StudentWorld::addNewSpray(double x, double y, int dir)
{
    m_actors.push_front(new Spray(this, x, y, dir));
}

void StudentWorld::addNewHealingGoodie(double x, double y)
{
    m_actors.push_front(new HealingGoodie(this, x, y));
}
bool StudentWorld::zombieCabDetect(ZombieCab *p, int direction)
{
    int current_x = p->getX();
    int x_min = 0;
    int x_max = 0;
    
    //Determine lane
    if(current_x >= LEFT_EDGE && current_x < LEFT_EDGE + ROAD_WIDTH/3)
    {
        x_min = LEFT_EDGE;
        x_max = LEFT_EDGE + ROAD_WIDTH/3;
    }
    else if(current_x >= LEFT_EDGE + ROAD_WIDTH/3 && current_x < RIGHT_EDGE-ROAD_WIDTH/3)
    {
        x_min = LEFT_EDGE+ROAD_WIDTH/3;
        x_max = RIGHT_EDGE-ROAD_WIDTH/3;
    }
    else
    {
        x_min = RIGHT_EDGE-ROAD_WIDTH/3;
        x_max = RIGHT_EDGE;
    }
    
    int current_y = p->getY();
    
    int closest_y = 99999;
    
    Actor* target = nullptr;
    
    // Check for collision-avoidance worthy actor BEHIND zombie cab
    if(direction == 0)
    {
        list<Actor*>::iterator it = m_actors.begin();
        
        for(; it != m_actors.end(); it++)
        {
            int object_x = (*it)->getX();
            
            if((*it)->isCollisionAvoidanceWorthy() && object_x >= x_min && object_x < x_max && (*it) != p)
            {
                int difference = (*it)->getY() - current_y;
                if(abs(difference) < closest_y && difference < 0)
                {
                    closest_y = difference;
                    target = (*it);
                }
            }
        }
        
        if(getPlayer()->getX() >= x_min && getPlayer()->getX() < x_max)
        {
            int difference = getPlayer()->getY() - current_y;
            if(abs(difference) < closest_y && difference < 0)
            {
                closest_y = difference;
                target = getPlayer();
            }
        }
    }
    
    // Check for collision-avoidance worthy actor in FRONT zombie cab
    else if (direction == 1)
    {
        list<Actor*>::iterator it = m_actors.begin();
        
        for(; it != m_actors.end(); it++)
        {
            int object_x = (*it)->getX();
            
            if((*it)->isCollisionAvoidanceWorthy() && object_x >= x_min && object_x < x_max && (*it) != p)
            {
                int difference = (*it)->getY() - current_y;
                if(abs(difference) < closest_y && difference > 0)
                {
                    closest_y = difference;
                    target = (*it);
                }
            }
        }
        
        if(getPlayer()->getX() >= x_min && getPlayer()->getX() < x_max)
        {
            int difference = getPlayer()->getY() - current_y;
            if(abs(difference) < closest_y && difference > 0)
            {
                closest_y = difference;
                target = (*it);
            }
        }
    }
    
    if(abs(closest_y) < 96 && target != nullptr && target != getPlayer())
    {
        return true;
    }
    
    return false;
}

void StudentWorld::determineZombieCabSpawn()
{
    //Set found to false
    bool foundSpawnPoint = false;
    
    //Get Order
    int cur_lane = randInt(0, 2);
    int first = cur_lane;
    
    while(cur_lane == first)
    {
        cur_lane = randInt(0, 2);
    }
    
    int second = cur_lane;
    
    while(cur_lane == first || cur_lane == second)
    {
        cur_lane = randInt(0, 2);
    }
    
    int third = cur_lane;
    
    //Input order into an array
    int order[3] = {first, second, third};
    
    //Initialize Boundaries
    int leftBoundary;
    int rightBoundary;
    
    //Initialize chosen values
    int chosenLane;
    int chosenY;
    int chosenX;
    int chosenSpeed;
    
    //Loop 3 times for each lane
    for(int i = 0; i < 2; i++)
    {
        cur_lane = order[i];
        
        int closest = 999;
        
        if(cur_lane == 0)
        {
            leftBoundary = LEFT_EDGE;
            rightBoundary = LEFT_EDGE + ROAD_WIDTH/3;
            
        }
        else if(cur_lane == 1)
        {
            leftBoundary = LEFT_EDGE + ROAD_WIDTH/3;
            rightBoundary = RIGHT_EDGE - ROAD_WIDTH/3;
        }
        
        else
        {
            leftBoundary = RIGHT_EDGE - ROAD_WIDTH/3;
            rightBoundary = RIGHT_EDGE;
        }
        
        //Check for closest collision avoidance-worhty actors from the BOTTOM
        Actor* target = nullptr;
        
        list<Actor*>::iterator it = m_actors.begin();
        
        for(; it != m_actors.end(); it++)
        {
            int cur_x = (*it)->getX();
            int cur_y = (*it)->getY();
            
            if(cur_x >= leftBoundary && cur_x < rightBoundary && (*it)->isCollisionAvoidanceWorthy())
            {
                if(cur_y < closest)
                {
                    closest = cur_y;
                    target = *it;
                }
            }
        }
        
        if(getPlayer()->getX() >= leftBoundary && getPlayer()->getX() < rightBoundary)
        {
            {
                if(getPlayer()->getY() < closest)
                {
                    closest = getPlayer()->getY();
                    target = getPlayer();
                }
            }
        }
        
        if(target == nullptr)
        {
            chosenLane = cur_lane;
            chosenY = SPRITE_HEIGHT/2;
            chosenSpeed = getPlayer()->getVerticalSpeed() + randInt(2, 4);
            foundSpawnPoint = true;
            break;
        }
        else if (target != nullptr && target->getY() > VIEW_HEIGHT/3)
        {
            chosenLane = cur_lane;
            chosenY = SPRITE_HEIGHT/2;
            chosenSpeed = getPlayer()->getVerticalSpeed() + randInt(2, 4);
            foundSpawnPoint = true;
            break;
        }
        
        
        //Check for collision avoidance-worthy actors from the TOP
        target = nullptr;
        it = m_actors.begin();
        closest = 999;
        
        for(; it != m_actors.end(); it++)
        {
            int cur_x = (*it)->getX();
            int cur_y = (*it)->getY();
            
            if(cur_x >= leftBoundary && cur_x < rightBoundary && (*it)->isCollisionAvoidanceWorthy())
            {
                if(255-cur_y < closest)
                {
                    closest = cur_y;
                    target = *it;
                }
            }
        }
        
        if(getPlayer()->getX() >= leftBoundary && getPlayer()->getX() < rightBoundary)
        {
            {
                if(255-getPlayer()->getY() < closest)
                {
                    closest = getPlayer()->getY();
                    target = getPlayer();
                }
            }
        }
        
        if(target == nullptr)
        {
            chosenLane = cur_lane;
            chosenY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
            chosenSpeed = getPlayer()->getVerticalSpeed() - randInt(2, 4);
            foundSpawnPoint = true;
            break;
        }
        
        else if (target != nullptr && target->getY() < (VIEW_HEIGHT * 2/3))
        {
            chosenLane = cur_lane;
            chosenY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
            chosenSpeed = getPlayer()->getVerticalSpeed() - randInt(2, 4);
            foundSpawnPoint = true;
            break;
        }
        
    }
    
    //If spawn point cannot be located, skip the tick
    if(foundSpawnPoint == false)
        return;
    else
    {
        if(chosenLane == 0)
        {
            chosenX = ROAD_CENTER-(ROAD_WIDTH/3);
        }
        else if(chosenLane == 1)
        {
            chosenX = ROAD_CENTER;
        }
        else
        {
            chosenLane = ROAD_CENTER + (ROAD_WIDTH/3);
        }
        
        m_actors.push_front(new ZombieCab(this, chosenX, chosenY, chosenSpeed));
    }
}

void StudentWorld::reduceBonusPoints()
{
    m_bonusPoints -= 1;
}

int StudentWorld::getBonusPoints() const
{
    return m_bonusPoints;
}

void StudentWorld::resetBonusPoints()
{
    m_bonusPoints = 5000;
}
