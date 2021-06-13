#include "SensorHttpServer.h"

#include <stdint.h>
#include <string.h>
#include <stdexcept>
#include <sstream>

#include "SensorBME280.h"
#include "MeasureDataBase.h"

void SensorHttpServer::Init(uv::EventLoop& vEventLoop)
{
	uv::http::HttpServer::SetBufferMode(uv::GlobalConfig::BufferMode::CycleBuffer);
	m_Server = std::unique_ptr<uv::http::HttpServer>(new uv::http::HttpServer(&vEventLoop));

	// default page
	//example:  127.0.0.1:80/
	m_Server->Get("/", std::bind(&SensorHttpServer::Defaultpage, this, std::placeholders::_1, std::placeholders::_2));

	//example:  127.0.0.1:80/sensor
	m_Server->Get("/sensor*", std::bind(&SensorHttpServer::SendSensorDatas, this, std::placeholders::_1, std::placeholders::_2));

	//example:  127.0.0.1:80/history:12
	m_Server->Get("/history:", std::bind(&SensorHttpServer::SendHistory, this, std::placeholders::_1, std::placeholders::_2));

	m_Server->setNewConnectCallback(std::bind(&SensorHttpServer::OnConnected, this, std::placeholders::_1));
	m_Server->setConnectCloseCallback(std::bind(&SensorHttpServer::OnDisConnected, this, std::placeholders::_1));

	uv::SocketAddr addr("0.0.0.0", m_Port, uv::SocketAddr::Ipv4);

	static uint32_t time_out_s = 60U * 5U; // 1m * 5 > 5m
	m_Server->setTimeout(time_out_s);

	m_Server->bindAndListen(addr);
}

////////////////////////////////////////////////////
//// PRIVATE ///////////////////////////////////////
////////////////////////////////////////////////////

void SensorHttpServer::OnConnected(std::weak_ptr<uv::TcpConnection> vPtr)
{
	if (!vPtr.expired())
	{
		auto p = vPtr.lock();
		if (p)
		{
			printf("Connected to %s\n", p->Name().c_str());
		}
	}
}

