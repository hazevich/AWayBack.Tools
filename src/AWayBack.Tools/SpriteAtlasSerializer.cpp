#include "SpriteAtlasSerializer.h"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

namespace AWayBack::SpriteAtlasSerializer
{
    void SerializeToFile(std::ofstream& file, SpriteAtlas& spriteAtlas)
    {
        rapidjson::OStreamWrapper fileStream(file);
        rapidjson::PrettyWriter jsonWriter(fileStream);

        jsonWriter.StartObject();

        jsonWriter.String("TextureName");
        jsonWriter.String(spriteAtlas.TextureName);

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

    SpriteAtlas* DeserializeFromFile(std::ifstream& file)
    {
        rapidjson::IStreamWrapper fileStream(file);
        rapidjson::Document document;
        document.ParseStream(fileStream);

        if (!document.IsObject()) return nullptr;
        if (!document.HasMember("TextureName")) return nullptr;
        if (!document.HasMember("Sprites")) return nullptr;

        auto* spriteAtlas = new SpriteAtlas();

        spriteAtlas->TextureName = document["TextureName"].GetString();

        rapidjson::Value& spritesArray = document["Sprites"];

        if (!spritesArray.IsArray()) return nullptr;
        
        spriteAtlas->Sprites = std::vector<Sprite>();
        
        for (rapidjson::SizeType i = 0; i < spritesArray.Size(); i++)
        {
            if (!spritesArray[i].IsObject()) continue;

            rapidjson::Value& spriteObject = spritesArray[i].GetObject();

            if (!spriteObject.HasMember("Name")) continue;

            Sprite sprite;
            sprite.Name = spriteObject["Name"].GetString();

            sprite.Min = ReadVector2(spriteObject, "Min");
            sprite.Max = ReadVector2(spriteObject, "Max");
            sprite.Origin = ReadVector2(spriteObject, "Origin");

            spriteAtlas->Sprites.push_back(sprite);
        }

        return spriteAtlas;
    }

}
