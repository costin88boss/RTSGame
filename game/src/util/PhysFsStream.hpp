#pragma once
#include <SFML/System/InputStream.hpp>

struct PHYSFS_File;
enum PHYSFS_ErrorCode;

namespace util {
    class PhysFsStream : public sf::InputStream {
    public:
        PhysFsStream(const char* filename = 0x0);
        virtual ~PhysFsStream();
        bool isOpen() const;
        bool open(const char* filename);
        void close();
        virtual std::optional<size_t> read(void* data, size_t size);
        virtual std::optional<size_t> seek(size_t position);
        virtual std::optional<size_t> tell();
        virtual std::optional<size_t> getSize();

        PHYSFS_ErrorCode getLastError();
        const char* getLastErrorDesc();

        PhysFsStream(PhysFsStream const&) = delete;
        void operator=(PhysFsStream const& x) = delete;

    private:
        PHYSFS_File* m_File;
    };

}  // namespace util
