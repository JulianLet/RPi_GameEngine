#include "TimerComponent.h"

TimerComponent::~TimerComponent() = default;

void TimerComponent::Reset()
{
    currentTime = 0;
}
