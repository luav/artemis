#pragma once

#include "RingBuffer.h"

#include <asio/streambuf.hpp>
#include <memory>


typedef RingBuffer<asio::streambuf,16> SerializedMessageBuffer;


struct NewAntDescription {
	int32_t ID;
	std::shared_ptr< std::vector<uint8_t> > PNGData;
};

typedef RingBuffer<NewAntDescription,256> UnknownAntsBuffer;
