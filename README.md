# universe

-- 版本迭代日志

-- v 0.0.1
--对tcp write和read进行封装,实现了最简单的gamesvr和client

-- v 0.0.2
-- 客户端和服务器通过协议文件进行通信
-- 实现tcpsvr可以管理多个客户端
-- tcpsvr接收缓存通过ringbuffer处理
-- 构建makefile


---src
   ---common
       ---ring_buffer
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

---lib

---tools

---bin
   ---gamesvr
      ---bin
   ---tools
      ---moc_client
   ---unitest    