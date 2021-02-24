#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <cmath>

const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor Implementations

Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, double size, int depth, int verticalSpeed, int horizontalSpeed): GraphObject(imageID, startX, startY, startDirection, size, depth), m_world(world), m_verticalSpeed(verticalSpeed), m_horizontalSpeed(horizontalSpeed)
{
    
}

//Get World
StudentWorld* Actor::getWorld() const
{
    return m_world;
}

//Actor Speed
int Actor::getVerticalSpeed() const
{
    return m_verticalSpeed;
}

int Actor::getHorizontalSpeed() const
{
    return m_horizontalSpeed;
}

void Actor::changeVerticalSpeed(int change)
{
    m_verticalSpeed += change;
}

void Actor::changeHorizontalSpeed(int change)
{
    m_horizontalSpeed += change;
}

void Actor::setVerticalSpeed(int set)
{
    m_verticalSpeed = set;
}

void Actor::setHorizontalSpeed(int set)
{
    m_horizontalSpeed = set;
}

//Actor alive/dead status
bool Actor::isAlive() const
{
    return m_alive;
}

bool Actor::isDamagable() const
{
    return true;
}

void Actor::killed()
{
    this->m_alive = false;
}

bool Actor::isHit()
{
    return m_isHit;
}

void Actor::gotHit()
{
    m_isHit = true;
}

//Actor Collision
bool Actor::isCollisionAvoidanceWorthy() const
{
    return false;
}


// Agent Implementations

Agent::Agent(StudentWorld* world, int imageID, double x, double y, int dir, double size, int verticalSpeed, int horizontalSpeed, int hp): Actor(world, imageID, x, y, dir, size, 0, verticalSpeed, horizontalSpeed), m_health(hp)
{
    
}

int Agent::getHP() const
{
    return m_health;
}

void Agent::reduceHP(int reduction)
{
    m_health -= reduction;
}

void Agent::increaseHP(int hp)
{
    m_health += hp;
}

// Ghost Racer Implementations
GhostRacer::GhostRacer(StudentWorld* world, double x, double y): Agent(world, IID_GHOST_RACER, x, y, 90, 4.0, 0, 0, 100), m_spray(10)
{
    
}

void GhostRacer::moveGhostRacer()
{
    double max_shift_per_tick = 4.0;
    double direction = this->getDirection();
    double delta_x = cos(direction*(PI/180.0)) * max_shift_per_tick;
    double cur_x = this->getX();
    double cur_y = this->getY();
    
    moveTo(cur_x + delta_x, cur_y);
}

void GhostRacer::doSomething()
{
    if(!isAlive())
        return;
    
    int ch;
    
    if(getX() <= LEFT_EDGE)
    {
        if(getDirection() > 90)
        {
            setDirection(82);
        }
    }
    
    if(getX() >= RIGHT_EDGE)
    {
        if(getDirection() < 90)
        {
            setDirection(98);
        }
    }
    
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
            {
                if(this->getDirection() < 114)
                {
                    setDirection(this->getDirection() + 8);
                }
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                if(getDirection() > 66)
                {
                    setDirection(getDirection() - 8);
                }
                break;
            }
            case KEY_PRESS_UP:
            {
                if(getVerticalSpeed() < 5)
                {
                    changeVerticalSpeed(1);
                }
                
                break;
            }
                
            case KEY_PRESS_DOWN:
            {
                if(getVerticalSpeed() > -1)
                {
                    changeVerticalSpeed(-1);
                }
                break;
            } 
        }
    }
    moveGhostRacer();
    
}

int GhostRacer::getSpray() const
{
    return m_spray;
}

// Border Line Implementations

BorderLine::BorderLine(StudentWorld* world, int imageID, double startX, double startY): Actor(world, imageID, startX, startY, 0, 2.0, 1, -4, 0)
{
    
}

void BorderLine::doSomething()
{
    int vert_speed = this->getVerticalSpeed() - getWorld()->getPlayer()->getVerticalSpeed();
    
    int horiz_speed = this->getHorizontalSpeed();
    
    int new_y = this->getY() + vert_speed;
    int new_x = this->getX() + horiz_speed;
    
    this->moveTo(new_x, new_y);
    
    
    if(this->getX() < 0 || this->getX() > VIEW_WIDTH || this->getY() < 0 || this->getY() > VIEW_HEIGHT)
    {
        this->killed();
        return;
    }
    
}


