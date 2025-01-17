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
    //Constructor
    StudentWorld(std::string assetPath);
    
    //Destructor
    virtual ~StudentWorld();
    
    //Stuff
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
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
    
    //Goodies Add functions
    void addNewHealingGoodie(double x, double y);
    void addNewOilSlick(double x, double y);
    
private:
    GhostRacer* m_player;
    list<Actor*> m_actors;
    int lastY = 0;
    int m_soulSaved = 0;
    int m_bonusPoints = 5000;
    
};

#endif // STUDENTWORLD_H_
