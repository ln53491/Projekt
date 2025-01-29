#include "ECF/ECF.h"
#include "engine/engine.h"
#include "utility/optionparser.h"
#include "Simulator.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

using namespace CityFlow;
using namespace rapidjson;

Simulator::Simulator(const std::string &configFile, int steps) : Engine(configFile, THREAD_NUM)
{
    this->steps = steps;
    this->setSaveReplay(false);

    // read config file to get roadnet file path
    std::ifstream ifs(configFile);
    IStreamWrapper isw(ifs);
    Document config;
    config.ParseStream(isw);

    std::string roadnetFile = std::string(config["dir"].GetString()) + config["roadnetFile"].GetString();
    
    // read roadnet file to get intersection id and tl phases
    std::ifstream ifsR(roadnetFile);
    IStreamWrapper iswR(ifsR);
    Document roadnet;
    roadnet.ParseStream(iswR);

    std::set<std::string> roadsWithTl;
    const Value& allIntersections = roadnet["intersections"];
    for (SizeType i = 0; i < allIntersections.Size(); ++i) {
        const Value& intersection = allIntersections[i];
        if (!intersection["virtual"].GetBool()) {
            intersectionId = intersection["id"].GetString();
            tlPhases = intersection["trafficLight"]["lightphases"].Size() - 1;
            defaultPhase = tlPhases;

            // get roads with tl
            const Value& roadLinks = intersection["roadLinks"];
            for (SizeType j = 0; j < roadLinks.Size(); j++) {
                roadsWithTl.insert(roadLinks[j]["startRoad"].GetString());
            }
        }
    }
    // get all lanes from roads with tl
    const Value& roads = roadnet["roads"];
    for (SizeType i = 0; i < roads.Size(); ++i) {
        const Value& currRoad = roads[i];
        if (roadsWithTl.count(currRoad["id"].GetString()) > 0) {
            int numLanes = currRoad["lanes"].Size();
            for (int j = 0; j < numLanes - 2; j++) {
                roadIds.push_back(std::string(currRoad["id"].GetString()) + "_" + std::to_string(j));
            }
        }
    }
}

std::vector<double> Simulator::getLaneVehicles_rel()
{
    std::vector<double> values;
    VehiclesByLane vlMap = this->getLaneVehicleCount();
    for (std::string key : roadIds) {
        auto it = vlMap.find(key);
        if (it != vlMap.end()) {
            values.push_back((double)it->second);
        }
    }
    return values;
}

std::vector<double> Simulator::getLaneWaitingVehicles_rel()
{
    std::vector<double> values;
    VehiclesByLane vlMap = this->getLaneWaitingVehicleCount();
    for (std::string key : roadIds) {
        auto it = vlMap.find(key);
        if (it != vlMap.end()) {
            values.push_back((double)it->second);
        }
    }
    return values;
}

void Simulator::closeIntersection() {
    this->setTrafficLightPhase(intersectionId, this->defaultPhase);
}

int Simulator::getSteps(){
    return steps;
}

void Simulator::setDefaultPhase(int phaseIndex) {
    defaultPhase = phaseIndex;
}

void Simulator::setYellowLight(int duration) {
    yellowLight = duration;
}

int Simulator::getYellowLight() {
    return yellowLight;
}

int Simulator::getTlPhases() {
    return tlPhases;
}

std::vector<string> Simulator::getRoadIds() {
    return roadIds;
}

std::string Simulator::getIntersectionId() {
    return intersectionId;
}
