#include "SpriteAtlasSerializer.h"

namespace AWayBack::SpriteAtlasSerializer
{
    void SerializeToFile(std::ofstream& file, SpriteAtlas& spriteAtlas)
    {
        rapidjson::OStreamWrapper fileStream(file);
        rapidjson::PrettyWriter jsonWriter(fileStream);

        jsonWriter.StartObject();

        jsonWriter.String("TextureName");
        jsonWriter.String(spriteAtlas.Name);

        jsonWriter.String("Sprites");
        jsonWriter.StartArray();

        for (Sprite& sprite : spriteAtlas.Sprites)
        {
            jsonWriter.StartObject();

            jsonWriter.String("Name");
            jsonWriter.String(sprite.Name);
            
            WriteVector2(jsonWriter, "Min", sprite.Min);
            WriteVector2(jsonWriter, "Max", sprite.Max);
            WriteVector2(jsonWriter, "Origin", sprite.Origin);

            jsonWriter.EndObject();
        }

        jsonWriter.EndArray();

        jsonWriter.EndObject();
    }
}
