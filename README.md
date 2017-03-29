# universe
common codes for game server develop



---src
   ---common
       ---math
       ---date
       ---net
   ---proto
       xxx.pb
       xxx.h
       xxx.cc
   ---unit_test
   ---tools
      ---moc_client
   ---apps
      ---gamesvr
      ---loginsvr
      ---mailsvr

---lib

---tools

---bin
   ---gamesvr
      ---bin
      ---src
      ---cfg
   ---loginsvr
      ---bin
      ---src
      ---cfg
   ---mailsvr
      ---bin
      ---src
      ---cfg




==test:
生成.a文件:
cd src/common
g++ -g -c -I . ring_buffer/ring_buffer.cpp 
g++ -g -c -I . net/tcp_client.cpp
g++ -g -c -I . net/tcp_server.cpp

ar -r libcommon.a ring_buffer.o  tcp_client.o tcp_server.o 

编译gamesvr:
cd src/apps/gamesvr
g++ -g gamesvr.cpp -L../../common/ -lcommon -lpthread


编译moc_client:
cd src/tools/moc_client
g++ -g moc_client.cpp -L../../common/ -lcommon -lpthread



下版本优化:
makefile warn当errror
封装log函数