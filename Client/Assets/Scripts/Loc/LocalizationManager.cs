﻿using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class LocalizationManager
{
    //单例模式  
    private static LocalizationManager _instance;

    public static LocalizationManager GetInstance
    {
        get
        {
            if (_instance == null)
            {
                _instance = new LocalizationManager();
            }

            return _instance;
        }
    }

    private const string chinese = "Loc/Chinese";
    private const string english = "Loc/English";

    //选择自已需要的本地语言  
    public const string language = chinese;


    private Dictionary<string, string> dic = new Dictionary<string, string>();
    /// <summary>  
    /// 读取配置文件，将文件信息保存到字典里  
    /// </summary>  
    public LocalizationManager()
    {
        TextAsset ta = Resources.Load<TextAsset>(language);
        string text = ta.text;

        string[] lines = text.Split('\n');
        foreach (string line in lines)
        {
            if (line == null)
            {
                continue;
            }
            string[] keyAndValue = line.Split('=');
            dic.Add(keyAndValue[0], keyAndValue[1]);
        }
    }

    /// <summary>  
    /// 获取value  
    /// </summary>  
    /// <param name="key"></param>  
    /// <returns></returns>  
    public string GetValue(string key)
    {
        if (dic.ContainsKey(key) == false)
        {
            return null;
        }
        string value = null;
        dic.TryGetValue(key, out value);
        return value;
    }
}