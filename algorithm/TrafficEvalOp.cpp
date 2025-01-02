#include "ECF/ECF.h"
#include "engine/engine.h"
#include "utility/optionparser.h"
#include "TrafficEvalOp.h"
#include <vector>
#include <algorithm>

typedef std::map<std::string, int> VehicleLaneMap;

using namespace CityFlow;

void TrafficEvalOp::registerParameters(StateP state) {
    state->getRegistry()->registerEntry("yellow.light", (voidP) (new int), ECF::INT);
}

bool TrafficEvalOp::initialize(StateP state) {
    // initialize engine
    std::string engineConfig = "/home/luka/Documents/Faks/Projekt/data/config.json";
    size_t threadNum = 1;
    this->trafficEngine = new Engine(engineConfig, threadNum);
    this->simulationSteps = 150;
    setReplay(false);

    // load yellow light duration value from config registry
    if (state->getRegistry()->isModified("yellow.light")) {
        voidP yellowLightPtr = state->getRegistry()->getEntry("yellow.light");
        this->yellowLight = *((int*) yellowLightPtr.get());
    }
    return true;
}

FitnessP TrafficEvalOp::evaluate(IndividualP individual) {
    // reset engine and average travel time
    trafficEngine->reset();
    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();

    double prevTreeOutput = 0;      // keep track of previous tree output
    double yellowLightTimer = 0;    // turn on yellow light after every decision
    int totalWaitingVehicles = 0;   // cumulative quantity of waiting vehicles

    // go through all simulation steps
    for (int step = 0; step < simulationSteps; step++) {
        double treeOutput = evaluateTree(tree);

        // get current number of vehicles waiting on each lane
        VehicleLaneMap vm = trafficEngine->getLaneWaitingVehicleCount();
        for (auto it = vm.begin(); it != vm.end(); it++) {
            if (it->second != 0) totalWaitingVehicles += it->second;
        }
        // map a decision only if it differs from the previous
        if (treeOutput != prevTreeOutput && yellowLightTimer == 0) {
            mapDecision(treeOutput);
            prevTreeOutput = treeOutput;
            yellowLightTimer = this->yellowLight;
        }
        // yellow light countdown
        if (yellowLightTimer > 0) yellowLightTimer--;

        // std::cout << "Tree output for step " << step + 1 << ": " << treeOutput << std::endl;
        trafficEngine->nextStep();
    }

    // calculate fitness
    FitnessP fitness (new FitnessMin);
    double averageWaitingTime = totalWaitingVehicles / trafficEngine->getVehicles().size();
    fitness->setValue(averageWaitingTime);
    return fitness;
}

double TrafficEvalOp::evaluateTree(Tree::Tree* tree) {
    // relevant simulator data
    VehicleLaneMap waitingVehicles = trafficEngine->getLaneWaitingVehicleCount();
    VehicleLaneMap totalVehicles = trafficEngine->getLaneVehicleCount();
    std::vector<double> terminalsX;
    std::vector<double> terminalsY;
    std::string roadsToIgnorePrefix = "road_1_1_";

    // put simulator values into tree terminals set
    for (auto it = waitingVehicles.begin(); it != waitingVehicles.end(); it++) {
        if (it->first.compare(0, roadsToIgnorePrefix.size(), roadsToIgnorePrefix) != 0) {
            terminalsX.push_back(it->second);
        }
    }
    for (auto it = totalVehicles.begin(); it != totalVehicles.end(); it++) {
        if (it->first.compare(0, roadsToIgnorePrefix.size(), roadsToIgnorePrefix) != 0) {
            terminalsY.push_back(it->second);
        }
    }
    for (int i = 0; i < terminalsX.size(); i++) {
        tree->setTerminalValue("X" + std::to_string(i), &terminalsX[i]);
        tree->setTerminalValue("Y" + std::to_string(i), &terminalsY[i]);
        // std::cout << "X" << i << ": " << terminalsX[i] << std::endl;
        // std::cout << "X" << i << ": " << terminalsY[i] << std::endl;
    }
    // evaluate tree
    double result = 0;
    tree->execute(&result);
    return result;
}

void TrafficEvalOp::mapDecision(double treeOutput) {
    std::string intersectionId = "intersection_1_1";
    std::vector<int> phaseIndices = {1, 2, 3, 4, 5, 6, 7, 8};

    // convert to int and clip to range of phaseIndices
    int phaseIndex = std::max(phaseIndices.front(), std::min((int)treeOutput, phaseIndices.back()));

    // std::cout << "Changing traffic light(s) " << phaseIndex << " of " << intersectionId << std::endl;
    trafficEngine->setTrafficLightPhase(intersectionId, phaseIndex);
}

void TrafficEvalOp::setReplay(bool save) {
    trafficEngine->setSaveReplay(save);
}
