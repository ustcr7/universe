# universe

-- 版本迭代日志

-- v 0.0.1
--对tcp write和read进行封装,实现了最简单的gamesvr和client

-- v 0.0.2
-- 客户端和服务器通过协议文件进行通信
-- 实现tcpsvr可以管理多个客户端
-- tcpsvr接收缓存通过ringbuffer处理
-- 构建makefile

-- v 0.0.3
-- 接入存储模块(序列化与反序列化)
-- 

-- v 0.0.4
-- tcp client端接收缓存也用ring buffer处理
-- socket write函数不能保证一次写完
-- 协议文件优化 (引入protobuf)


-- v 0.0.5
--  注册, 登录, 同步玩家数据, 玩家请求-移动, 玩家下线  一套简单的逻辑流程 

-- v 0.0.6
-- 引入文本日志

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