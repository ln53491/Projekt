#ifndef Simulator_h
#define Simulator_h

#include "engine/engine.h"
#include "utility/optionparser.h"
#include <memory>

#define THREAD_NUM 1

typedef std::map<std::string, int> VehiclesByLane;

using namespace CityFlow;

class Simulator : public Engine
{
private:
    int steps;
    int tlPhases = 0;
    int yellowLight = 0;
    int redLight = 0;
    int defaultPhase;
    std::string intersectionId;
    std::vector<string> roadIds;
public:
    Simulator (const std::string &configFile, int steps);
    int getSteps();
    void setDefaultPhase(int phaseIndex);
    void setYellowLight(int duration);
    int getYellowLight();
    void setRedLight(int duration);
    int getRedLight();
    int getTlPhases();
    void closeIntersection();
    std::string getIntersectionId();
    std::vector<string> getRoadIds();
    std::vector<double> getLaneVehicles_rel();
    std::vector<double> getLaneWaitingVehicles_rel();
};

#endif // Simulator_h
