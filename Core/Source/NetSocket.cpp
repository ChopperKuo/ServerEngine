#include "Core_PCH.h"

#define CHECK(x) m_lastErrorCode = x; if (m_lastErrorCode != 0) { break; }
#define CHECK_EX(c, x) c->m_lastErrorCode = x; if (c->m_lastErrorCode != 0) { break; }

namespace ServerEngine
{

	NetSocket::NetSocket() :
		NetSocket(uv_default_loop())
	{
	}

	NetSocket::NetSocket(uv_loop_t* loopHandle) :
		m_loopHandle(loopHandle),
		m_lastErrorCode(0)
	{
		m_socketHandle = new uv_tcp_t();
		m_socketHandle->data = this;
		uv_tcp_init(m_loopHandle, m_socketHandle);

		m_reuseBuf.base = nullptr;
		m_reuseBuf.len = 0;
	}

	NetSocket::~NetSocket()
	{
		/*if (uv_is_active((uv_handle_t*)m_socketHandle))
		{
			uv_close((uv_handle_t*)m_socketHandle, nullptr);
			uv_run(m_loopHandle, UV_RUN_DEFAULT);
		}*/

		SAFE_DELETE(m_socketHandle);
		SAFE_DELETE_ARRAY(m_reuseBuf.base);
	}

	bool NetSocket::Bind(const char* ip, int port)
	{
		do 
		{
			sockaddr_in addr;
			CHECK(uv_ip4_addr(ip, port, &addr));

			CHECK(uv_tcp_bind(m_socketHandle, (const struct sockaddr*)&addr, 0));

			return true;
		} while (false);

		return false;
	}

	bool NetSocket::Listen(int maxConnections)
	{
		do
		{
			CHECK(uv_listen((uv_stream_t*)m_socketHandle, maxConnections, UV_OnNewConnection));

			return true;
		} while (false);

		return false;
	}

	bool NetSocket::Connect(const char* ip, int port)
	{
		uv_connect_t* request = new uv_connect_t();

		do 
		{
			sockaddr_in addr;
			CHECK(uv_ip4_addr(ip, port, &addr));

			CHECK(uv_tcp_connect(request, m_socketHandle, (const struct sockaddr*)&addr, UV_OnConnect));

			return true;
		} while (false);

		SAFE_DELETE(request);
		return false;
	}

	bool NetSocket::Send(const char* data, int length)
	{
		NetPacket* packet = new NetPacket();
		packet->Split(data, length);

		uv_write_t request;
		request.data = packet;
		m_lastErrorCode = uv_write(&request, (uv_stream_t*)m_socketHandle, packet->GetBuffers(), packet->GetBufferCount(), UV_OnWrite);
		if (m_lastErrorCode != 0)
		{
			SAFE_DELETE(packet);
			return false;
		}

		return true;
	}

	void NetSocket::Close()
	{
		uv_shutdown_t request;
		m_lastErrorCode = uv_shutdown(&request, (uv_stream_t*)m_socketHandle, UV_OnShutdown);
		if (m_lastErrorCode != 0)
		{
			request.handle = (uv_stream_t*)m_socketHandle;
			UV_OnShutdown(&request, m_lastErrorCode);
		}
	}

	uv_loop_t* NetSocket::GetLoopHandle() const
	{
		return m_loopHandle;
	}

	uv_tcp_t* NetSocket::GetSocketHandle() const
	{
		return m_socketHandle;
	}

	void NetSocket::UV_OnNewConnection(uv_stream_t* server, int status)
	{
		if (server == nullptr || server->data == nullptr)
		{
			return;
		}

		NetSocket* serverSocket = (NetSocket*)server->data;

		if (status < 0)
		{
			serverSocket->m_lastErrorCode = status;
			return;
		}

		NetSocket* clientSocket = new NetSocket(serverSocket->m_loopHandle);

		do 
		{
			CHECK_EX(serverSocket, uv_accept((uv_stream_t*)serverSocket->m_socketHandle, (uv_stream_t*)clientSocket->m_socketHandle));

			CHECK_EX(serverSocket, uv_read_start((uv_stream_t*)clientSocket->m_socketHandle, UV_OnAllocBuf, UV_OnRead));

			// TODO: Notify out.

			return; // Success.
		} while (false);

		SAFE_DELETE(clientSocket);
	}

	void NetSocket::UV_OnConnect(uv_connect_t* request, int status)
	{
		do 
		{
			if (request == nullptr || request->handle == nullptr || request->handle->data == nullptr)
			{
				break;
			}

			NetSocket* socket = (NetSocket*)request->handle->data;

			CHECK_EX(socket, status);

			CHECK_EX(socket, uv_read_start((uv_stream_t*)socket->m_socketHandle, UV_OnAllocBuf, UV_OnRead));

		} while (false);

		SAFE_DELETE(request);
	}

	void NetSocket::UV_OnAllocBuf(uv_handle_t* handle, size_t size, uv_buf_t* buf)
	{
		if (handle == nullptr || handle->data == nullptr)
		{
			return;
		}

		NetSocket* serverSocket = (NetSocket*)handle->data;

		if (serverSocket->m_reuseBuf.len != size)
		{
			SAFE_DELETE_ARRAY(serverSocket->m_reuseBuf.base);
			serverSocket->m_reuseBuf.base = new char[size];
			serverSocket->m_reuseBuf.len = (unsigned long)size;
		}

		buf->base = serverSocket->m_reuseBuf.base;
		buf->len = serverSocket->m_reuseBuf.len;
	}

	void NetSocket::UV_OnRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
	{
		//if (nread < 0)
		//{
		//	uv_shutdown_t* shutdownReq = new uv_shutdown_t();
		//	uv_shutdown(shutdownReq, stream, on_shutdown);
		//}
	}

	void NetSocket::UV_OnWrite(uv_write_t* request, int status)
	{
		SAFE_DELETE(request->data);
	}

	void NetSocket::UV_OnShutdown(uv_shutdown_t* request, int status)
	{

	}

}
