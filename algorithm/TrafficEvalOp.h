#ifndef TrafficEvalOp_h
#define TrafficEvalOp_h

#include "Simulator.h"
#include <memory>

using namespace CityFlow;

class TrafficEvalOp : public EvaluateOp
{
private:
    bool baselineMode;
    virtual void registerParameters(StateP state);
    double evaluateTree(Tree::Tree* tree);
    int makeDecision(double result);
public:
    Simulator* simulator;
    bool initialize(StateP);
    FitnessP evaluate(IndividualP individual);
};

#endif // TestEvalOp_h
