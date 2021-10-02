 # BME280Server
 
 bme280Server is a Http server who expose sensor datas of a bme280 as a json
 
 ## how to generate configure file
 
 ./gen_configure.sh
 
 ## how to build and install
 
 after ./gen_configure.sh

 you need libuv1.0 and libsqlite3

 just do :

 ./configure
 
 make
 
 sudo make install
 
 sudo make install-service
 
 ## how to uninstall
 
 sudo make uninstall-service
 
 sudo make uninstall
 
 ## modifications
 
 if you want to modify the code, you need to build BuildInc (in libs) and install it
 he is need for inc build.h who contain the version number
 
 ## uses
 
 ```cpp
 usage : bme280Server [-h] [-v] [-b i2cbus] [-p http_port] [-d database_file] [-t sensor_save_in_db_delay_in_seconds]

Options :
  -h      Print this help
  -v      Print the version
  -b      Specify the i2cbus
          By default, the ic2bus /dev/i2c-1 will be used
  -p      Specify a port number for the http server to listen to
          By default, the port 80 is used
  -d      Specify the FilePathName of the database.
          By default, db.db3 will be used
  -t      Specify a delay in second for save bme280 Sensor datas in db
          The minimal accepted value is 10s
          By default the value will be 3600 (1h)

  Description :
    this programm get datas from a i2c sensor bme280 and can do many things with it :
     - Act as a http server. the possible urls are :
       - http://ip:port will show you a page with explanation of possible url options
       - http://ip:port/sensor will launch a measure and give you the result in a json format
       - http://ip:port/history:count will extract the last count measures from DB, and show you as a json format
         count must be a valid number
     - Can start a measure of the i2c sensor bme280 (temperature, pressure, humidity)
     - Do logging each delay, a bm280 measure in a local sqlite database
 ```
 
