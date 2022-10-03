#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _index(0), _buffer(), _last_byte_index(), _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    bool fully_buffered = true;
    for (size_t i = 0; i < data.size(); i++) {
        // Dedup byte already assembled or buffered.
        if (index + i < _index || _buffer.count(index + i)) {
            continue;
        }
        // Only add the byte if byte index not exceed the next segment limit.
        if (index + i - _index + 1 <= _capacity - _output.buffer_size()) {
            _buffer.emplace(index + i, data.at(i));
        } else {
            fully_buffered = false;
            break;
        }
    }
    if (fully_buffered && eof) {
        _last_byte_index = index + data.size();
    }
    StreamReassembler::assemble();
}

size_t StreamReassembler::unassembled_bytes() const { return _buffer.size(); }

bool StreamReassembler::empty() const { return _buffer.empty(); }

void StreamReassembler::assemble() {
    stringstream ss;
    while (_buffer.count(_index)) {
        ss << _buffer.at(_index);
        _buffer.erase(_index++);
    }
    string sstring = ss.str();
    size_t written = _output.write(sstring);
    if (written != sstring.size()) {
        throw runtime_error("assembled string size not equal");
    }
    if (_last_byte_index.has_value() && _index >= _last_byte_index.value()) {
        _output.end_input();
    }
}
