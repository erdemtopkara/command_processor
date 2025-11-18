#include "App.hpp"

static App app;

extern "C" void App_10msTask(void)
{
    app.task10ms();
}

extern "C" void App_45msTask(void)
{
    app.task45ms();
}
