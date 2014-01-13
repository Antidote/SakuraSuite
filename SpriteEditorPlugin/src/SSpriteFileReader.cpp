#include "SSpriteFileReader.hpp"
#include "SSpriteFile.hpp"
#include "SSprite.hpp"
#include "SSpritePart.hpp"
#include "SSpriteFrame.hpp"
#include <InvalidOperationException.hpp>
#include <IOException.hpp>

SSpriteFileReader::SSpriteFileReader(Uint8* data, Uint64 length)
    : base(data, length)
{
}

SSpriteFileReader::SSpriteFileReader(const std::string& filepath)
    : base(filepath)
{

}

SSpriteFile* SSpriteFileReader::readFile()
{
    Uint32 magic = base::readUInt32();

    if (magic != SSpriteFile::Magic)
        throw zelda::error::InvalidOperationException("Not a valid Sakura Sprite container");

    Uint32 version = base::readUInt32();

    // TODO: Make this more verbose
    if (version != SSpriteFile::Version)
        throw zelda::error::InvalidOperationException("Unsupported version");

    // After reading in the magic and version we need to load some
    // metadata about the file.
    // Such as the texture count, it's dimensions, and it's origin.
    // After that we have the number of sprites contained in this
    // sprite container.
    Uint16 textureCount = base::readUInt16(); // Having it as a Uint16 gives us the ability to have up to 65536 different states
    // This is probably overkill, but it's better safe than sorry.
    Uint32 width        = base::readUInt32();
    Uint32 height       = base::readUInt32();
    float originX       = base::readFloat();
    float originY       = base::readFloat();
    Uint16 spriteCount  = base::readUInt16();

    // Lets go ahead and create or new container.
    SSpriteFile* ret = new SSpriteFile(width, height, originX, originY);

    // The next four bytes are reserved to keep the header 32 byte aligned.
    // This isn't necessary for most systems, but it's eventually planned
    // to migrate this code to Big Endian based systems, such as the wii
    // which require data to be 32 byte aligned, or it causes some issues.
    // It's also convenient to have this, for later expansion.
    Uint32 reserved     = base::readUInt32();
    UNUSED(reserved);

    // Next we have to load the textures
    // If we tried to add them one at a time to the sprite container
    // it will be slow as hell, so we store them in a vector locally
    // then give that vector the the container, this bypasses the de-reference
    // for each texture
    std::vector<STexture*> textures;

    for (Uint16 i = 0; i < textureCount; i++)
    {
        STexture* texture = new STexture;
        texture->Filepath = base::readString();
        texture->Preload  = base::readBool();
        textures.push_back(texture);
    }

    ret->setTextures(textures);

    // Now for the sprites
    // The sprites are a bit more difficult, they are stored in an unordered_map
    // with it's name as the key, this means we can't have two sprites with the same name
    // Normally this isn't a problem, but someone may decide to copy and paste a sprite
    // and forget to change the name, that needs to be handled, but it's outside the scope
    // of this reader.
    std::unordered_map <std::string, SSprite*> sprites;

    for (Uint16 i = 0; i < spriteCount; i++)
    {
        SSprite* sprite = new SSprite(ret);
        sprite->setName(base::readString());
        Uint16 partCount = base::readUInt16();
        Uint16 stateCount = base::readUInt16();

        // Each state id corresponds to a texture held in the parent class
        std::vector<int> stateIds;
        for (int j = 0; j < stateCount; j++)
            stateIds.push_back(base::readUInt16());


        sprite->setStateIds(stateIds);

        // Now to read the sprite parts.
        // The parts allow us to build retro style sprites very easily
        // making it possible to use one texture atlas for all possible
        // frame combinations, this reduces the amount of memory overhead
        // and the storage footprint, while Sakura supports packs and zips
        // it's still a bad idea to have a metric ton of texture resources
        // littering the place
        std::vector<SSpritePart*> parts;
        for (Uint8 j = 0; j < partCount; j++)
        {
            SSpritePart* part = new SSpritePart(sprite);
            part->setName(base::readString());
            part->setCollision(base::readBool());

            Uint32 frameCount = base::readUInt32();
            std::vector<SSpriteFrame*> frames;

            for (Uint32 k = 0; k < frameCount; k++)
            {
                float xOff = base::readFloat();
                float yOff = base::readFloat();
                float texXOff = base::readFloat();
                float texYOff = base::readFloat();
                Uint32 width = base::readUInt32();
                Uint32 height = base::readUInt32();
                float frameTime = base::readFloat();
                bool flippedH = base::readBool();
                bool flippedV = base::readBool();

                frames.push_back(new SSpriteFrame(xOff, yOff, texXOff, texYOff, width, height, frameTime, flippedH, flippedV));
            }
            part->setFrames(frames);
            parts.push_back(part);
        }

        sprite->setParts(parts);
        if (sprite->name() != std::string())
            sprites[sprite->name()] = sprite;
        else
            throw zelda::error::IOException("SSpriteFileReader::readFile -> Sprite names cannot be empty");
    }

    ret->setSprites(sprites);

    return ret;
}
