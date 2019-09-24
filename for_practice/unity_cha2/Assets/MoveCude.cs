//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;

//public class MoveCude : MonoBehaviour
//{

//    // Use this for initialization
//    void Start()
//    {

//    }

//    // Update is called once per frame
//    void Update()
//    {

//    }
//}


//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;
//using HoloToolkit.Unity.InputModule;

//public class MoveCude : MonoBehaviour, IFocusable
//{
//    public Material focusOn;
//    public Material focusOut;

//    public void OnFocusEnter()
//    {
//        GetComponent<Renderer>().material = focusOn;
//    }

//    public void OnFocusExit()
//    {
//        GetComponent<Renderer>().material = focusOut;
//    }
//}


//using UnityEngine;
//using System.Collections;
//using HoloToolkit.Unity.InputModule;

//public class MoveCude : MonoBehaviour, IInputClickHandler
//{
//    public Material first;
//    public Material second;

//    public void OnInputClicked(InputClickedEventData eventData)
//    {
//        var material = GetComponent<Renderer>().material;
//        GetComponent<Renderer>().material = (material.color == first.color) ? second : first;
//    }
//}



using UnityEngine;
using System.Collections;
//using System.Collections.Generic;
using HoloToolkit.Unity.InputModule;

public class MoveCude : MonoBehaviour, IInputClickHandler
{
    public Camera HololensCamara;

    void Start()
    {
        //AirTapを検出したとき、OnInputClickedが呼ばれる。
        InputManager.Instance.PushFallbackInputHandler(gameObject);
    }

    void Update()
    {

    }

    //AirTapを検出したとき呼ばれるメソッド
    public void OnInputClicked(InputClickedEventData eventData)
    {
        //AirTap検出時の処理を記述
        gameObject.AddComponent<Rigidbody>();
    }


}