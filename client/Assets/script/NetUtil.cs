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
namespace NetUtil
{

    public class TcpMgr
    {
        static public Socket clientSocket;

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
            return 0;
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

        static public T Deserialize<T>(byte[] message)
        {
            T result = default(T);
            if (message != null)
            {
                using (var stream = new MemoryStream(message))
                {
                    result = Serializer.Deserialize<T>(stream);
                }
            }
            return result;
        }
    }

    public class UniverseMsgMgr
    {
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
