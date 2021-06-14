#include <cstdio>
#include <stdexcept>
#include <cassert>
#include <string.h>

#include "SensorHttpServer.h"
#include "SensorBME280.h"
#include "SensorHistory.h"
#include "MeasureDataBase.h"
#include "Build.h"

#ifdef _DEBUG
#define TIME_MIN_IN_SECONDS 1
#else
#define TIME_MIN_IN_SECONDS 10
#endif

bool parse_commands(int *vArgc, char** vArgs, std::string* vI2cBus, uint32_t *vPort, std::string* vDBFile, uint64_t* vSaveDelay)
{
	assert(vArgc);
	assert(vArgs);
	assert(vI2cBus);
	assert(vPort);
	assert(vDBFile);
	assert(vSaveDelay);

	bool _CantContinue = false;
	bool _MustQuit = false;
	try
	{
		// when an erro ocurred, i continue the parsing, for show in one time all errors to the user
		// instead of break the loop after each errors
		
		// start at 1, because the arg 0 is the path of the app
		for (int i = 1; i < *vArgc; i++)
		{
			if (strcmp(vArgs[i], "-b") == 0) // i2cbus
			{
				if (i + 1 < *vArgc)
				{
					*vI2cBus = std::string(vArgs[i + 1]);
					i++; // jump to next options if any
				}
				else
				{
					printf("err, a i2c bus must be specified after the option -b\n");
					_CantContinue = true;
				}
			}
			else if (strcmp(vArgs[i], "-p") == 0) // port
			{
				if (i + 1 < *vArgc)
				{
					const char* port_s = vArgs[i + 1];
					int port_n = atoi(port_s); // this function car raise an exception, the try catch is for him
					*vPort = port_n;
					i++; // jump to next options if any
				}
				else
				{
					printf("err, a port number must be specified after the option -p\n");
					_CantContinue = true;
				}
			}
			else if (strcmp(vArgs[i], "-d") == 0) // db file
			{
				if (i + 1 < *vArgc)
				{
					*vDBFile = std::string(vArgs[i + 1]);
					i++; // jump to next options if any
				}
				else
				{
					printf("err, a database FilePathName must be specified after the option -d\n");
					_CantContinue = true;
				}
			}
			else if (strcmp(vArgs[i], "-t") == 0) // save delay
			{
				if (i + 1 < *vArgc)
				{
					const char* delay_s = vArgs[i + 1];
					int ns = atoi(delay_s); // this function car raise an exception, the try catch is for him
					if (ns >= TIME_MIN_IN_SECONDS)
					{
						*vSaveDelay = ns;
					}
					else
					{
						printf("err, the delay must be at mini %i after the option -t\n", TIME_MIN_IN_SECONDS);
						_CantContinue = true;
					}
					i++; // jump to next options if any
				}
				else
				{
					printf("err, a delay number must be specified after the option -t\n");
					_CantContinue = true;
				}
			}
			else if (strcmp(vArgs[i], "-v") == 0) // version
			{
				printf("bme280Server %s\n", BuildId);
				_MustQuit = true;
				
				// we could break or show erros if other options was specified in more than this one
				// but in fact i see no reason to show usage for this
				// ca be valid to launch the program and print the version
			}
			else if (strcmp(vArgs[i], "-h") == 0) // help
			{
				_CantContinue = true;
				_MustQuit = true;
				
				// we could break or show erros if other options was specified in more than this one
				// but in fact i see no reason to show usage for this
				// ca be valid to launch the program and print the help
			}
			else
			{
				printf("err, command %s not recognized\n", vArgs[i]);
				_CantContinue = true;
			}
		}
	}
	catch (std::exception& ex)
	{
		printf("err, command not recognized : %s\n", ex.what());
		_CantContinue = true;
	}

	if (_CantContinue)
	{
		printf("\n");
		printf("bme280Server %s\n", BuildId);
		printf("\n");
		printf("usage : bme280Server [-h] [-v] [-b i2cbus] [-p http_port] [-d database_file] [-t sensor_save_in_db_delay_in_seconds]\n");
		printf("\n");
		printf(" Options :\n");
		printf("  -h      Print this help\n");
		printf("  -v      Print the version\n");
		printf("  -b      Specify the i2cbus\n");
		printf("          By default, the ic2bus /dev/i2c-1 will be used\n");
		printf("  -p      Specify a port number for the http server to listen to\n");
		printf("          By default, the port 80 is used\n");
		printf("  -d      Specify the FilePathName of the database.\n");
		printf("          By default, db.db3 will be used\n");
		printf("  -t      Specify a delay in second for save bme280 Sensor datas in db\n");
		printf("          The minimal accepted value is 10s\n");
		printf("          By default the value will be 3600 (1h)\n");
		printf("\n");
		printf(" Description :\n");
		printf("  this programm get datas from a i2c sensor bme280 and can do many things with it :\n");
		printf("    - Act as a http server. the possible urls are :\n");
		printf("      - http://ip:port will show you a page with explanation of possible url options\n");
		printf("      - http://ip:port/sensor will launch a measure and give you the result in a json format\n");
		printf("      - http://ip:port/history:count will extract the last count measures from DB, and show you as a json format\n");
		printf("        count must be a valid number\n");
		printf("    - Can start a measure of the i2c sensor bme280 (temperature, pressure, humidity)\n");
		printf("    - Do logging each delay, a bm280 measure in a local sqlite database\n");

		return false;
	}

	return !_MustQuit;
}

int main(int argc, char** args)
{
	std::string default_i2cbus = "/dev/i2c-1";
	std::string default_db_file = "database.db3";
	uint64_t default_save_in_db_delay = 3600U;
	uint32_t default_port = 80;

	if (parse_commands(&argc, args, &default_i2cbus, &default_port , &default_db_file, &default_save_in_db_delay))
	{
		SensorHttpServer server(default_port);
		SensorBME280::Instance(default_i2cbus);
		MeasureDataBase::Instance(default_db_file);
		SensorHistory history(default_save_in_db_delay);

		try
		{
			uv::EventLoop loop;

			server.Init(loop);
			history.Init(loop);

			printf("bme280Server is started\n");
			loop.run();
		}
		catch (std::exception& ex)
		{
			printf("The server will stop.\nUnknown err was encountered : %s", ex.what());
		}
	}

	return 0;
}
