#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class GhostRacer;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //LastY functions
    int getLastY() const;
    void changeLastY(int new_y);
    
    //Get Ghost Racer
    GhostRacer* getPlayer() const;
    
    //Overlap functions
    bool overlaps(Actor* subject, Actor* object) const;
    
    //Soul Saved
    void recordSoulSaved();
    
    //Spray
    bool sprayFirstAppropriateActor(Actor* p);
    
    //Identify functions
    bool isCollisionWorthyObjects(Actor* p);
    
    //Chances
    int ChancesHumanPed() const;
    int ChanceOfHolyWater() const;
    int ChanceOilSlick() const;
    int ChanceZombiePed() const;
    int ChanceOfLostSoul() const;

private:
    GhostRacer* m_player;
    list<Actor*> m_actors;
    int lastY = 0;
    //int numberOfObjects = 0;
    
};

#endif // STUDENTWORLD_H_
