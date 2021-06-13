ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/libs/BME280_driver \
              -I$(top_srcdir)/libs/uv_cpp \
              -I$(top_srcdir)/libs/uv_cpp/uv \
              -I$(top_srcdir)/src

bin_PROGRAMS = bme280Server

bme280Server_SOURCES =  \
	libs/BME280_driver/bme280.c \
	libs/BME280_driver/bme280.h \
	libs/BME280_driver/bme280_defs.h \
	libs/uv_cpp/uv/Async.cpp \
	libs/uv_cpp/uv/CycleBuffer.cpp \
	libs/uv_cpp/uv/DnsGet.cpp \
	libs/uv_cpp/uv/EventLoop.cpp \
	libs/uv_cpp/uv/GlobalConfig.cpp \
	libs/uv_cpp/uv/Idle.cpp \
	libs/uv_cpp/uv/ListBuffer.cpp \
	libs/uv_cpp/uv/LogWriter.cpp \
	libs/uv_cpp/uv/Packet.cpp \
	libs/uv_cpp/uv/Signal.cpp \
	libs/uv_cpp/uv/SocketAddr.cpp \
	libs/uv_cpp/uv/TcpAcceptor.cpp \
	libs/uv_cpp/uv/TcpClient.cpp \
	libs/uv_cpp/uv/TcpConnection.cpp \
	libs/uv_cpp/uv/TcpServer.cpp \
	libs/uv_cpp/uv/Timer.cpp \
	libs/uv_cpp/uv/Udp.cpp \
	libs/uv_cpp/uv/http/HttpClient.cpp \
	libs/uv_cpp/uv/http/HttpCommon.cpp \
	libs/uv_cpp/uv/http/HttpServer.cpp \
	libs/uv_cpp/uv/http/Request.cpp \
	libs/uv_cpp/uv/http/Response.cpp \
	libs/uv_cpp/uv/include/Async.hpp \
	libs/uv_cpp/uv/include/CycleBuffer.hpp \
	libs/uv_cpp/uv/include/DnsGet.hpp \
	libs/uv_cpp/uv/include/EventLoop.hpp \
	libs/uv_cpp/uv/include/GlobalConfig.hpp \
	libs/uv_cpp/uv/include/Idle.hpp \
	libs/uv_cpp/uv/include/ListBuffer.hpp \
	libs/uv_cpp/uv/include/LogWriter.hpp \
	libs/uv_cpp/uv/include/Packet.hpp \
	libs/uv_cpp/uv/include/PacketBuffer.hpp \
	libs/uv_cpp/uv/include/Signal.hpp \
	libs/uv_cpp/uv/include/SocketAddr.hpp \
	libs/uv_cpp/uv/include/TcpAcceptor.hpp \
	libs/uv_cpp/uv/include/TcpClient.hpp \
	libs/uv_cpp/uv/include/TcpConnection.hpp \
	libs/uv_cpp/uv/include/TcpServer.hpp \
	libs/uv_cpp/uv/include/Timer.hpp \
	libs/uv_cpp/uv/include/TimerWheel.hpp \
	libs/uv_cpp/uv/include/Udp.hpp \
	libs/uv_cpp/uv/include/uv11.hpp \
	libs/uv_cpp/uv/include/http/HttpClient.hpp \
	libs/uv_cpp/uv/include/http/HttpCommon.hpp \
	libs/uv_cpp/uv/include/http/HttpServer.hpp \
	libs/uv_cpp/uv/include/http/RadixTree.hpp \
	libs/uv_cpp/uv/include/http/Request.hpp \
	libs/uv_cpp/uv/include/http/Response.hpp \
	src/Build.h \
	src/MeasureDataBase.cpp \
	src/MeasureDataBase.h \
	src/SensorBME280.cpp \
	src/SensorBME280.h \
	src/SensorHistory.cpp \
	src/SensorHistory.h \
	src/SensorHttpServer.cpp \
	src/SensorHttpServer.h \
	src/main.cpp

bme280Server_CXXFLAGS = @CXXFLAGS@ -pthread

install-service:
	mkdir -p /usr/share/bme280Server
	cp $(srcdir)/bme280Server /usr/share/bme280Server/
	chmod u+x /usr/share/bme280Server/bme280Server
	cp $(srcdir)/bme280Server.service /usr/share/bme280Server/
	rm /etc/systemd/system/bme280Server.service 2> /dev/null || true
	ln -s /usr/share/bme280Server/bme280Server.service /etc/systemd/system/bme280Server.service
	systemctl daemon-reload
	systemctl enable bme280Server.service
	systemctl start bme280Server.service

uninstall-service:
	systemctl stop bme280Server.service
	systemctl disable bme280Server.service
	systemctl daemon-reload
	rm /usr/share/bme280Server/bme280Server
	rm /usr/share/bme280Server/bme280Server.service
	rm /etc/systemd/system/bme280Server.service

