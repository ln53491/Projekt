#ifndef TrafficEvalOp_h
#define TrafficEvalOp_h

#include "engine/engine.h"
#include "utility/optionparser.h"
#include <memory>

using namespace CityFlow;

class TrafficEvalOp : public EvaluateOp
{
private:
    int simulationSteps;
    Engine* trafficEngine;
    double evaluateTree(Tree::Tree* tree);
    void mapDecision(double treeOutput);
public:
    FitnessP evaluate(IndividualP individual);
    bool initialize(StateP);
    void setReplay(bool save);
};

#endif // TestEvalOp_h
