#include "engine/engine.h"
#include "utility/optionparser.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ECF/ECF.h"
#include "TrafficEvalOp.cpp"

using namespace CityFlow;

void saveBestReplay(StateP state) {
    IndividualP bestOfRun = state->getHoF()->getBest()[0];
    TrafficEvalOp* trafficEvalOp = (TrafficEvalOp*) state->getEvalOp().get();
    trafficEvalOp->setReplay(true);
    trafficEvalOp->evaluate(bestOfRun);
}


int main() {
    StateP state (new State);
    state->setEvalOp(new TrafficEvalOp);

    const char* args[] = {
        "main",
        "/home/luka/Documents/Faks/Projekt/algorithm/config.xml"
    };
    int argc = sizeof(args) / sizeof(args[0]);
    char** argv = const_cast<char**>(args);

    state->initialize(argc, argv);
    state->run();
    saveBestReplay(state);

    return 0;
}
