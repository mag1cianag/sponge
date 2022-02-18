#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`
using namespace std;

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (data.empty()) {
        return;
    }
    DUMMY_CODE(data, index, eof);
}

size_t StreamReassembler::unassembled_bytes() const { return 0; }

bool StreamReassembler::empty() const { return false; }
