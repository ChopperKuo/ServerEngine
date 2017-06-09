#pragma once

namespace ServerEngine
{
	class NetSocket
	{
	private:
		uv_loop_t* m_loopHandle;
		uv_tcp_t* m_socketHandle;
		uv_buf_t m_reuseBuf;
		int m_lastErrorCode;

	public:
		NetSocket();
		NetSocket(uv_loop_t* loopHandle);
		virtual ~NetSocket();

	public:
		bool Bind(const char* ip, int port);

		bool Listen(int maxConnections);

		bool Accept();

		bool Connect(const char* ip, int port);

		bool Send(const char* data, int length);

		void Close();

		uv_loop_t* GetLoopHandle() const;

		uv_tcp_t* GetSocketHandle() const;

	private:
		static void UV_OnNewConnection(uv_stream_t* server, int status);

		static void UV_OnConnect(uv_connect_t* request, int status);

		static void UV_OnAllocBuf(uv_handle_t* handle, size_t size, uv_buf_t* buf);

		static void UV_OnRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

		static void UV_OnWrite(uv_write_t* request, int status);

		static void UV_OnShutdown(uv_shutdown_t* request, int status);
	};
}
