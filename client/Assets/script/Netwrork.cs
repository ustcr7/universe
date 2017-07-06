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

public class Netwrork : MonoBehaviour {
    public String textAreaString;
    public String textFieldString;
    public GUISkin mySkin;

    // Use this for initialization
    void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}


    void OnGUI()
    {
        GUI.skin = mySkin;
        //String s = textAreaString + "\n" + textAreaString + "\n" + textAreaString;
        //GUI.TextArea(new Rect(0, 50, 100, 60), s);
        //textFieldString = GUI.TextField(new Rect(0, 0, 100, 20), textFieldString);
        if (GUI.Button(new Rect(0, 0, 40, 20), "连接服务器"))
        {
            int connectRet = 0;
            //Debug.Log("连接消息发送成功");
            //建立一个线程,线程收发网络消息,收到消息时,调用注册的处理函数
            Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            String ip = "118.89.165.176";
            int port = 6789;
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

            //尝试和服务器建立连接,连接成功后打开登录界面
            if(connectRet == 0)
            {
                //发送消息
                UniverseMsg reg_msg = new UniverseMsg();
                reg_msg.msgHead = new UniverseMsgHead();
                reg_msg.msgHead.msgId = (int)UniverseMsgId.UNIVERSE_MSG_ID_ACTOR_REGISTE_REQ;
                reg_msg.msgHead.actorId = 10000;

                reg_msg.msgBody = new UniverseMsgBody();
                reg_msg.msgBody.registeReq = new ActorRegisteReq();
                reg_msg.msgBody.registeReq.id = 10000;
                reg_msg.msgBody.registeReq.name = "wcc";

                byte[] buf_msg = SerializerMgr.Serialize(reg_msg);

                int msg_len = buf_msg.Length;
                byte[] buf_len_msg = BitConverter.GetBytes(msg_len);

                byte[] total_buff = new byte[buf_msg.Length + buf_len_msg.Length];
                buf_len_msg.CopyTo(total_buff, 0);
                buf_msg.CopyTo(total_buff, buf_len_msg.Length);


                int sended = clientSocket.Send(total_buff, 0, total_buff.Length, SocketFlags.None);
                Debug.Log("发送消息完毕 长度");
                Debug.Log(msg_len.ToString());


                SceneManager.LoadScene("aa");
            }
            else
            {
                Debug.Log("连接服务器失败");
            }
            
        }
        else if (GUI.Button(new Rect(0, 25, 40, 20), "关闭"))
        {
            Debug.Log("关闭消息发送成功");
        }

        if (GUI.Button(new Rect(Screen.width - 60, Screen.height - 30, 60, 30), "退出"))
        {
            Application.Quit();
        }

    }

}
