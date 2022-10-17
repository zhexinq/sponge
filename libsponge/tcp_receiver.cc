#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    // Ignore segments when we don't have an ISN.
    if (!_isn.has_value() && !seg.header().syn) {
        return;
    }
    if (!_isn.has_value()) {
        _isn.emplace(seg.header().seqno);
    }
    auto stream_seqno = seg.header().syn ? seg.header().seqno + 1 : seg.header().seqno;
    _reassembler.push_substring(seg.payload().copy(),
                                unwrap(stream_seqno, _isn.value(), _reassembler.stream_out().bytes_written() + 1) - 1,
                                seg.header().fin);
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    auto extra = _reassembler.stream_out().input_ended() ? 1 : 0;
    return _isn.has_value() ? wrap(_reassembler.stream_out().bytes_written() + 1 + extra, _isn.value())
                            : optional<WrappingInt32>{};
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
