#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // dismatch
    if (index > _expect) {
        return;
    }
    size_t start;
    for (start = _expect - index; start < data.size(); start += 1) {
        if (_storage.size() >= _capacity) {
            break;
        }
        _storage.push_back(data[start]);
        _expect += 1;
    }
    if (start == data.size() && eof) {
        _eof = true;
    }
    send_out();
}

void StreamReassembler::send_out() {
    auto remain = min(_output.remaining_capacity(), _storage.size());
    if (remain > 0) {
        string data;
        for (size_t i = 0; i < remain; i += 1) {
            data += _storage.front();
            _storage.pop_front();
        }
        _output.write(data);
    }
    if (_storage.empty() && _eof) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _storage.size(); }

bool StreamReassembler::empty() const { return _storage.empty(); }
