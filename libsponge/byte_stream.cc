#include "byte_stream.hh"

#include <sstream>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : buffer_(), capacity_(capacity), input_ended_(false), bytes_written_(0), bytes_read_(0) {}

size_t ByteStream::write(const string &data) {
    if (ByteStream::input_ended()) {
        ByteStream::set_error();
        return 0;
    }
    size_t i;
    for (i = 0; i < data.size(); i++) {
        if (buffer_.size() < capacity_) {
            buffer_.push_back(data.at(i));
        } else {
            break;
        }
    }
    bytes_written_ += i;
    return i;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    stringstream ss;
    for (size_t i = 0; i < len && i < buffer_.size() && i < len; i++) {
        ss << buffer_.at(i);
    }
    return ss.str();
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t i = 0;
    while (!buffer_.empty() && i < len) {
        buffer_.pop_front();
        i++;
    }
    bytes_read_ += i;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    stringstream ss;
    for (auto ch : buffer_) {
        if (ss.gcount() == static_cast<long int>(len)) {
            break;
        }
        buffer_.erase(buffer_.begin() + ss.gcount());
        ss << ch;
    }
    string res = ss.str();
    bytes_read_ += res.size();
    return res;
}

void ByteStream::end_input() { input_ended_ = true; }

bool ByteStream::input_ended() const { return input_ended_; }

size_t ByteStream::buffer_size() const { return buffer_.size(); }

bool ByteStream::buffer_empty() const { return buffer_.empty(); }

bool ByteStream::eof() const { return input_ended_ && ByteStream::buffer_empty(); }

size_t ByteStream::bytes_written() const { return bytes_written_; }

size_t ByteStream::bytes_read() const { return bytes_read_; }

size_t ByteStream::remaining_capacity() const { return capacity_ - buffer_.size(); }
