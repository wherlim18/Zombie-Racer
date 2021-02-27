#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>

const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
const double pi = 3.14159265;

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor Implementations

Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, double size, int depth, double verticalSpeed, double horizontalSpeed): GraphObject(imageID, startX, startY, startDirection, size, depth), m_world(world), m_verticalSpeed(verticalSpeed), m_horizontalSpeed(horizontalSpeed)
{
    
}

//Get World
StudentWorld* Actor::getWorld() const
{
    return m_world;
}

//Actor Speed
double Actor::getVerticalSpeed() const
{
    return m_verticalSpeed;
}

double Actor::getHorizontalSpeed() const
{
    return m_horizontalSpeed;
}

void Actor::changeVerticalSpeed(double change)
{
    m_verticalSpeed += change;
}

void Actor::changeHorizontalSpeed(double change)
{
    m_horizontalSpeed += change;
}

void Actor::setVerticalSpeed(double set)
{
    m_verticalSpeed = set;
}

void Actor::setHorizontalSpeed(double set)
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
int Actor::doSomethingWhenHit()
{
    return -1;
}

//Actor Spray
bool Actor::beSprayedIfAppropriate()
{
    return false;
}

bool Actor::moveRelativeToGhostRacerVerticalSpeed(double dx)
{
    double vert_speed = getVerticalSpeed() - getWorld()->getPlayer()->getVerticalSpeed();
    
    double horiz_speed = dx;
    
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;
    
    moveTo(new_x, new_y);
    
    
    if(this->getX() < 0 || this->getX() > VIEW_WIDTH || this->getY() < 0 || this->getY() > VIEW_HEIGHT)
    {
        killed();
        return false;
    }
    
    return true;
}

// Agent Implementations

Agent::Agent(StudentWorld* world, int imageID, double x, double y, int dir, double size, double verticalSpeed, double horizontalSpeed, int hp): Actor(world, imageID, x, y, dir, size, 0, verticalSpeed, horizontalSpeed), m_health(hp)
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

bool Agent::isCollisionAvoidanceWorthy() const
{
    return true;
}

// Ghost Racer Implementations
GhostRacer::GhostRacer(StudentWorld* world, double x, double y): Agent(world, IID_GHOST_RACER, x, y, 90, 4.0, 0, 0, 100), m_spray(10)
{
    
}

void GhostRacer::moveGhostRacer()
{
    double max_shift_per_tick = 4.0;
    double direction = this->getDirection();
    double delta_x = cos(direction*(pi/180.0)) * max_shift_per_tick;
    double cur_x = this->getX();
    double cur_y = this->getY();
    
    moveTo(cur_x + delta_x, cur_y);
}

