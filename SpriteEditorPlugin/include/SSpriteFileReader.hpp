#ifndef SSPRITEFILEREADER_HPP
#define SSPRITEFILEREADER_HPP

#include <BinaryReader.hpp>

class SSpriteFile;

class SSpriteFileReader : public zelda::io::BinaryReader
{
    BINARYREADER_BASE;
public:
    SSpriteFileReader(Uint8* data, Uint64 length);
    SSpriteFileReader(const std::string& filepath);

    SSpriteFile* readFile();
};


#endif // SSPRITEFILEREADER_HPP
