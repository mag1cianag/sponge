#include "tcp_receiver.hh"
#include "wrapping_integers.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
  static size_t abs_seqno = 0;
  if (seg.header().syn) {
    if (_synReceived) {
      // already get a syn flag , refuse others
      return ;
    }
    _synReceived  = true;
    _isn  = seg.header().seqno;
    abs_seqno = 1;
    _base = 1;
  }else if (!_synReceived) {
    // listen state
    return;
  }else{
    //
    abs_seqno = unwrap(seg.header().seqno, _isn, abs_seqno);
  }

  if (seg.header().fin) {
    if(_finReceived) {
      // refuse other
      return;
    }
    _finReceived = true;
  }


  _reassembler.push_substring(seg.payload().copy(), abs_seqno-1, seg.header().fin);
  _base = _reassembler.ack_index() + 1;
  if (_reassembler.stream_out().input_ended()) {
    // fin ocppy a seqno
    _base +=1;
  }

}

optional<WrappingInt32> TCPReceiver::ackno() const {
  if (_base > 0) {
    return wrap(_base, _isn);
  }
  return nullopt;
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
