using UnityEngine;
using System;
using UnityEngine.SceneManagement;
using System.Net.Sockets;
using System.Net;
using UnityEngine;
using System.Collections;
using universe_cs;
using System.IO;
using ProtoBuf;
using System;
using System.Net.Sockets;
using System.Net;
using NetUtil;
using System.Threading;


namespace NetUtil
{

    public class TcpMgr
    {
        static public Socket clientSocket;
        static public Queue msgQueue;

        static public UniverseMsg PopQueueMsg()
        {
            lock(msgQueue)
            {

                if(msgQueue.Count > 0)
                { 
                    Debug.Log("try pop msg");
                    System.Object msg = msgQueue.Dequeue();
                    if (msg != null)
                    {
                        return (UniverseMsg)msg;
                    }
                }
            }
            return null;
        }
        static public Queue GetMsgQueue()
        {
            return msgQueue;
        }

        static public int Init(String ip, int port)
        {
            int connectRet = 0;
            clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            System.Net.IPEndPoint ipe = new IPEndPoint(IPAddress.Parse(ip), port);
            try
            {
                clientSocket.Connect(ipe);
                Debug.Log(" Connect Success IP: " + ip + " Port : " + port.ToString());
            }
            catch (Exception e)
            {
                connectRet = -1;
                Debug.LogError(e.ToString());
            }

            if(connectRet != 0)
            {
                Debug.Log("连接服务器失败");
                return -1;
            }
            msgQueue = new Queue();

            Thread th = new Thread(TcpMgr.RecvDataFromSocket);
            th.Start();

            Debug.Log("init queue success");
            return 0;
        }

        public static void RecvDataFromSocket()
        {
            while (true)
            {
                //Debug.Log("try recv data");
                Thread.Sleep(500);

                //尝试从网络读取数据
                //byte[] result = new byte[1024];

               // int max_byte_len = 1024;
                byte[] recvBytes = new byte[1024];
                int read_len = TcpMgr.clientSocket.Receive(recvBytes, recvBytes.Length, 0);//从服务器端接受返回信息
                Debug.Log(string.Format("recive success len {0}, bytes {1}", read_len, recvBytes.Length));
                //int read_len = clientSocket.Receive(result);
                if (read_len > 0)
                {
                    Debug.LogFormat(string.Format("read len {0}", read_len));
                    //lock (msgQueue)
                    {
                        //WCCTODO: 先假设每个网络包在一次recv中处理成功,切没有分包(这是个明显的bug,后续需要改掉)
                        Debug.Log("aaa");
                        int msg_byte_len = BitConverter.ToInt32(recvBytes, 0);
                        Debug.Log(string.Format("msg body len{0}", msg_byte_len));

                        UniverseMsg msg = NetUtil.SerializerMgr.Deserialize<UniverseMsg>(recvBytes, 4, msg_byte_len);//WCCTODO OFFSET
                        Debug.Log("ccc");
                        msgQueue.Enqueue(msg);
                        Debug.Log("recv msg data\n");
                        Debug.Log(string.Format(">>>>>>>>>>>msgid:{0}, actorid:{1}", msg.msgHead.msgId, msg.msgHead.actorId));
                    }
                }
                else
                {
                    Debug.Log("no data");
                }

            }
        }

        static public int SendMsg(UniverseMsg msg)
        {
            byte[] buf_msg = NetUtil.SerializerMgr.Serialize(msg);

            int msg_len = buf_msg.Length;
            byte[] buf_len_msg = BitConverter.GetBytes(msg_len);

            byte[] total_buff = new byte[buf_msg.Length + buf_len_msg.Length];
            buf_len_msg.CopyTo(total_buff, 0);
            buf_msg.CopyTo(total_buff, buf_len_msg.Length);


            int sended = clientSocket.Send(total_buff, 0, total_buff.Length, SocketFlags.None);
            Debug.Log("发送消息完毕 长度");
            Debug.Log(msg_len.ToString());
            return 0;
        }

    }

