#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`
using namespace std;

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {
}

long StreamReassembler::merge_block(block_node &elm1, const block_node &elm2) {
    // todo opt
    // copy
    block_node x, y;
    // x < y
    if (elm1 < elm2) {
        x = elm1;
        y = elm2;
    } else {
        x = elm2;
        y = elm1;
    }
    if (x.begin + x.length < y.begin) {
        // no intersection, couldn't merge
        return -1;
    } else if (x.begin + x.length >= y.begin + y.length) {
        // overlap whole y
        elm1 = x;
        return y.length;
    } else {
        // part overlap
        elm1.begin = x.begin;
        // add length
        elm1.data = x.data + y.data.substr(x.begin + x.length - y.begin);
        elm1.length = elm1.data.length();
        return x.begin + x.length - y.begin;
    }
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // capacity over
    if (index >= _head_index + _output.remaining_capacity()) {
        // equal means
        return;
    }

    // handle extra substring prefix
    block_node elm;
    if (index + data.length() <= _head_index) {
        // couldn't equal, because there have emtpy substring
        // useless substr
        goto JUDGE_EOF;
    } else if (index < _head_index) {
        size_t offset = _head_index - index;
        elm.data.assign(data.begin() + offset, data.end());
        elm.begin = index + offset;
        elm.length = elm.data.length();
    } else {
        // equal
        elm.begin = index;
        elm.length = data.length();
        elm.data = data;
    }
    _unassembled_byte += elm.length;

    // merge substring
    do {
        // merge next
        long merged_bytes = 0;
        auto iter = _blocks.lower_bound(elm);
        while (iter != _blocks.end() && (merged_bytes = merge_block(elm, *iter)) >= 0) {
            _unassembled_byte -= merged_bytes;
            _blocks.erase(iter);
            iter = _blocks.lower_bound(elm);
        }
        // merge prev
        if (iter == _blocks.begin()) {
            // no prev
            break;
        }
        iter--;
        while ((merged_bytes = merge_block(elm, *iter)) >= 0) {
            _unassembled_byte -= merged_bytes;
            _blocks.erase(iter);
            iter = _blocks.lower_bound(elm);
            if (iter == _blocks.begin()) {
                break;
            }
            iter--;
        }
     } while (false);
    _blocks.insert(elm);

    // write to ByteStream
    if (!_blocks.empty() && _blocks.begin()->begin == _head_index) {
        const block_node head_block = *_blocks.begin();
        // modify _head_index and _unassembled_byte according to successful write to _output
        size_t write_bytes = _output.write(head_block.data);
        _head_index += write_bytes;
        _unassembled_byte -= write_bytes;
        _blocks.erase(_blocks.begin());
    }

JUDGE_EOF:
    if (eof) {
        _eof_flag = true;
    }
    if (_eof_flag && empty()) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_byte; }

bool StreamReassembler::empty() const { return _unassembled_byte == 0; }

size_t StreamReassembler::ack_index() { return _head_index; }