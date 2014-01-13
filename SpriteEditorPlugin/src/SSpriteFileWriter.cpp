#include "SSpriteFileWriter.hpp"

SSpriteFileWriter::SSpriteFileWriter(Uint8 *data, Uint64 length)
    : base(data, length)
{
}

SSpriteFileWriter::SSpriteFileWriter(const std::string& filepath)
    : base(filepath)
{
}

void SSpriteFileWriter::writeFile(SSpriteFile* file)
{
    UNUSED(file);
}
