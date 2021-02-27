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
    void resetLastY();
    
    //Get Ghost Racer
    GhostRacer* getPlayer() const;
    
    //Overlap functions
    bool overlaps(Actor* subject, Actor* object) const;
    
    //Soul Saved
    void recordSoulSaved();
    int getSoulSaved() const;
    void resetSoulSaved();
    
    //Spray
    bool sprayFirstAppropriateActor(Actor* p);
    void addNewSpray(double x, double y, int dir);
    
    //Identify functions
    bool isCollisionWorthyObjects(Actor* p);
    bool zombieCabDetect(ZombieCab* p, int direction);
    void determineZombieCabSpawn();

    //Bonus points
    void reduceBonusPoints();
    int getBonusPoints() const;
    void resetBonusPoints();
    
    //Goodies
    void addNewHealingGoodie(double x, double y);
    void addNewOilSlick(double x, double y);
    
private:
    GhostRacer* m_player;
    list<Actor*> m_actors;
    double lastY = 0;
    int m_soulSaved = 0;
    int m_bonusPoints = 5000;
    //int numberOfObjects = 0;
    
};

#endif // STUDENTWORLD_H_