void GhostRacer::doSomething()
{
    if(!isAlive())
        return;
    
    if(getHP() <= 0)
    {
        killed();
    }
    
    int ch;
    
    if(getX() <= LEFT_EDGE)
    {
        if(getDirection() > 90)
        {
            reduceHP(10);
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
            setDirection(82);
        }
    }
    
    if(getX() >= RIGHT_EDGE)
    {
        if(getDirection() < 90)
        {
            reduceHP(10);
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
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
            
            case KEY_PRESS_SPACE:
            {
                if(getSpray() >= 1)
                {
                    double cur_dir = getDirection();
                    double abs_dir = abs(90-cur_dir);
                    double delta_x;
                    double delta_y;
                    double new_x;
                    double new_y;
                    
                    delta_x = sin(abs_dir * (pi/180)) * SPRITE_HEIGHT;
                    delta_y = cos(abs_dir * (pi/180)) * SPRITE_HEIGHT;
                    
                    if(cur_dir > 90)
                    {
                        new_x = getX() - delta_x;
                        new_y = getY() + delta_y;
                        getWorld()->addNewSpray(new_x, new_y, cur_dir);
                    }
                    else if (cur_dir < 90)
                    {
                        new_x = getX() + delta_x;
                        new_y = getY() + delta_y;
                        getWorld()->addNewSpray(new_x, new_y, cur_dir);
                    }
                    else
                    {
                        new_y = getY() + SPRITE_HEIGHT;
                        getWorld()->addNewSpray(getX(), new_y, cur_dir);
                    }
                    
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                    increaseSprays(-1);
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

void GhostRacer::increaseSprays(int amount)
{
    m_spray += amount;
}

// Ghost Racer Activated Objects Implementations
 
GhostRacerActivatedObject::GhostRacerActivatedObject(StudentWorld* world, int imageID, double x, double y, double size, int dir): Actor(world, imageID, x, y, dir, size, 2, -4, 0)
{
    
}

bool GhostRacerActivatedObject::beSprayedIfAppropriate()
{
    if(isSprayable())
    {
        if(selfDestructs())
        {
            killed();
        }
        return true;
    }
    return false;
}

// Border Line Implementations

BorderLine::BorderLine(StudentWorld* world, int imageID, double startX, double startY): Actor(world, imageID, startX, startY, 0, 2.0, 2, -4, 0)
{
    
}

void BorderLine::doSomething()
{
    double horizSpeed = getHorizontalSpeed();
    moveRelativeToGhostRacerVerticalSpeed(horizSpeed);
    
}

// Spray Implementations

Spray::Spray(StudentWorld* world, double x, double y, int dir): Actor(world, IID_HOLY_WATER_PROJECTILE, x, y, dir, 1.0, 1, 0, 0), m_maximumTravel(160)
{
    
}

int Spray::getMaximumTravel() const
{
    return m_maximumTravel;
}

void Spray::setMaximumTravel(int max)
{
    m_maximumTravel = max;
}

void Spray::doSomething()
{
    if(!isAlive())
        return;
    
    if(getWorld()->sprayFirstAppropriateActor(this))
    {
        killed();
        return;
    }
    
    moveForward(SPRITE_HEIGHT);
    setMaximumTravel(getMaximumTravel() - SPRITE_HEIGHT);
    
    
    if(getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT)
    {
        killed();
        return;
    }
    
    if(getMaximumTravel() <= 0)
    {
        killed();
        return;
    }
    
    
}

// Pedestrian Implementations

Pedestrian:: Pedestrian(StudentWorld* world, int imageID, double startX, double startY, double size): Agent(world, imageID, startX, startY, 0, size, -4, 0, 2)
{
    
}

void Pedestrian::moveAndPossiblyPickPlan()
{
    double horizSpeed = getHorizontalSpeed();
    moveRelativeToGhostRacerVerticalSpeed(horizSpeed);
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

bool HumanPedestrian::beSprayedIfAppropriate()
{
    setHorizontalSpeed(getHorizontalSpeed()*(-1));
    setDirection(180-getDirection());
    getWorld()->playSound(SOUND_PED_HURT);
    return true;
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
    getWorld()->resetSoulSaved();
    getWorld()->resetBonusPoints();
    return GWSTATUS_PLAYER_DIED;
}

//Zombie Pedestrian Implementations

ZombiePedestrian::ZombiePedestrian(StudentWorld* world, double x, double y): Pedestrian(world, IID_ZOMBIE_PED, x, y, 3.0)
{
    
}

void ZombiePedestrian::doSomething()
{
    if(!isAlive())
        return;
    
    if(getWorld()->overlaps(this, getWorld()->getPlayer()))
    {
        gotHit();
        getWorld()->playSound(SOUND_PED_DIE);
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

bool ZombiePedestrian::beSprayedIfAppropriate()
{
    reduceHP(1);
    
    if(getHP() <= 0)
    {
        getWorld()->playSound(SOUND_PED_DIE);
        
        killed();
        
        int random = randInt(1, 5);
        
        if(random == 1)
        {
            if(!getWorld()->overlaps(this, getWorld()->getPlayer()))
            {
                getWorld()->addNewHealingGoodie(getX(), getY());
            }
        }
        
        getWorld()->increaseScore(150);
    }
    else
    {
        getWorld()->playSound(SOUND_PED_HURT);
    }
    
    return true;
}

// Zombie Cab Implementations

ZombieCab::ZombieCab(StudentWorld* world, double x, double y, double verticalSpeed): Agent(world, IID_ZOMBIE_CAB, x, y, 90, 4.0, verticalSpeed, 0, 3)
{
    
}

bool ZombieCab::hasDamaged() const
{
    return m_hasDamaged;
}

void ZombieCab::isDamaging()
{
    m_hasDamaged = true;
}

bool ZombieCab::beSprayedIfAppropriate()
{
    reduceHP(1);
    
    if(getHP() <= 0)
    {
        getWorld()->playSound(SOUND_VEHICLE_DIE);
        
        killed();
        
        int random = randInt(1, 5);
        
        if(random == 1)
        {
            getWorld()->addNewOilSlick(getX(), getY());
        }
        
        getWorld()->increaseScore(200);
        
        return true;
    }
    else
    {
        getWorld()->playSound(SOUND_VEHICLE_HURT);
    }
    
    return true;
}

void ZombieCab::moveAndPossiblyPickPlan()
{
    double horizSpeed = getHorizontalSpeed();
    moveRelativeToGhostRacerVerticalSpeed(horizSpeed);
}

int ZombieCab::getMovementPlanDistance() const
{
    return m_movementPlanDistance;
}

void ZombieCab::setMovementPlanDistance(int distance)
{
    m_movementPlanDistance = distance;
}

void ZombieCab::doSomething()
{
    if(!isAlive())
        return;
    
    if(getWorld()->overlaps(this, getWorld()->getPlayer()))
    {
        if(!hasDamaged())
        {
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
            getWorld()->getPlayer()->reduceHP(20);
            
            if(getX() <= getWorld()->getPlayer()->getX())
            {
                setHorizontalSpeed(-5);
                
                int random = randInt(0, 19);
                
                setDirection(120+random);
            }
            else
            {
                setHorizontalSpeed(5);
                
                int random = randInt(0, 19);
                
                setDirection(60-random);
            }
            
            isDamaging();
        }
    }
    
    moveAndPossiblyPickPlan();
    
    double ghostRacerSpeed = getWorld()->getPlayer()->getVerticalSpeed();
    double cabSpeed = getVerticalSpeed();
    
    if(cabSpeed > ghostRacerSpeed)
    {
        if(getWorld()->zombieCabDetect(this, 1))
        {
            setVerticalSpeed(cabSpeed - 0.5);
            return;
        }
    }
   
    if(cabSpeed <= ghostRacerSpeed)
    {
        if(getWorld()->zombieCabDetect(this, 0))
        {
            setVerticalSpeed(cabSpeed + 0.5);
            return;
        }
    }
    
    setMovementPlanDistance(getMovementPlanDistance()-1);
    
    if(getMovementPlanDistance() > 0)
        return;
    
    setMovementPlanDistance(randInt(4, 32));
    
    changeVerticalSpeed(randInt(-2, 2));
    
}


// Oil Slick Implementations

OilSlick::OilSlick(StudentWorld* world, double x, double y): GhostRacerActivatedObject(world, IID_OIL_SLICK, x, y, randInt(2, 5), 0)
{
    
}

void OilSlick::doSomething()
{
    double horizSpeed = getHorizontalSpeed();
    moveRelativeToGhostRacerVerticalSpeed(horizSpeed);
    
    if(getWorld()->overlaps(this, getWorld()->getPlayer()))
    {
        getWorld()->playSound(SOUND_OIL_SLICK);
        doActivity(getWorld()->getPlayer());
    }
}

void OilSlick::doActivity(GhostRacer* gr)
{
    int random = randInt(5, 20);
    int random_multiplier = randInt(0, 1);
    
    int cur_dir = gr->getDirection();
    
    if(random_multiplier == 0)
    {
        gr->setDirection(cur_dir - random);
    }
    else
    {
        gr->setDirection(cur_dir + random);
    }
    
    int new_dir = gr->getDirection();
    
    if(new_dir < 60)
    {
        gr->setDirection(60);
    }
    else if (new_dir > 120)
    {
        gr->setDirection(120);
    }
}

int OilSlick::getScoreIncrease() const
{
    return 0;
}

bool OilSlick::selfDestructs() const
{
    return false;
}

bool OilSlick::isSprayable() const
{
    return false;
}

//Healing Goodie Implementations

HealingGoodie::HealingGoodie(StudentWorld* world, double x, double y):GhostRacerActivatedObject(world, IID_HEAL_GOODIE, x, y, 1.0, 0)
{
    
}

void HealingGoodie::doSomething()
{
    double horizSpeed = getHorizontalSpeed();
    moveRelativeToGhostRacerVerticalSpeed(horizSpeed);
    
    if(getWorld()->overlaps(this, getWorld()->getPlayer()))
    {
        doActivity(getWorld()->getPlayer());
    }
}

void HealingGoodie::doActivity(GhostRacer* gr)
{
    gr->increaseHP(10);
    
    if(gr->getHP() > 100)
    {
        gr->reduceHP(gr->getHP()-100);
    }
    
    killed();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(getScoreIncrease());
    
}

int HealingGoodie::getScoreIncrease() const
{
    return 250;
}

bool HealingGoodie::selfDestructs() const
{
    return true;
}

bool HealingGoodie::isSprayable() const
{
    return true;
}

//Holy Water Goodie Implementations
HolyWaterGoodie::HolyWaterGoodie(StudentWorld* world, double x, double y): GhostRacerActivatedObject(world, IID_HOLY_WATER_GOODIE, x, y, 2.0, 90)
{
    
}

void HolyWaterGoodie::doSomething()
{
    double horizSpeed = getHorizontalSpeed();
    moveRelativeToGhostRacerVerticalSpeed(horizSpeed);
    
    if(getWorld()->overlaps(this, getWorld()->getPlayer()))
    {
        doActivity(getWorld()->getPlayer());
    }
}

void HolyWaterGoodie::doActivity(GhostRacer* gr)
{
    gr->increaseSprays(10);
    killed();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(getScoreIncrease());
}

int HolyWaterGoodie::getScoreIncrease() const
{
    return 50;
}

bool HolyWaterGoodie::selfDestructs() const
{
    return true;
}

bool HolyWaterGoodie::isSprayable() const
{
    return true;
}

// Soul Goodie Implementations
SoulGoodie::SoulGoodie(StudentWorld* world, double x, double y):GhostRacerActivatedObject(world, IID_SOUL_GOODIE, x, y, 4.0, 0)
{
    
}

void SoulGoodie::doSomething()
{
    double horizSpeed = getHorizontalSpeed();
    moveRelativeToGhostRacerVerticalSpeed(horizSpeed);
    
    if(getWorld()->overlaps(this, getWorld()->getPlayer()))
    {
        doActivity(getWorld()->getPlayer());
    }
    
    setDirection(getDirection()-10);
}

void SoulGoodie::doActivity(GhostRacer* gr)
{
    getWorld()->recordSoulSaved();
    killed();
    getWorld()->playSound(SOUND_GOT_SOUL);
    getWorld()->increaseScore(getScoreIncrease());
}

int SoulGoodie::getScoreIncrease() const
{
    return 100;
}

bool SoulGoodie::selfDestructs() const
{
    return false;
}

bool SoulGoodie::isSprayable() const
{
    return false;
}
