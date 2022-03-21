TileEditor.cs
세부정보
활동
TileEditor.cs
공유 정보.
액세스 권한이 있는 사용자
공유되지 않음

액세스 관리
일반 정보.
시스템 속성
유형
텍스트
크기
13KB
사용한 용량
13KB
위치

Unity 10/17/16 <Delaunay Tri><Convexhull><Funnel_ray>
소유자
나
수정 날짜
2016. 10. 17.에 내가 수정
열어 본 날짜
오전 2:13에 내가 열어 봄
생성 날짜
2016. 10. 17.에 Google Drive Web 사용
설명.
설명 추가
다운로드 권한.
뷰어가 다운로드할 수 있음

﻿using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(TileBase))]
public class TileEditor : Editor
{
    TileBase TilebaseScript;
    int start_n, y, z, target_n, b, c, id, size, new_size;
    bool toggleTxt, toogleVector, toggleBox,toggleVectorMove, toggleVectorDelete;
    bool toggleTri_center, toggleVoronoi, toggleDISshow;
    GameObject[] source;

    // Use this for initialization
    void OnEnable()
    {
        TilebaseScript = target as TileBase;
    }
    public void OnSceneGUI()
    {
        Handles.BeginGUI();
        GUI.Label(new Rect(0, 0, 300, 15), "NAVI_MASH_WOO_MAP");        
        Handles.EndGUI();
        DrawCube();
    }
    
