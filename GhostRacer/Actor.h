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
    //Constructor
    Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, double size, int depth, double verticalSpeed, double horizontalSpeed);
    
    //Destructor
    virtual ~Actor() {};
    
    //doSomething
    virtual void doSomething() = 0;
    
    //Get World
    StudentWorld* getWorld() const;
    
    //Speed functions
    double getVerticalSpeed() const;
    double getHorizontalSpeed() const;
    void changeVerticalSpeed(double change);
    void changeHorizontalSpeed(double change);
    void setVerticalSpeed(double set);
    void setHorizontalSpeed(double set); 
    
    virtual bool isCollisionAvoidanceWorthy() const;
    
    virtual bool beSprayedIfAppropriate();
    
    bool moveRelativeToGhostRacerVerticalSpeed(double dx);
    
    //Health Related
    bool isAlive() const;
    void killed();
    
    
private:
    bool m_alive = true;
    StudentWorld* m_world;
    double m_verticalSpeed;
    double m_horizontalSpeed;
    
};

// Base Class: Agent

class Agent: public Actor
{
public:
    //Constructor
    Agent(StudentWorld* world, int imageID, double x, double y, int dir, double size, double verticalSpeed, double horizontalSpeed, int hp);
    
    //Destructor
    virtual ~Agent() {};
    
    virtual bool isCollisionAvoidanceWorthy() const;
    
    //Health related
    int getHP() const;
    void increaseHP(int hp);
    void reduceHP(int reduction);
    
    
private:
    int m_health;
    
};

// Derived Class: Ghost Racer
class GhostRacer: public Agent
{
public:
    //Constructor
    GhostRacer(StudentWorld* world, double x, double y);
    
    //doSomething
    virtual void doSomething();
    
    //Spray related
    int getSpray() const;
    void increaseSprays(int amount);
    
    
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
    //Constructor
    BorderLine(StudentWorld* world, int imageID, double startX, double startY);
    
    //doSomething
    virtual void doSomething();
    
private:
    
};

// Derived Class: Human Pedestrians

class HumanPedestrian: public Pedestrian
{
public:
    //Constructor
    HumanPedestrian(StudentWorld* world, double startX, double startY);
    
    //doSomething
    virtual void doSomething();
    
    //Spray
    virtual bool beSprayedIfAppropriate();
    
private:
    
};

// Derived Class: Zombie Pedestrian

class ZombiePedestrian: public Pedestrian
{
public:
    //Constructor
    ZombiePedestrian(StudentWorld* world, double x, double y);
    
    //doSomething
    virtual void doSomething();
    
    //Spray
    virtual bool beSprayedIfAppropriate();

protected:
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
    //Constructor
    ZombieCab(StudentWorld* world, double x, double y, double verticalSpeed);
    
    //doSomething
    virtual void doSomething();
    
    //Spray
    virtual bool beSprayedIfAppropriate();
    
protected: //Plan
    void moveAndPossiblyPickPlan();
    int getMovementPlanDistance() const;
    void setMovementPlanDistance(int distance); 
    
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
    //Constructor
    Spray(StudentWorld* world, double x, double y, int dir);
    
    //doSomething
    virtual void doSomething();
    
protected:    //Maximum travel distance
    int getMaximumTravel() const;
    void setMaximumTravel(int max);
    
private:
    int m_maximumTravel;
};


// Derived Class: Oil Slick
class OilSlick: public GhostRacerActivatedObject
{
public:
    //Constructor
    OilSlick(StudentWorld* world, double x, double y);
    
    //Stuff
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
    //Constructor
    HealingGoodie(StudentWorld* world, double x, double y);
    
    //Stuff
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const; 
};

//Derived Class: Holy Water Goodie
class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    //Constructor
    HolyWaterGoodie(StudentWorld* world, double x, double y);
    
    //Stuff
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
    //Constructor
    SoulGoodie(StudentWorld* world, double x, double y);
    
    //Stuff
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

#endif // ACTOR_H_
