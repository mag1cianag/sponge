#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`
using namespace std;

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity), _capacity(capacity), _storage(capacity), _bitmap(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t remain = _output.remaining_capacity(); if (_expect >= index) {
        // match
        size_t i;
        for (i = _expect - index; i < data.size(); i++) {
            if (_size >= remain) {
                break;
            }
            _expect++;
            if (_bitmap[_idx]) {
                _idx += 1;
                continue;
            }
            _storage[_idx] = data[i];
            _bitmap[_idx] = true;
            _idx++;
            _size++;
        }
        if (eof) {
            _eof = true;
            _eofidx = index + data.size();
        }
        // send to output
        while (_idx < _capacity) {
            if (!_bitmap[_idx]) {
                break;
            }
            _idx++;
            _expect++;
        }
        string mess;
        for (size_t a = 0; a < _idx; a++) {
            mess += _storage[a];
            _size--;
            _bitmap[a] = false;
        }
        _output.write(mess);
        for (size_t a = _idx; a < _capacity; a++) {
            _storage[a - _idx] = _storage[a];
            _bitmap[a - _idx] = _bitmap[a];
        }
        _idx = 0;
        if (_expect == _eofidx && _eof) {
            _output.end_input();
        }
    } else {
        // cache

        size_t start = _idx + index - _expect;
        for (size_t i = 0; i < data.size(); i++) {
            if (start >= remain) {
                break;
            }
            if (_bitmap[start]) {
                start += 1;
                continue;
            }
            _storage[start] = data[i];
            _bitmap[start] = true;
            start++;
            _size++;
        }
        if (eof) {
            _eof =true;
            _eofidx = index + data.size();
        }
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _size; }

bool StreamReassembler::empty() const { return _size == 0; }
