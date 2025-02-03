#include "PhysFsStream.hpp"
#include <physfs.h>

util::PhysFsStream::PhysFsStream(const char * filename) : m_File(0x0)
{
    if(filename)
        open(filename);
}

util::PhysFsStream::~PhysFsStream()
{
    close();
}

bool util::PhysFsStream::isOpen() const
{
    return (m_File != 0x0);
}

bool util::PhysFsStream::open(const char * filename)
{
    close();
    m_File = PHYSFS_openRead(filename);
    return isOpen();
}

void util::PhysFsStream::close()
{
    if(isOpen())
        PHYSFS_close(m_File);
    m_File = 0x0;
}

std::optional<size_t> util::PhysFsStream::read(void* data, size_t size) {
    if(!isOpen())
        return std::optional<size_t>();

    PHYSFS_sint64 read = PHYSFS_readBytes(m_File, data, static_cast<PHYSFS_uint32>(size));
    if (read == -1)
        return std::optional<size_t>();
    else
        return std::optional<size_t>(read);
}

std::optional<size_t> util::PhysFsStream::seek(size_t position) {
    if(!isOpen())
        return std::optional<size_t>();

    if(PHYSFS_seek(m_File, position))
        return tell();
    else
        return std::optional<size_t>();
}

std::optional<size_t> util::PhysFsStream::tell() {
    if(!isOpen())
        return std::optional<size_t>();

    PHYSFS_sint64 read = PHYSFS_tell(m_File);
    if (read == -1)
        return std::optional<size_t>();
    else
        return std::optional<size_t>(read);
}

std::optional<size_t> util::PhysFsStream::getSize() {
    if(!isOpen())
        return std::optional<size_t>();

    PHYSFS_sint64 read = PHYSFS_fileLength(m_File);
    if (read == -1)
        return std::optional<size_t>();
    else
        return std::optional<size_t>(read);
}

PHYSFS_ErrorCode util::PhysFsStream::getLastError() {
    return PHYSFS_getLastErrorCode();
}

const char* util::PhysFsStream::getLastErrorDesc() {
    return PHYSFS_getErrorByCode(getLastError());
}