    public override void OnInspectorGUI()
    {
        EditorGUILayout.BeginVertical();
        TilebaseScript.start_Coordinates = EditorGUILayout.Vector3Field("World_StartPos", TilebaseScript.start_Coordinates);
        EditorGUILayout.EndVertical();

        EditorGUILayout.BeginHorizontal();
        EditorGUILayout.PrefixLabel("X_C");
        TilebaseScript.point_count_x =
        EditorGUILayout.IntSlider(TilebaseScript.point_count_x, 1, 40);
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.BeginHorizontal();
        EditorGUILayout.PrefixLabel("Z_C");
        TilebaseScript.point_count_z =
        EditorGUILayout.IntSlider(TilebaseScript.point_count_z, 1, 40);
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.BeginHorizontal();
        EditorGUILayout.PrefixLabel("point_interval");
        TilebaseScript.point_interval = EditorGUILayout.FloatField(TilebaseScript.point_interval);
        EditorGUILayout.EndHorizontal();
        
        EditorGUILayout.BeginHorizontal();
        if (GUILayout.Button("MAKE Point_MAP"))
        {
            TilebaseScript.FIRST();
        }
        EditorGUILayout.EndHorizontal();
              
        if (GUILayout.Button("SEND OBJECT"))
        {
            TilebaseScript.Navi_Mesh_Editing();
        }
        //FOR TRIANGLE INFORMATION TEXT
        EditorGUILayout.BeginHorizontal();
        toggleBox = GUILayout.Toggle(toggleBox, "Show Box");
        if (toggleBox == true)
        {
            toggleVectorMove = GUILayout.Toggle(toggleVectorMove, "Move Box");
            if(toggleVectorMove == true)
                toggleVectorDelete = GUILayout.Toggle(toggleVectorDelete, "Delete Box");
        }
        toggleTxt = GUILayout.Toggle(toggleTxt, "Show center");
        toogleVector = GUILayout.Toggle(toogleVector, "Show Vector");
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.BeginHorizontal();
        toggleVoronoi = GUILayout.Toggle(toggleVoronoi, "Show Voronoi");
        toggleTri_center = GUILayout.Toggle(toggleTri_center, "Show Center");
        toggleDISshow = GUILayout.Toggle(toggleDISshow, "Show Near");
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.BeginHorizontal();
        if (TilebaseScript.Navi_property_list != null && GUILayout.Button("2. SAVE"))
        {
            TilebaseScript.File_Save();
        }
        if (GUILayout.Button("2. LOAD"))
        {
            TilebaseScript.File_Load();
            TilebaseScript.Navi_Integrity_Check();
        }
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.BeginHorizontal();
        start_n = EditorGUILayout.IntField(start_n);
        target_n = EditorGUILayout.IntField(target_n);
        if (TilebaseScript.Navi_property_list != null && GUILayout.Button("FIND WAY A*"))
        {
            TilebaseScript.A_node_point_start(TilebaseScript.Navi_property_list[start_n].Triangle[3], TilebaseScript.Navi_property_list[target_n].Triangle[3]);
        }
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.BeginHorizontal();
        EditorGUILayout.PrefixLabel("MapMaterial");
        TilebaseScript.Mapmat = (Material)EditorGUILayout.ObjectField(TilebaseScript.Mapmat, typeof(Material), true);
        EditorGUILayout.EndHorizontal();
    }
    public void DrawCube()
    {
        if (TilebaseScript.Line_poin != null)
        {
            for (int n = 0; n < TilebaseScript.Line_poin.Count/2; n++)
            {
                Handles.color = new Color32(255, 0, 0, 255);
                Handles.DrawLine(TilebaseScript.Line_poin[0 + 2 * n], TilebaseScript.Line_poin[1 + 2 * n]);
            }
        }
        //MV TRI
        if (TilebaseScript.Navi_property_list != null)
        {
            for (int n = 0; n < TilebaseScript.mv_Tri.Count - 1; n++)
            {
                Handles.color = new Color32(255, 255, 0, 255);
                Handles.CylinderCap(n, TilebaseScript.mv_Tri[n], Quaternion.Euler(90, 0, 0), 0.2f);
                Handles.DrawLine(TilebaseScript.mv_Tri[n], TilebaseScript.mv_Tri[n + 1]);
            }
            for (int n = 0; n < TilebaseScript.mv_funnel.Count - 1; n++)
            {
                Handles.color = new Color32(0, 255, 0, 255);
                Handles.DrawLine(TilebaseScript.mv_funnel[n], TilebaseScript.mv_funnel[n + 1]);
            }
        }
        if (TilebaseScript.Point_list != null)
        {
            for (int i = 0; i < TilebaseScript.Point_list.Count; i++)
            {
                Color myColor = new Color32(255, 155, 100, 255);               
                Handles.color = myColor;
                if (toggleBox == true)
                {                    
                    bool box = Handles.Button(TilebaseScript.Point_list[i], Quaternion.identity, 0.1f, 0.1f, Handles.CubeCap);
                    
                    if (box && toggleVectorMove == true)
                    {
                        id = i;
                    }
                    if (toggleVectorMove == true && id == i)
                    {
                        GUIStyle style = new GUIStyle();
                        style.normal.textColor = Color.green;

                        string vectorValue = (TilebaseScript.Point_list[i]).ToString();
                        Handles.Label(TilebaseScript.Point_list[i],
                            "[" + i.ToString() + "]\n" + vectorValue, style);

                        Vector3 pos = Handles.PositionHandle(
                        TilebaseScript.Point_list[id],
                        Quaternion.identity
                         );
                        if (pos != TilebaseScript.Point_list[id])
                        {                            
                            TilebaseScript.Point_list[id] = pos;
                        }
                        
                        
                    }
                    if (toggleVectorDelete == true && id == i)
                    {
                        Handles.color = new Color32(255, 0, 0, 255);
                        if (Handles.Button(TilebaseScript.Point_list[i], Quaternion.identity, 0.3f, 0.3f, Handles.CubeCap))
                        {
                            TilebaseScript.Point_list.Remove(TilebaseScript.Point_list[id]);
                            Debug.Log("DELETE" + id);
                        }
                    }
                }
                if (toogleVector == true)
                {
                    GUIStyle style = new GUIStyle();
                    style.normal.textColor = Color.yellow;
                    string vectorValue = (TilebaseScript.Point_list[i]).ToString();
                    Handles.Label(TilebaseScript.Point_list[i], "["+i.ToString()+"]\n" + vectorValue, style);
                }
                if (toggleTxt == true)
                {
                    Vector3 pos = Handles.PositionHandle(
                        TilebaseScript.Point_list[i],
                        Quaternion.identity
                        );
                    if (pos != TilebaseScript.Point_list[i])
                    {
                        Debug.Log("Point[" + i + "] : " + pos);
                        TilebaseScript.Point_list[i] = pos;
                    }
                }                
            }
        }
        //voronoi line showing
        if (TilebaseScript.Navi_property_list != null && toggleVoronoi == true)
        {
            for (int i = 0; i < TilebaseScript.Navi_property_list.Count; i++)
            {
                NAVI_Property current_Navi = TilebaseScript.Navi_property_list[i];
                Color myColor = new Color32(0, 255, 50, 205);
                Handles.color = myColor;
                Vector3 circle_center = current_Navi.Triangle[4];
                Handles.ConeCap(i, circle_center, Quaternion.Euler(90,0,0), 1f);
            }
        }
        //Show Tri Center
        if (TilebaseScript.Navi_property_list != null && toggleTri_center == true)
        {            
            for (int i = 0; i < TilebaseScript.Navi_property_list.Count; i++)
            {
                Color myColor = new Color32(255, 178, 217, 205);
                if (TilebaseScript.Navi_property_list[i].block == true)
                    myColor = new Color32(255, 0, 0, 255);
                if (TilebaseScript.Navi_property_list[i].block == false)
                    myColor = new Color32(255, 255, 255, 50);
                
                NAVI_Property current_Navi = TilebaseScript.Navi_property_list[i];
                Vector3 center_pos = current_Navi.Triangle[3];
                
                // DELETE
                Handles.color = myColor;
                if(Handles.Button(center_pos, Quaternion.identity, 0.2f, 0.2f, Handles.CubeCap))
                {
                    Debug.Log(current_Navi.Triangle[0] + "/" + current_Navi.Triangle[1] + "/" + current_Navi.Triangle[2]);
                    if (current_Navi.near_triange[0] != null)
                    {
                        Debug.Log(current_Navi.near_triange[0].ID
      + " : ("
      + current_Navi.near_triange[0].e1_x
      + "," + current_Navi.near_triange[0].e1_y
      + "," + current_Navi.near_triange[0].e1_z
      + ")_("
      + current_Navi.near_triange[0].e2_x
      + "," + current_Navi.near_triange[0].e2_y
      + "," + current_Navi.near_triange[0].e2_z
      + ")");
                    }
                    if (current_Navi.near_triange[1] != null)
                    {
                        Debug.Log(current_Navi.near_triange[1].ID
      + " : ("
      + current_Navi.near_triange[1].e1_x
      + "," + current_Navi.near_triange[1].e1_y
      + "," + current_Navi.near_triange[1].e1_z
      + ")_("
      + current_Navi.near_triange[1].e2_x
      + "," + current_Navi.near_triange[1].e2_y
      + "," + current_Navi.near_triange[1].e2_z
      + ")");
                    }
                    if (current_Navi.near_triange[2] != null)
                    {
                        Debug.Log(current_Navi.near_triange[2].ID
     + " : ("
     + current_Navi.near_triange[2].e1_x
     + "," + current_Navi.near_triange[2].e1_y
     + "," + current_Navi.near_triange[2].e1_z
     + ")_("
     + current_Navi.near_triange[2].e2_x
     + "," + current_Navi.near_triange[2].e2_y
     + "," + current_Navi.near_triange[2].e2_z
     + ")");
                    }
                    //TilebaseScript.Navi_Mesh_delete(center_pos);
                }
                GUIStyle style = new GUIStyle();
                style.normal.textColor = Color.white;
                string vectorValue = "["+ current_Navi.Id + "]"+ (center_pos).ToString();
                Handles.Label(center_pos, vectorValue, style);
            }
        }
        //NEAR CONNECT LINE SHOWING
        if (TilebaseScript.Navi_property_list != null && toggleDISshow == true)
        {
            for (int i = 0; i < TilebaseScript.Navi_property_list.Count; i++)
            {
                NAVI_Property current_Navi = TilebaseScript.Navi_property_list[i];
                Vector3 center_pos = current_Navi.Triangle[3];  
                if(current_Navi.near_triange != null)              
                for (int k = 0; k < current_Navi.near_triange.Count; k++)
                {
                    int nearId = current_Navi.near_triange[k].ID;
                    Handles.color = new Color32(255, 255, 255, 255);
                    Handles.DrawLine(center_pos, TilebaseScript.Navi_property_list[nearId].Triangle[3]);
                    string dis = "[DIS : " + (current_Navi.near_triange[k].dis).ToString() + " ]";
                    GUIStyle style1 = new GUIStyle();
                    Vector3 half = (TilebaseScript.Navi_property_list[nearId].Triangle[3] + current_Navi.Triangle[3]) / 2;
                    Handles.Label(half, dis, style1);
                }
            }
        }
    }
}
