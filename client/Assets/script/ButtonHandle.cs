using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;

public class ButtonHandle : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    public void Click()
    {
        Debug.Log("登录成功");
    }

    public void ChangToScene(string sceneToChangeTo)
    {
        SceneManager.LoadScene(sceneToChangeTo);
    }
}
