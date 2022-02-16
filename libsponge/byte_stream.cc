#include "byte_stream.hh"

#include <algorithm>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _capa(capacity) {}

size_t ByteStream::write(const string &data) {
    if (this->input_ended()) {
        return 0;
    }
    size_t count = 0;
    for (auto c : data) {
        if (_buf.size() >= _capa) {
            break;
        }
        _buf.push_back(c);
        count += 1;
    }
    _wbytes += count;
    return count;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string s;
    for (size_t i = 0; i < len && i < _buf.size(); i += 1) {
        s.append(1, _buf[i]);
    }
    return s;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t end = min(len, buffer_size());
    for (size_t i = 0; i < end; i += 1) {
        _buf.pop_front();
    }
    _rbytes += end;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    if (this->eof()) {
        return {};
    }
    string s = peek_output(len);
    pop_output(len);
    return s;
}

void ByteStream::end_input() { _ended = true; }

bool ByteStream::input_ended() const { return _ended; }

size_t ByteStream::buffer_size() const { return _buf.size(); }

bool ByteStream::buffer_empty() const { return _buf.empty(); }

bool ByteStream::eof() const { return this->input_ended() && this->buffer_empty(); }

size_t ByteStream::bytes_written() const { return _wbytes; }

size_t ByteStream::bytes_read() const { return _rbytes; }

size_t ByteStream::remaining_capacity() const { return _capa - buffer_size(); }
