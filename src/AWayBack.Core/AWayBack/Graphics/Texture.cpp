#include "Texture.h"

#ifdef WINDOWS
    #include "AWayBack/Platform/OpenGL/OpenGLTexture.h"
#endif // WINDOWS

namespace AWayBack
{
    Texture2D* Texture2D::FromFile(const std::string& file)
    {
    #ifdef WINDOWS
        return new OpenGLTexture2D(file);
    #else
        return nullptr;
    #endif // WINDOWS        
    }

}