void SensorHttpServer::OnDisConnected(std::weak_ptr<uv::TcpConnection> vPtr)
{
	if (!vPtr.expired())
	{
		auto p = vPtr.lock();
		if (p)
		{
			printf("DisConnected from %s\n", p->Name().c_str());
		}
	}
}
void SensorHttpServer::Defaultpage(uv::http::Request& /*req*/, uv::http::Response* resp)
{
	resp->setVersion(uv::http::HttpVersion::Http1_1);
	resp->setStatus(uv::http::Response::StatusCode::OK, "OK");
	std::string help = u8R"(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>BME280 HTTP Server</title>
		<style>
			@import url('https://fonts.googleapis.com/css.family=Poppins:200,300,400,500,600,700,800,900&display=swap');
			*
			{
				margin:0;
				padding:0;
				box-sizing:border-box;
				font-family: 'Poppins', sans-serif;
			}
			body
			{
				display:flex;
				justify-content: center;
				align-items:center;
				background:gainsboro;
				padding: 10px;
			}
			.container
			{
				align-items: center;
				flex-wrap: wrap;
				max-width: 600px;
				color:rgb(100, 100, 100);
			}
			h1
			{
				color:rgb(100, 100, 100);
			}
			ul 
			{
				list-style: none;
				font-family: 'Poppins', sans-serif;
				text-shadow: 0 1px 0 rgb(100, 100, 100,0.1);
				align-items: center;
			}
			li
			{
				position: relative;
				top:10px;
				display: block;
				background-color:rgb(100, 100, 100, 0.1);
				border-radius: 5px;
				padding: 2px;
				margin:5px;
			}
			.container .card
			{
				box-shadow: inset 5px 5px 5px rgba(0,0,0,0.05),
							inset -5px -5px 5px rgba(255,255,255,0.5),
							5px 5px 5px rgba(0,0,0,0.05),
							-5px -5px 5px rgba(255,255,255,0.5);
				border-radius: 25px;
				margin: 20px;
				padding: 10px;
			}
			.container .card .box
			{
				position:relative;
				background:lightgray;
				box-shadow: 0px 5px 20px rgba(0,0,0,0.2);
				border-radius: 15px;
				display: flex;
				justify-content: center;
				align-items: center;
				transition: 0.5s;
			}
			.container .card:hover .box
			{
				background:linear-gradient(45deg, rgba(200,200,200,0.25), rgba(100,100,100,0.25));
				transform: translateY(-10px);
				
			}
			.container .card .box .content
			{
				padding: 5px;
				margin: 5px;
				text-align: center;
			}
			h1
			{
				color:rgb(100, 100, 100);
				font-size:2em;
			}
			h2
			{
				color:rgb(49, 44, 44);
				font-size:1.5em;
			}
			.container .card .box .content p
			{
				font-size: 1em;
				font-weight: 300;
				color: #777;
				z-index: 1;
				transition: 0.5s;
			}
			a
			{
				display: inline-block;
				padding:5px;
				margin: 5px;
				border-radius: 5px;
				background: #03a9f4;
				color: #fff;
				text-decoration: none;
				font-weight: 400;
				box-shadow: 0 5px 10px rgba(0,0,0,0.1);
			}
			.container .card .box .content a:hover
			{
				background: #ff568f;
				color: #fff;
			}
		</style>
    </head>
    <body>
		<div class="container">
			<div class="card">
				<div class="box">
					<div class="content">
						<h1>BME280 HTTP Server</h1>	
					</div>
				</div>
			</div>
			<div class="card">
				<div class="box">
					<div class="content">
						<h2>This Server expose JSon Datas of</h2>
						<p><a href="https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/">I2C Sensor BME280</a>
					</div>
				</div>
			</div>
			<div class="card">
				<div class="box">
					<div class="content">
						<h2>Available datas per measures</h2>
						<ul>
							<li>Epoch time (seconds)</li>
							<li>Temperature (Celsius)</li>
							<li>Pressure (hPa)</li>
							<li>Humidity (%)</li>
						</ul>
					</div>
				</div>
			</div>
			<div class="card">
				<div class="box">
					<div class="content">
						<h2>Availables Urls queries</h2>
						<ul>
							<li>
								<div class="link">
									<p>Start a measure of the sensor</p>
									<p>Ex : <a href="/sensor">http://ip:port/sensor</a></p>
								</div>
							</li>
							<li>
								<div class="link">
									<p>Retrieve an history of last N hourly sensor measures</p>
									<p>Ex : <a href="/history:12">http://ip:port/history:N (here n is 12)</a></p>
								</div>
							</li>
						</ul>
					</div>
				</div>
			</div>
			<div class="card">
				<div class="box">
						<div class="content">
							<p>Happy Logging :)</p>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
    </body>
</html>
)";
	resp->swapContent(help);
}

void SensorHttpServer::SendSensorDatas(uv::http::Request& /*req*/, uv::http::Response* resp)
{
	resp->setVersion(uv::http::HttpVersion::Http1_1);
	resp->setStatus(uv::http::Response::StatusCode::OK, "OK");
	resp->appendHead("Content-Type", "application/json");

	std::string json = SensorBME280::Instance()->GetSensorBME280DatasToJSON();
	
	resp->swapContent(json);
}

void SensorHttpServer::SendHistory(uv::http::Request& req, uv::http::Response* resp)
{
	resp->setVersion(uv::http::HttpVersion::Http1_1);
	resp->setStatus(uv::http::Response::StatusCode::OK, "OK");
	resp->appendHead("Content-Type", "application/json");

	// sqlite extract

	std::string json;
	auto countStr = req.getValue();
	if (!countStr.empty())
	{
		int count = 0;

		try
		{
			count = atoi(countStr.c_str()); // atoi can raise an exception
		}
		catch (std::exception &ex)
		{
			printf("Err : %s\n", ex.what());
			count = 0;
		}

		if (count)
		{
			// 4a is converted by atoi without exception to a. 
			// its not a problem. the json count value will use 4 and not 4a in MeasureDataBase::GetHistoryToJson
			json = MeasureDataBase::Instance()->GetHistoryToJson(count);
		}
	}

	if (json.empty())
	{
		static char err[256 + 1] = "";
		
		const char* value = req.getValue().c_str();
		
		snprintf(err, 256, "{\"error\":\"history count value %s is a bad value. must be a valid number\"}", value);
		printf("Error :\n    The url query : %s%s is bad\n    The history count number %s must be a valid number\n", req.getPath().c_str(), value, value);
		
		json = std::string(err);
	}
	else
	{
		json = "{" + json + "}";
	}

	resp->swapContent(json);
}
