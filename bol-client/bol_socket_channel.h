#pragma once
#include "packet_reader.h"
#include <hv/TcpClient.h>

class BOLSocketChannel : public hv::SocketChannel
{
public:
	explicit BOLSocketChannel(hio_t* io) : SocketChannel(io) {}

	NetworkPacketReader reader_;
};