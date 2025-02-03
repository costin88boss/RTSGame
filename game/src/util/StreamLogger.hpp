#include <ostream>
#include <streambuf>

#include "spdlog/spdlog.h"

namespace util {

    template<class Element = char, class Trait = std::char_traits<Element>>
    class StreamLogger : public std::basic_streambuf<Element, Trait> {
    public:
        StreamLogger(std::ostream& stream, std::shared_ptr<spdlog::logger> logger)
            : m_stream{stream}, m_logger{std::move(logger)} {
            // Redirect provided stream
            m_buffer = m_stream.rdbuf(this);
        };

        ~StreamLogger() {
            try {
                // Restore provided stream
                m_stream.rdbuf(m_buffer);

            } catch (const std::ios_base::failure& exception) {
                const auto message = std::string{"StreamLogger was unable to restore the provided stream: "};
                m_logger->error(message + exception.what());
            }
        }

        StreamLogger(const StreamLogger& other) = delete;
        StreamLogger& operator=(const StreamLogger& other) = delete;

        StreamLogger(StreamLogger&& other) noexcept = default;
        StreamLogger& operator=(StreamLogger&& other) noexcept = default;

    protected:
        std::streamsize xsputn(const Element* elements, const std::streamsize count) override {
            m_string_buffer.append(elements, static_cast<std::string::size_type>(count));
            return count;
        }

        typename Trait::int_type overflow(typename Trait::int_type final_character) override {
            m_logger->error(m_string_buffer);
            m_string_buffer.clear();
            return Trait::not_eof(final_character);
        }

        std::basic_ostream<Element, Trait>& m_stream;
        std::streambuf* m_buffer;
        std::basic_string<Element, Trait> m_string_buffer;
        std::shared_ptr<spdlog::logger> m_logger;
    };
}  // namespace util

/*
------------------------------------------------------------------------------
MIT License

Copyright (c) 2020-2023 Lukas Durrenberger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
*/
