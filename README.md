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
����.a�ļ�:
cd src/common
g++ -g -c -I . ring_buffer/ring_buffer.cpp 
g++ -g -c -I . net/tcp_client.cpp
g++ -g -c -I . net/tcp_server.cpp

ar -r libcommon.a ring_buffer.o  tcp_client.o tcp_server.o 

����gamesvr:
cd src/apps/gamesvr
g++ -g gamesvr.cpp -L../../common/ -lcommon -lpthread


����moc_client:
cd src/tools/moc_client
g++ -g moc_client.cpp -L../../common/ -lcommon -lpthread



�°汾�Ż�:
makefile warn��errror
��װlog����