using UnityEngine;
using System;
using UnityEngine.SceneManagement;
using System.Net.Sockets;
using System.Net;

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
