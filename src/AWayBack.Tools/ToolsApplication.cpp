#include "Application.h"

using namespace AWayBack;

int main()
{
    Application* app = new Application();
    app->Run();

    delete app;
}