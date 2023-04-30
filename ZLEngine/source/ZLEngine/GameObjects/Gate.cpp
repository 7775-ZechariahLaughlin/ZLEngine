#include "ZLEngine/GameObjects/Gate.h""

Gate::Gate()
{
    GateModel = nullptr;
}

Gate::~Gate()
{
    GateModel = nullptr;
}

ModelPtr Gate::ReturnGateModel()
{
    return GateModel;
}
