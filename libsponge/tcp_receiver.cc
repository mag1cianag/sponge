#include "tcp_receiver.hh"

#include "wrapping_integers.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

/**
 *  \brief
 */
void TCPReceiver::segment_received(const TCPSegment &seg) {
    // judge syn
    const TCPHeader &header = seg.header();
    if (not _set_syn_flag) {
        if (not header.syn) {
            return;
        }
        _isn = header.seqno;
        _set_syn_flag = true;
    }
    // plus syn
    uint64_t abs_ackno = _reassembler.stream_out().bytes_written() + 1;
    uint64_t curr_abs_seqno = unwrap(header.seqno,_isn,abs_ackno);

    uint64_t stream_idx = curr_abs_seqno -1 + (header.syn);
    _reassembler.push_substring(seg.payload().copy(),stream_idx,header.fin);
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (not _set_syn_flag) {
        // listen
        return nullopt;
    }
    // plus syn
    uint64_t abs_ackno = _reassembler.stream_out().bytes_written() + 1;
    // plus fin
    if (_reassembler.stream_out().input_ended())
        abs_ackno += 1;
    return _isn + abs_ackno;
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
