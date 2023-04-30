#include "ZLEngine/GameObjects/Gate.h"

Gate::Gate()
{
    GateModel = nullptr;
}

Gate::Gate(ModelPtr ModelToUse)
{
    this->GateModel = ModelToUse;
}

Gate::~Gate()
{
    GateModel = nullptr;
}

ModelPtr Gate::ReturnGateModel()
{
    return GateModel;
}
