#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <iostream>

#define PI 3.14159265

class StudentWorld;

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// Base Class: Actor
class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, double size, int depth, int verticalSpeed, int horizontalSpeed);
    
    //doSomething
    virtual void doSomething() = 0;
    
    //Get World
    StudentWorld* getWorld() const;
    
    //Speed
    int getVerticalSpeed() const;
    int getHorizontalSpeed() const;
    void changeVerticalSpeed(int change);
    void changeHorizontalSpeed(int change);
    void setVerticalSpeed(int set);
    void setHorizontalSpeed(int set); 
    
    virtual bool isDamagable() const;
    
    virtual bool isCollisionAvoidanceWorthy() const;
    
    virtual int doSomethingWhenHit() = 0;
    
    //virtual bool beSprayedIfAppropriate();
    
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
    int m_verticalSpeed;
    int m_horizontalSpeed;
    bool m_isHit = false;
    
};

// Base Class: Agent

class Agent: public Actor
{
public:
    Agent(StudentWorld* world, int imageID, double x, double y, int dir, double size, int verticalSpeed, int horizontalSpeed, int hp);
    
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

    virtual int doSomethingWhenHit() {return 0;};
    
    //virtual int soundWhenDie();
    
protected:
    void moveGhostRacer();
private:
    int m_spray;
    
    
};


// Base Class: Pedestrians
class Pedestrian: public Agent
{
public:
    Pedestrian(StudentWorld* world, int imageID, double startX, double startY, double size);
    
    virtual bool isCollisionAvoidanceWorthy() const;
    
    //Speed
    void setHorizSpeed(int s);
    
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
    virtual int doSomethingWhenHit() {return 0;};
    
private:
    
};

// Derived Class: Human Pedestrians

class HumanPedestrian: public Pedestrian
{
public:
    HumanPedestrian(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
    
    virtual int doSomethingWhenHit();
    
    //virtual bool beSprayedIfAppropriate();
    
private:
    
};

class ZombiePedestrian: public Pedestrian
{
public:
    ZombiePedestrian(StudentWorld* world, double x, double y);
    virtual void doSomething();
    //virtual bool beSprayedIfAppropriate();
    //virtual bool takeDamageAndPossiblyDie(int hp);
    virtual int doSomethingWhenHit();
    
    //Grunts
    int getUntilGrunt() const;
    void setUntilGrunt(int ticks);
    void changeUntilGrunt(int ticks);
    
private:
    int m_untilGrunt = 0;
};

#endif // ACTOR_H_