// Pedestrian Implementations

Pedestrian:: Pedestrian(StudentWorld* world, int imageID, double startX, double startY, double size): Agent(world, imageID, startX, startY, 0, size, -4, 0, 2)
{
    
}

bool Pedestrian::isCollisionAvoidanceWorthy() const
{
    return true;
}

void Pedestrian::moveAndPossiblyPickPlan()
{
    int vert_speed = getVerticalSpeed() - getWorld()->getPlayer()->getVerticalSpeed();
    int horiz_speed = getHorizontalSpeed();
    
    int new_y = getY() + vert_speed;
    int new_x = getX() + horiz_speed;
    
    this->moveTo(new_x, new_y);
    
    if(getX() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT || getY() < 0)
    {
        this->killed();
        return;
    }
    
}

void Pedestrian::decrementMovementPlan()
{
    m_movementPlanDistance -= 1;
}

int Pedestrian::getMovementPlanDistance() const
{
    return m_movementPlanDistance;
}

void Pedestrian::setMovementPlanDistance(int distance)
{
    m_movementPlanDistance = distance;
}

// Human Pedestrian Implementations

HumanPedestrian:: HumanPedestrian(StudentWorld* world, double startX, double startY): Pedestrian(world, IID_HUMAN_PED, startX, startY, 2.0)
{

}

void HumanPedestrian::doSomething()
{
    if(!isAlive())
        return;
    
    if(getWorld()->overlaps(this, getWorld()->getPlayer()))
    {
        this->gotHit();
        return;
    }
    
    moveAndPossiblyPickPlan();
    
    decrementMovementPlan();
    
    if(getMovementPlanDistance() > 0)
        return;
    
    int random = randInt(-3, 3);
    
    while(random == 0)
    {
        random = randInt(-3, 3);
    }
    
    setHorizontalSpeed(random);
    
    random = randInt(4, 32);
    
    setMovementPlanDistance(random);
    
    if(getHorizontalSpeed() < 0)
    {
        setDirection(180);
    }
    else
    {
        setDirection(0);
    }

}

int HumanPedestrian::doSomethingWhenHit()
{
    getWorld()->decLives();
    return GWSTATUS_PLAYER_DIED;
}

//Zombie Pedestrian Implementations

ZombiePedestrian::ZombiePedestrian(StudentWorld* world, double x, double y):Pedestrian(world, IID_ZOMBIE_PED, x, y, 3.0)
{
    
}

void ZombiePedestrian::doSomething()
{
    if(!isAlive())
        return;
    
    if(getWorld()->overlaps(this, getWorld()->getPlayer()))
    {
        gotHit();
        killed();
        return;
    }
    
    if(abs(getWorld()->getPlayer()->getX() - getX()) <= 30 && getY() > getWorld()->getPlayer()->getY())
    {
        setDirection(270);
        
        if(getX() < getWorld()->getPlayer()->getX())
        {
            setHorizontalSpeed(1);
        }
        else if(getX() > getWorld()->getPlayer()->getX())
        {
            setHorizontalSpeed(-1);
        }
        else
        {
            setHorizontalSpeed(0);
        }
        
        changeUntilGrunt(-1);
        
        if(getUntilGrunt() <= 0)
        {
            getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
            setUntilGrunt(20);
        }
    }
    moveAndPossiblyPickPlan();
    
    if(getMovementPlanDistance() > 0)
    {
        decrementMovementPlan();
        return;
    }
    
    int random = randInt(-3, 3);
    
    while(random == 0)
    {
        random = randInt(-3, 3);
    }
    
    setHorizontalSpeed(random);
    
    random = randInt(4, 32);
    
    setMovementPlanDistance(random);
    
    if(getHorizontalSpeed() < 0)
    {
        setDirection(180);
    }
    else
    {
        setDirection(0);
    }
}


int ZombiePedestrian::doSomethingWhenHit()
{
    getWorld()->getPlayer()->reduceHP(5);
    return GWSTATUS_CONTINUE_GAME;
}

int ZombiePedestrian::getUntilGrunt() const
{
    return m_untilGrunt;
}

void ZombiePedestrian::setUntilGrunt(int ticks)
{
    m_untilGrunt = ticks;
}

void ZombiePedestrian::changeUntilGrunt(int ticks)
{
    m_untilGrunt += ticks;
}
