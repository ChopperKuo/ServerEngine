#include "Core_PCH.h"

namespace ServerEngine
{

	NetPacket::NetPacket() :
		m_buffers(nullptr),
		m_bufferCount(0)
	{
	}

	NetPacket::~NetPacket()
	{
		if (m_buffers != nullptr)
		{
			for (int i = 0; i < m_bufferCount; ++i)
			{
				SAFE_DELETE_ARRAY(m_buffers[i].base);
			}

			SAFE_DELETE_ARRAY(m_buffers);
		}
	}

	void NetPacket::Split(const char* data, int length, int packetSize /*= PACKET_SIZE*/)
	{
		if (data == nullptr || length <= 0 || packetSize <= 0)
		{
			return;
		}

		// 1 length + n split buffer.
		m_bufferCount = (length - 1) / packetSize + 2;

		m_buffers = new uv_buf_t[m_bufferCount];
		memset(m_buffers, 0, sizeof(uv_buf_t) * m_bufferCount);

		for (int i = 0; i < m_bufferCount; ++i)
		{
			m_buffers[i].base = new char[packetSize];
			memset(m_buffers[i].base, 0, sizeof(char) * packetSize);
		}

		// fill length to buffer.
		m_buffers[0].base = (char*)&length;
		m_buffers[0].len = sizeof(int);

		// fill incoming data to buffer.
		for (int i = 1; i < m_bufferCount; ++i)
		{
			memcpy(m_buffers[i].base, data + (i - 1) * packetSize, packetSize);
		}
	}

	const uv_buf_t* NetPacket::GetBuffers() const
	{
		return m_buffers;
	}

	int NetPacket::GetBufferCount() const
	{
		return m_bufferCount;
	}

}
