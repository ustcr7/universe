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



public class Netwrork : MonoBehaviour {
    public String textAreaString;
    public String textFieldString;
    public GUISkin mySkin;

    // Use this for initialization
    void Start () {
        int connectRet = TcpMgr.Init("118.89.165.176", 6789);
        //尝试和服务器建立连接,连接成功后打开登录界面
        if (connectRet == 0)
        {
            Debug.Log("connect success");
        }
        else
        {
            Debug.Log("连接服务器失败");
        }
    }
	
	// Update is called once per frame
	void Update () {
        return;




        //尝试从网络读取数据
        int max_byte_len = 1024;
        byte[] recvBytes = new byte[max_byte_len];

        int read_len = TcpMgr.clientSocket.Receive(recvBytes, max_byte_len, 0);//从服务器端接受返回信息
        if(read_len <= 0)
        {
            Debug.Log("no data");
            return;
        }

        //WCCTODO: 先假设每个网络包在一次recv中处理成功,切没有分包(这是个明显的bug,后续需要改掉)
        int msg_byte_len = BitConverter.ToInt32(recvBytes, 0);
        UniverseMsg msg = NetUtil.SerializerMgr.Deserialize<UniverseMsg>(recvBytes);

        UniverseMsgMgr.RecvServerMsg(msg);
    }

    void OnGUI()
    {
        GUI.skin = mySkin;
        //String s = textAreaString + "\n" + textAreaString + "\n" + textAreaString;
        //GUI.TextArea(new Rect(0, 50, 100, 60), s);
        //textFieldString = GUI.TextField(new Rect(0, 0, 100, 20), textFieldString);
        if (GUI.Button(new Rect(0, 0, 40, 20), "注册"))
        {
            UniverseMsgMgr.SendRegMsg(10000, "wcc");
            Debug.Log("注册消息发送成功");
        }
        else if (GUI.Button(new Rect(0, 25, 40, 20), "关闭"))
        {
            Debug.Log("关闭消息发送成功");
        }
        else if(GUI.Button(new Rect(0, 50, 40, 20), "登录"))
        {
            Debug.Log("登录消息发送成功");
        }
        else if (GUI.Button(new Rect(0, 75, 40, 20), "移动"))
        {
            Debug.Log("移动消息发送成功");
        }

        if (GUI.Button(new Rect(Screen.width - 60, Screen.height - 30, 60, 30), "退出"))
        {
            Application.Quit();
        }

    }

}
