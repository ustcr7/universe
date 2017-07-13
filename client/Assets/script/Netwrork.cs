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
using System.Threading;
using NetUtil;



public class Netwrork : MonoBehaviour {
    public String textAreaString;
    public String textFieldString;
    public GUISkin mySkin;
    public 

    // Use this for initialization
    void Start () {
        int connectRet = TcpMgr.Init("118.89.165.176", 6788);
        //尝试和服务器建立连接,连接成功后打开登录界面
        if (connectRet == 0)
        {
            Debug.Log("connect success");
        }
        else
        {
            Debug.Log("连接服务器失败");
        }

        //WCC_TODO:创建线程,接受数据,然后Update()函数里读取这些数据进行处理
        Thread th = new Thread(TcpMgr.RecvDataFromSocket);
        th.Start();


       
    }

    // Update is called once per frame
    void Update () {

        //lock(TcpMgr.GetMsgQueue())
        //{ 
            UniverseMsg msg = TcpMgr.PopQueueMsg();
            if (msg == null)
            {
                return;
            }
            UniverseMsgMgr.RecvServerMsg(msg);
        //}
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