    public class SerializerMgr
    {
        static public byte[] Serialize<T>(T msg)
        {
            byte[] result = null;
            if (msg != null)
            {
                using (var stream = new MemoryStream())
                {
                    Serializer.Serialize<T>(stream, msg);
                    result = stream.ToArray();
                }
            }
            return result;
        }

        static public T Deserialize<T>(byte[] message, int start_idx, int count)
        {
            T result = default(T);
            if (message != null)
            {
                using (var stream = new MemoryStream(message,start_idx, count))
                {
                    result = Serializer.Deserialize<T>(stream);
                }
            }
            return result;
        }
    }

    public class UniverseMsgMgr
    {
        //static public GameObject playerPrefab;
        static public int SendRegMsg(ulong actorid, String name)
        {

            UniverseMsg reg_msg = new UniverseMsg();
            reg_msg.msgHead = new UniverseMsgHead();
            reg_msg.msgHead.msgId = (int)UniverseMsgId.UNIVERSE_MSG_ID_ACTOR_REGISTE_REQ;
            reg_msg.msgHead.actorId = 10000;

            reg_msg.msgBody = new UniverseMsgBody();
            reg_msg.msgBody.registeReq = new ActorRegisteReq();
            reg_msg.msgBody.registeReq.id = 10000;
            reg_msg.msgBody.registeReq.name = "wcc";

            return TcpMgr.SendMsg(reg_msg);
        }

        static public int RecvServerMsg(UniverseMsg msg)
        {
            switch(msg.msgHead.msgId)
            {
                case (int)UniverseMsgId.UNIVERSE_MSG_ID_ACTOR_REGISTE_RSP:
                    {
                        Debug.Log("recv registe rsp");
                        Vector3 c = new Vector3();
                        c.Set(0, 0, 0);
                        Quaternion d = new Quaternion();
                        d.Set(0, 0, 0, 0);
                        string ad = "palyerPrefab";
                        GameObject.Instantiate(Resources.Load(ad), c, d);
                        //GameObject palyer = (GameObject)GameObject.Instantiate(playerPrefab);
                        //debug code:
                        /*
                        GameObject obj1 = GameObject.CreatePrimitive(PrimitiveType.Cube);
                        if(obj1 != null)
                        {
                            Debug.Log("create obj success");
                        }
                        obj1.transform.position = new Vector3(1, 1, 1);
                        obj1.name = "player";
                        obj1.transform.Rotate(new Vector3(10, 10, 10));
                        obj1.transform.Translate(new Vector3(10, 10, 10));


                        GameObject obj3 = GameObject.CreatePrimitive(PrimitiveType.Cube);
                        obj3.transform.position = new Vector3(1, 1, 1);
                        obj3.name = "player2";
                        if (obj3 != null)
                        {
                            Debug.Log("create obj success");
                        }
                        */
                        break;
                    }
                case (int)UniverseMsgId.UNIVERSE_MSG_ID_ACTOR_LOGIN_RSP:
                    {
                        Debug.Log("recv login rsp");
                        break;
                    }
                case (int)UniverseMsgId.UNIVERSE_MSG_ID_ACTOR_LOGOUT_RSP:
                    {
                        Debug.Log("recv logout rsp");
                        break;
                    }
                case (int)UniverseMsgId.UNIVERSE_MSG_ID_ACTOR_GET_FULL_DATA_RSP:
                    {
                        break;
                    }
                case (int)UniverseMsgId.UNIVERSE_MSG_ID_ACTOR_SET_POS_RSP:
                    {
                        break;
                    }
                case (int)UniverseMsgId.UNIVERSE_MSG_ID_ACTOR_MOVE_RSP:
                    {
                        break;
                    }
                case (int)UniverseMsgId.UNIVERSE_MSG_ID_CHAT_RSP:
                    {
                        break;
                    }
                case (int)UniverseMsgId.UNIVERSE_MSG_ID_LEARN_SPELL_RSP:
                    {
                        break;
                    }
                case (int)UniverseMsgId.UNIVERSE_MSG_ID_CAST_SPELL_RSP:
                    {
                        break;
                    }
                default:
                    {
                        Debug.Log("invalid msg id");
                        break;
                    }
            }
            return 0;
        }
    }

}
