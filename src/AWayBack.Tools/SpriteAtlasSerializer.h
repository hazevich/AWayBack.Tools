#pragma once

#include <fstream>
#include <optional>

#include "SpriteEditor.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/document.h"

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

    inline Vector2 ReadVector2(rapidjson::Value& document, const char* name)
    {
        if (!document.HasMember(name) || !document.IsObject()) return Vector2();

        rapidjson::Value& vectorValue = document[name];

        if (!vectorValue.HasMember("X") || !vectorValue["X"].IsFloat()) return Vector2();
        if (!vectorValue.HasMember("Y") || !vectorValue["Y"].IsFloat()) return Vector2();

        auto vector = Vector2(vectorValue["X"].GetFloat(), vectorValue["Y"].GetFloat());

        return vector;
    }

    void SerializeToFile(std::ofstream& file, SpriteAtlas& spriteAtlas);
    std::optional<SpriteAtlas*> DeserializeFromFile(std::ifstream& file);
}
