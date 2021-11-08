#include <fstream>
#include <filesystem>
#include <optional>

#include "SpriteAtlas.h"
#include "AWayBack/Graphics/Texture.h"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/ostreamwrapper.h"

namespace fs = std::filesystem;

namespace AWayBack
{
    SpriteAtlas::SpriteAtlas(std::string folder, std::string name, std::vector<Sprite> sprites, Texture2D* texture)
        : Folder(std::move(folder)), Name(std::move(name)), Sprites(std::move(sprites)), Texture(texture)
    {
        
    }

    SpriteAtlas::~SpriteAtlas()
    {
        delete Texture;
    }

    template <typename OutputStream>
    void WriteVector2(rapidjson::PrettyWriter<OutputStream>& jsonWriter, const char* name, const Vector2& vector)
    {
        jsonWriter.String(name);

        jsonWriter.StartObject();

        jsonWriter.String("X");
        jsonWriter.Double(vector.X);

        jsonWriter.String("Y");
        jsonWriter.Double(vector.Y);

        jsonWriter.EndObject();
    }

    Vector2 ReadVector2(rapidjson::Value& document, const char* name)
    {
        if (!document.HasMember(name) || !document.IsObject()) return Vector2();

        rapidjson::Value& vectorValue = document[name];

        if (!vectorValue.HasMember("X") || !vectorValue["X"].IsFloat()) return Vector2();
        if (!vectorValue.HasMember("Y") || !vectorValue["Y"].IsFloat()) return Vector2();

        auto vector = Vector2(vectorValue["X"].GetFloat(), vectorValue["Y"].GetFloat());

        return vector;
    }

    void SerializeToFile(std::ofstream& file, const SpriteAtlas& spriteAtlas)
    {
        rapidjson::OStreamWrapper fileStream(file);
        rapidjson::PrettyWriter jsonWriter(fileStream);

        jsonWriter.StartObject();

        jsonWriter.String("TextureName");
        jsonWriter.String(spriteAtlas.Texture->GetFullPath());

        jsonWriter.String("Sprites");
        jsonWriter.StartArray();

        for (const Sprite& sprite : spriteAtlas.Sprites)
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

    struct DeserializedSpriteAtlas
    {
        std::vector<Sprite> Sprites;
        std::string TextureName;
    };


    std::optional<DeserializedSpriteAtlas> DeserializeFromFile(std::ifstream& file)
    {
        rapidjson::IStreamWrapper fileStream(file);
        rapidjson::Document document;
        document.ParseStream(fileStream);

        if (!document.IsObject()) return std::nullopt;
        if (!document.HasMember("TextureName")) return std::nullopt;
        if (!document.HasMember("Sprites")) return std::nullopt;

        DeserializedSpriteAtlas spriteAtlas;

        spriteAtlas.TextureName = document["TextureName"].GetString();

        rapidjson::Value& spritesArray = document["Sprites"];

        if (!spritesArray.IsArray()) return std::nullopt;
        
        spriteAtlas.Sprites = std::vector<Sprite>(spritesArray.Size());
        
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

            spriteAtlas.Sprites[i] = sprite;
        }

        return spriteAtlas;
    }

    SpriteAtlas* LoadSpriteAtlasFromFile(const std::string& spriteAtlasPath)
    {
        std::ifstream fileStream;
        fileStream.open(spriteAtlasPath);

        auto deserializedSpriteAtlas = DeserializeFromFile(fileStream);

        if (deserializedSpriteAtlas)
        {
            fs::path path = spriteAtlasPath;
            std::string name = path.filename().string();
            std::string folder = path.parent_path().string();
            Texture2D* texture = Texture2D::FromFile(deserializedSpriteAtlas->TextureName);

            return new SpriteAtlas(folder, name, deserializedSpriteAtlas->Sprites, texture);
        }

        return nullptr;
    }

    void SaveSpriteAtlas(const SpriteAtlas& spriteAtlas)
    {
        auto spriteAtlasPath = getFullPath(spriteAtlas);
        
        std::ofstream file;
        file.open(spriteAtlasPath);
        SerializeToFile(file, spriteAtlas);
    }

}