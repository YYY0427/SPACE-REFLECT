using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class CustomMenu 
{
    enum ObjectType
    {
        Mesh, 
    }
    public static void WriteVector3(BinaryWriter bw, Vector3 vec)
    {
        bw.Write(vec.x);
        bw.Write(vec.y);
        bw.Write(vec.z);
    }

    [MenuItem("CustomMenu/ObjectData Exportar")]
    public static void Func()
    {
        var rootObject = Selection.activeGameObject;
        if(rootObject == null)
        {
            EditorUtility.DisplayDialog("配置データを出力できません", "配置データを出力するには、ルートオブジェクトを選択してください", "閉じる");
            return;
        }
        else
        {
            // SaveFilePanelはセーブしてくれない。文字列を返すだけ
            // セーブダイアログを出して、ユーザが選んだ出力フォルダが返される
            var path = EditorUtility.SaveFilePanel("配置データ出力", ".", "ObjectData.dat", "dat");
            var stream = File.Create(path);
            var bw = new BinaryWriter(stream);
           
            bw.Write(rootObject.transform.childCount);

            // 自分の子オブジェクトがいくつあるかを調べて子供の座標を出力する
            for(int i = 0; i < rootObject.transform.childCount; i++)
            {
                var gameObject = rootObject.transform.GetChild(i);

                // オブジェクト名
                bw.Write(gameObject.name);

                // 位置情報の出力
                var pos = gameObject.transform.position;
                WriteVector3(bw, pos);

                // 回転情報を出力
                // 度数法で出力
                var eular = gameObject.transform.eulerAngles;
                WriteVector3(bw, eular);

                // 拡大情報の出力
                var scale = gameObject.transform.localScale;
                WriteVector3(bw, scale);
            }

            bw.Close();
            stream.Close(); ;
            Debug.Log("ObjectData Exportarの実行が押されました");
            EditorUtility.DisplayDialog("配置データが出力されました", path + "に出力されました", "閉じる");
        }
    }
}
