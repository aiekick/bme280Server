#pragma once

#include <stdint.h>
#include <string>
#include <memory>
#include <uv/include/uv11.hpp>
#include <uv/include/TcpConnection.hpp>

class SensorHttpServer
{
private:
	std::unique_ptr<uv::http::HttpServer> m_Server;
	uint32_t m_Port;

public:
	SensorHttpServer(const uint32_t& vPort) : m_Port(vPort) {}
	void Init(uv::EventLoop& vEventLoop);

private:
	void OnConnected(std::weak_ptr<uv::TcpConnection> vPtr);
	void OnDisConnected(std::weak_ptr<uv::TcpConnection> vPtr);
	void Defaultpage(uv::http::Request& req, uv::http::Response* resp);
	void SendSensorDatas(uv::http::Request& req, uv::http::Response* resp);
	void SendHistory(uv::http::Request& req, uv::http::Response* resp);
};