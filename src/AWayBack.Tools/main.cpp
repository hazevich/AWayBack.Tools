#include "ToolsApplication.h"

using namespace AWayBack;

int main()
{
    Application* app = new ToolsApplication();
    app->Run();

    delete app;
}