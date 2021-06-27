#pragma once

#include <fstream>

#include "SpriteEditor.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/ostreamwrapper.h"

namespace AWayBack::SpriteAtlasSerializer
{
    template <typename OutputStream>
    void WriteVector2(rapidjson::PrettyWriter<OutputStream>& jsonWriter, const char* name, Vector2& vector)
    {
        jsonWriter.String(name);

        jsonWriter.StartObject();

        jsonWriter.String("X");
        jsonWriter.Double(vector.X);

        jsonWriter.String("Y");
        jsonWriter.Double(vector.Y);

        jsonWriter.EndObject();
    }

    void SerializeToFile(std::ofstream& file, SpriteAtlas& spriteAtlas);
}
