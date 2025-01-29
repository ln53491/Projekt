#include "ECF/ECF.h"
#include "Simulator.h"
#include "TrafficEvalOp.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace CityFlow;

void TrafficEvalOp::registerParameters(StateP state)
{
    state->getRegistry()->registerEntry("baseline.mode", (voidP) (new int), ECF::INT);
    state->getRegistry()->registerEntry("simulator.red.light", (voidP) (new int), ECF::INT);
    state->getRegistry()->registerEntry("simulator.yellow.light", (voidP) (new int), ECF::INT);
    state->getRegistry()->registerEntry("simulator.steps", (voidP) (new int), ECF::INT);
    state->getRegistry()->registerEntry("simulator.config", (voidP) (new std::string), ECF::STRING);
    
}

bool TrafficEvalOp::initialize(StateP state)
{
    baselineMode = (bool) *((int*) state->getRegistry()->getEntry("baseline.mode").get());
    // create simulator instance
    std::string simulatorConfig = *((std::string*) state->getRegistry()->getEntry("simulator.config").get());
    int simulatorSteps = *((int*) state->getRegistry()->getEntry("simulator.steps").get());
    int yellowLight = *((int*) state->getRegistry()->getEntry("simulator.yellow.light").get());
    int redLight = *((int*) state->getRegistry()->getEntry("simulator.red.light").get());
    
    simulator = new Simulator(simulatorConfig, simulatorSteps);
    simulator->setYellowLight(yellowLight);
    simulator->setRedLight(yellowLight);
    return true;
}

FitnessP TrafficEvalOp::evaluate(IndividualP individual)
{
    simulator->reset();
    std::string intersectionId = simulator->getIntersectionId();
    Tree::Tree* currTree = (Tree::Tree*) individual->getGenotype().get();

    int prevDecision = 0;           // keep track of previous decision
    int totalWaitingVehicles = 0;   // cumulative number of waiting vehicles

    int step = 0;
    while (step < simulator->getSteps()) {
        if (baselineMode) {
            if (step % (int)(simulator->getSteps() / simulator->getTlPhases()) == 0) {
                simulator->setTrafficLightPhase(intersectionId, prevDecision);
                std::cout << "Set phase " << prevDecision << " at step " << step << std::endl;
                prevDecision++;
            }
            std::vector<double> vhm = simulator->getLaneWaitingVehicles_rel();
            totalWaitingVehicles += std::accumulate(vhm.begin(), vhm.end(), 0.0);
        }
        else {
            // simulate yellow light
            simulator->closeIntersection();
            for (int i = 0; i < simulator->getYellowLight(); i++) {
                simulator->nextStep();
                step++;
                std::vector<double> vhm = simulator->getLaneWaitingVehicles_rel();
                totalWaitingVehicles += std::accumulate(vhm.begin(), vhm.end(), 0.0);   
            }

            double treeOutput = evaluateTree(currTree);
            int currDecision = makeDecision(treeOutput);
            // change phase
            // std::cout << "Set phase " << currDecision << " at step " << step << std::endl;
            simulator->setTrafficLightPhase(intersectionId, currDecision);
            for (int j = 0; j < simulator->getRedLight(); j++) {
                simulator->nextStep();
                step++;
                std::vector<double> vhm = simulator->getLaneWaitingVehicles_rel();
                totalWaitingVehicles += std::accumulate(vhm.begin(), vhm.end(), 0.0);
            }
        }
        simulator->nextStep();
        step++;
    }
    // calculate fitness
    FitnessP fitness (new FitnessMin);
    // double averageWaitingTime = totalWaitingVehicles / simulator->getVehicles().size();
    fitness->setValue(totalWaitingVehicles);
    return fitness;
}

double TrafficEvalOp::evaluateTree(Tree::Tree* tree)
{
    // relevant simulator data
    std::vector<double> terminalsX = simulator->getLaneVehicles_rel();
    // std::vector<double> terminalsY = simulator->getLaneVehicles_rel();

    for (int i = 0; i < (int)terminalsX.size(); i++) {
        tree->setTerminalValue("X" + std::to_string(i), &terminalsX[i]);
        // tree->setTerminalValue("Y" + std::to_string(i), &terminalsY[i]);
    }
    // evaluate tree
    double result = 0;
    tree->execute(&result);
    return result;
}

int TrafficEvalOp::makeDecision(double result)
{
    int numPhases = simulator->getTlPhases();
    int phaseIndex = std::max(0, std::min((int)result, numPhases - 1));
    // int phaseIndex = ((int)result % numPhases + numPhases) % numPhases;
    return phaseIndex;
}
