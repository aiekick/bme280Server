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
 
 ## hwo to uninstall
 
 sudo make uninstall-service
 sudo make uninstall
 
 
 ## modifications
 
 if you want to modify the code, you need to build BuildInc (in libs) and install it
 he is need for inc build.h who contain the version number
 
