#ifndef SSPRITEFILEWRITER_HPP
#define SSPRITEFILEWRITER_HPP

#include <BinaryWriter.hpp>

class SSpriteFile;

class SSpriteFileWriter : public zelda::io::BinaryWriter
{
    BINARYWRITER_BASE;
public:
    SSpriteFileWriter(Uint8* data, Uint64 length);

    SSpriteFileWriter(const std::string& filepath);

    void writeFile(SSpriteFile* file);
};

#endif // SSPRITEFILEWRITER_HPP
