using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
using universe_cs;
using System.IO;
using ProtoBuf;
using System;
using NetUtil;

public class ButtonHandle : MonoBehaviour {

	// Use this for initialization
	void Start () {


    }
	
	// Update is called once per frame
	void Update () {
	
	}

    public void Click()
    {

        
    }

    public void ChangToScene(string sceneToChangeTo)
    {
        UniverseMsgMgr.SendRegMsg(10000, "wcc");
        Debug.Log("注册消息发送成功");

        SceneManager.LoadScene(sceneToChangeTo);

    }
}
