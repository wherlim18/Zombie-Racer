#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <iostream>

class StudentWorld;

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// Base Class: Actor
class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, double size, int depth, double verticalSpeed, double horizontalSpeed);
    
    //doSomething
    virtual void doSomething() = 0;
    
    //Get World
    StudentWorld* getWorld() const;
    
    //Speed
    double getVerticalSpeed() const;
    double getHorizontalSpeed() const;
    void changeVerticalSpeed(double change);
    void changeHorizontalSpeed(double change);
    void setVerticalSpeed(double set);
    void setHorizontalSpeed(double set); 
    
    virtual bool isDamagable() const;
    
    virtual bool isCollisionAvoidanceWorthy() const;
    
    virtual int doSomethingWhenHit();
    
    virtual bool beSprayedIfAppropriate();
    
    virtual bool moveRelativeToGhostRacerVerticalSpeed(double dx); 
    
    //Hit Related
    bool isHit();
    void gotHit();
    
    
    //Health Related
    bool isAlive() const;
    void killed();
    
    //Hit related functions
    
private:
    bool m_alive = true;
    StudentWorld* m_world;
    double m_verticalSpeed;
    double m_horizontalSpeed;
    bool m_isHit = false;
    
};

// Base Class: Agent

class Agent: public Actor
{
public:
    Agent(StudentWorld* world, int imageID, double x, double y, int dir, double size, double verticalSpeed, double horizontalSpeed, int hp);
    
    virtual bool isCollisionAvoidanceWorthy() const;
    
    //Health related
    int getHP() const;
    void increaseHP(int hp);
    void reduceHP(int reduction);
    
    //virtual bool takeDamageAndPossiblyDie(int hp);
    
    //virtual int soundWhenHurt();
    
    //virtual int soundWhenDie();
    
private:
    int m_health;
    
};

// Derived Class: Ghost Racer
class GhostRacer: public Agent
{
public:
    GhostRacer(StudentWorld* world, double x, double y);
    
    //doSomething
    virtual void doSomething();
    
    //Spray related
    int getSpray() const;
    void increaseSprays(int amount);
    
    void spin();
    
    //virtual int soundWhenDie();
    
protected:
    void moveGhostRacer();
private:
    int m_spray;
    
    
};

// Base Class: Ghost Racer Activated Object

class GhostRacerActivatedObject: public Actor
{
public:
    GhostRacerActivatedObject(StudentWorld* world, int imageID, double x, double y, double size, int dir);
    
    virtual bool beSprayedIfAppropriate();
    
    virtual void doActivity(GhostRacer* gr) = 0;
    
    virtual int getScoreIncrease() const = 0;
    
    virtual bool selfDestructs() const = 0;
    
    virtual bool isSprayable() const = 0;
    
private:
    
};

// Base Class: Pedestrians
class Pedestrian: public Agent
{
public:
    Pedestrian(StudentWorld* world, int imageID, double startX, double startY, double size);
    
    //Plan
    void moveAndPossiblyPickPlan();
    void decrementMovementPlan();
    int getMovementPlanDistance() const;
    void setMovementPlanDistance(int distance);
    
private:
    int m_movementPlanDistance = 0;
};

// Derived Class: Border lines

class BorderLine: public Actor
{
public:
    BorderLine(StudentWorld* world, int imageID, double startX, double startY);
    virtual void doSomething();
    
private:
    
};

// Derived Class: Human Pedestrians

class HumanPedestrian: public Pedestrian
{
public:
    HumanPedestrian(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
    
    virtual int doSomethingWhenHit();
    
    virtual bool beSprayedIfAppropriate();
    
private:
    
};

// Derived Class: Zombie Pedestrian

class ZombiePedestrian: public Pedestrian
{
public:
    ZombiePedestrian(StudentWorld* world, double x, double y);
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    virtual int doSomethingWhenHit();
    
    //Grunts
    int getUntilGrunt() const;
    void setUntilGrunt(int ticks);
    void changeUntilGrunt(int ticks);
    
private:
    int m_untilGrunt = 0;
};

// Derived Class: Zombie Cab
class ZombieCab: public Agent
{
public:
    ZombieCab(StudentWorld* world, double x, double y, double verticalSpeed);
    virtual void doSomething();
    
    //Plan
    void moveAndPossiblyPickPlan();
    int getMovementPlanDistance() const;
    void setMovementPlanDistance(int distance); 
    
    virtual bool beSprayedIfAppropriate();
    
    //Damaged Ghost Racer
    bool hasDamaged() const;
    void isDamaging();

private:
    
    int m_movementPlanDistance = 0;
    bool m_hasDamaged = false;
    
};

// Derived Class: Spray
class Spray: public Actor
{
public:
    Spray(StudentWorld* world, double x, double y, int dir);
    virtual void doSomething();
    
    //Maximum travel distance
    int getMaximumTravel() const;
    void setMaximumTravel(int max);
    
private:
    int m_maximumTravel;
};


// Derived Class: Oil Slick
class OilSlick: public GhostRacerActivatedObject
{
public:
    OilSlick(StudentWorld* world, double x, double y);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;

private:
    
};

//Derived Class: Healing Goodie
class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(StudentWorld* world, double x, double y);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const; 
};

//Derived Class: Holy Water GOodie
class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(StudentWorld* world, double x, double y);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

//Derived Class: Soul Goodie
class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(StudentWorld* world, double x, double y);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

#endif // ACTOR_H_
