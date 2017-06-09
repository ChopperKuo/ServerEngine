#pragma once

#define PACKET_SIZE 1024 * 1024

namespace ServerEngine
{
	class NetPacket
	{
	private:
		uv_buf_t* m_buffers;
		int m_bufferCount;

	public:
		NetPacket();
		virtual ~NetPacket();

	public:
		void Split(const char* data, int length, int packetSize = PACKET_SIZE);

		const uv_buf_t* GetBuffers() const;

		int GetBufferCount() const;
	};
}
