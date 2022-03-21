TileBase.cs
세부정보
활동
TileBase.cs
공유 정보.
액세스 권한이 있는 사용자
공유되지 않음

액세스 관리
일반 정보.
시스템 속성
유형
텍스트
크기
47KB
사용한 용량
47KB
위치

22/10/16 <DelaunayTri><Convex><Funnel_Edge_crossPoint>
소유자
나
수정 날짜
2016. 10. 23.에 내가 수정
열어 본 날짜
오전 2:11에 내가 열어 봄
생성 날짜
2016. 10. 23.에 Google Drive Web 사용
설명.
설명 추가
다운로드 권한.
뷰어가 다운로드할 수 있음

﻿using UnityEngine;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

public class TileBase : MonoBehaviour
{
    public Vector3 start_Coordinates;
    //A# algorithm
    public int point_count_x;
    public int point_count_z;
    public float point_interval;
    public List<Vector3> BASE_point;
    //--------showing
    [SerializeField]
    public List<Vector3> Point_list;//Do not Reset if u made map already.    
    //------------
    public List<Vector3> Line_poin;
    //A*
    List<NAVI_Property> open_tri_l;
    List<NAVI_Property> close_tri_l;
    public List<Vector3> mv_Tri;
    public List<Vector3> mv_funnel;
    NAVI_Property start_node;
    NAVI_Property target_node;
    //ABOUT MESH COMPONENT(MADE BY UNITY)
    private Mesh mesh;
    private Mesh Line_mesh;
    public Material mat;
    public Material Mapmat;
    //Delaunay triangulation
    public List<Vector3[]> block_edge_list;
    //FOR NAVI STATE 
    [SerializeField]
    public List<NAVIDATA> Navi_data_list;//FOR SAVE AND LOAD
    public List<NAVI_Property> Navi_property_list;
    //FILE SAVER AND LODER
    //-----------SAVE
    public void File_Save()
    {
        Navimesh_data_save();
        Point_data_save();
    }
    public void Navimesh_data_save()
    {
        BinaryFormatter bf = new BinaryFormatter();
        FileStream file = File.Create(System.Environment.GetFolderPath(System.Environment.SpecialFolder.MyDocuments) + "/My Games/KUS_GAMES/MAP/MapInfo.data");

        Navi_data_list = new List<NAVIDATA>();

        int Navi_ln = Navi_property_list.Count;
        for (int i = 0; i < Navi_ln; i++)
        {
            NAVIDATA save_data = new NAVIDATA();
            save_data.ID = Navi_property_list[i].Id;
            save_data.tri_data = new TRIANGLEDATA();
            save_data.edge_data = new List<EDGEDATA>();
            save_data.Neighborhood_data = new List<IDnDIS>();

            TRIANGLEDATA Tri = new TRIANGLEDATA();
            Tri.Tri_state_1_x = Navi_property_list[i].Triangle[0].x;
            Tri.Tri_state_1_y = Navi_property_list[i].Triangle[0].y;
            Tri.Tri_state_1_z = Navi_property_list[i].Triangle[0].z;

            Tri.Tri_state_2_x = Navi_property_list[i].Triangle[1].x;
            Tri.Tri_state_2_y = Navi_property_list[i].Triangle[1].y;
            Tri.Tri_state_2_z = Navi_property_list[i].Triangle[1].z;

            Tri.Tri_state_3_x = Navi_property_list[i].Triangle[2].x;
            Tri.Tri_state_3_y = Navi_property_list[i].Triangle[2].y;
            Tri.Tri_state_3_z = Navi_property_list[i].Triangle[2].z;

            Tri.Tri_state_4_x = Navi_property_list[i].Triangle[3].x;
            Tri.Tri_state_4_y = Navi_property_list[i].Triangle[3].y;
            Tri.Tri_state_4_z = Navi_property_list[i].Triangle[3].z;

            Tri.Tri_state_5_x = Navi_property_list[i].Triangle[4].x;
            Tri.Tri_state_5_y = Navi_property_list[i].Triangle[4].y;
            Tri.Tri_state_5_z = Navi_property_list[i].Triangle[4].z;

            save_data.tri_data = Tri;
            save_data.Neighborhood_data = Navi_property_list[i].near_triange;
            save_data.block = Navi_property_list[i].block;
            Navi_data_list.Add(save_data);
        }
        bf.Serialize(file, Navi_data_list);
        file.Close();
        Debug.Log("SAVE NAVI MESH FILE");
    }
    public void Point_data_save()
    {
        BinaryFormatter bf = new BinaryFormatter();
        FileStream file = File.Create(System.Environment.GetFolderPath(System.Environment.SpecialFolder.MyDocuments) + "/My Games/KUS_GAMES/MAP/Point.data");
        float[] Point_save_data = new float[Point_list.Count * 3];
        for (int i = 0; i < Point_list.Count; i++)
        {
            Point_save_data[i * 3] = Point_list[i].x;
            Point_save_data[i * 3 + 1] = Point_list[i].y;
            Point_save_data[i * 3 + 2] = Point_list[i].z;
        }

        bf.Serialize(file, Point_save_data);
        file.Close();
        Debug.Log("SAVE NAVI Tile FILE");
    }
    //------------LOAD
    public void File_Load()
    {
        Navimesh_data_load();
        Point_data_load();
    }
    public void Navimesh_data_load()
    {
        if (File.Exists(System.Environment.GetFolderPath(System.Environment.SpecialFolder.MyDocuments) + "/My Games/KUS_GAMES/MAP/MapInfo.data"))
        {
            BinaryFormatter bf = new BinaryFormatter();
            FileStream file = File.Open(
                System.Environment.GetFolderPath(System.Environment.SpecialFolder.MyDocuments) + "/My Games/KUS_GAMES/MAP/MapInfo.data",
                FileMode.Open);

            List<NAVIDATA> load_data = (List<NAVIDATA>)bf.Deserialize(file);
            file.Close();

            int Nd_ln = load_data.Count;
            Navi_property_list = new List<NAVI_Property>();

            for (int i = 0; i < Nd_ln; i++)
            {
                NAVI_Property Navi_pro = new NAVI_Property();

                Navi_pro.Id = load_data[i].ID;

                Vector3 Triangle_p1 = new Vector3(
                load_data[i].tri_data.Tri_state_1_x,
                load_data[i].tri_data.Tri_state_1_y,
                load_data[i].tri_data.Tri_state_1_z);
                Vector3 Triangle_p2 = new Vector3(
                load_data[i].tri_data.Tri_state_2_x,
                load_data[i].tri_data.Tri_state_2_y,
                load_data[i].tri_data.Tri_state_2_z);
                Vector3 Triangle_p3 = new Vector3(
                load_data[i].tri_data.Tri_state_3_x,
                load_data[i].tri_data.Tri_state_3_y,
                load_data[i].tri_data.Tri_state_3_z);
                Vector3 Triangle_p4 = new Vector3(
                load_data[i].tri_data.Tri_state_4_x,
                load_data[i].tri_data.Tri_state_4_y,
                load_data[i].tri_data.Tri_state_4_z);
                Vector3 Triangle_p5 = new Vector3(
                load_data[i].tri_data.Tri_state_5_x,
                load_data[i].tri_data.Tri_state_5_y,
                load_data[i].tri_data.Tri_state_5_z);

                Navi_pro.Triangle = new Vector3[5];
                Navi_pro.Triangle[0] = Triangle_p1;
                Navi_pro.Triangle[1] = Triangle_p2;
                Navi_pro.Triangle[2] = Triangle_p3;
                Navi_pro.Triangle[3] = Triangle_p4;
                Navi_pro.Triangle[4] = Triangle_p5;

                Navi_pro.near_triange = new List<IDnDIS>();
                Navi_pro.near_triange = load_data[i].Neighborhood_data;

                Navi_pro.block = load_data[i].block;

                Navi_property_list.Add(Navi_pro);
            }

            Debug.Log("LOAD NaviMesh");
        }
    }
    public void Point_data_load()
    {
        Point_list = new List<Vector3>();
        if (File.Exists(System.Environment.GetFolderPath(System.Environment.SpecialFolder.MyDocuments) + "/My Games/KUS_GAMES/MAP/Point.data"))
        {
            BinaryFormatter bf = new BinaryFormatter();
            FileStream file = File.Open(
                System.Environment.GetFolderPath(System.Environment.SpecialFolder.MyDocuments) + "/My Games/KUS_GAMES/MAP/Point.data",
                FileMode.Open);

            float[] load_data = (float[])bf.Deserialize(file);
            file.Close();
            int length = load_data.Length / 3;
            for (int i = 0; i < length; i++)
            {
                Vector3 vec_pos = new Vector3(load_data[i * 3], load_data[i * 3 + 1], load_data[i * 3 + 2]);
                Point_list.Add(vec_pos);
            }
            Debug.Log("LOAD TILE");
        }
    }
    //Voronoi diagram
    public List<Vector3> Voronoi_centerPoint_list;
    //GameObject
    public GameObject[] Navi_obj_list;
    public String obj_pos;
    void Awake()
    {
        File_Load();
        Navi_Integrity_Check();
        A_node_point_clear();
        MESH_Delanuary_triangulation(Navi_property_list);        
    }
    
    //FIRST
    public void FIRST()
    {
        PointMapReset();
        Make_pointerMap();
        Navi_property_list = new List<NAVI_Property>();
        List<NAVI_Property> new_Navi_list = Delaunay_triangulation(BASE_point, 3200);
        Navi_property_list.AddRange(new_Navi_list);
        Navi_Property_GiveID();
        Navi_Property_neighbor_searching();
        Navi_Integrity_Check();
        MESH_Delanuary_triangulation(Navi_property_list);
    }
    //NAVI MESH CUTTING TOOL [Dynamics]
    public void Dynamic_checker(GameObject current_Object,float space)
    {
        String new_obj_pos =  Dynamic_object();
        if(obj_pos != new_obj_pos)
        {
            obj_pos = new_obj_pos;
            Navi_Mesh_Editing(current_Object, space);
        }
        else if(obj_pos == null)
        {
            obj_pos = Dynamic_object();
        }
    }
    public String Dynamic_object()
    {
        String new_obj_pos = "";
        if(Navi_obj_list != null)
        {
            for (int i = 0; i < Navi_obj_list.Length; i++)
                new_obj_pos = new_obj_pos + (Navi_obj_list[i].transform.position.ToString());
        }
        return new_obj_pos;
    }
    public void Navi_Mesh_Editing(GameObject current_Object, float space)
    {
        Navi_obj_list = GameObject.FindGameObjectsWithTag("NAVI_BLOCK");
        List<Vector3> selectPoint = new List<Vector3>();
        selectPoint.AddRange(BASE_point);
        List<List<Vector3>> list_object_edge = new List<List<Vector3>>();
        int NavI_l = Navi_obj_list.Length;
        for (int j = 0; j < NavI_l; j++)
        {
            GameObject current_obj = Navi_obj_list[j];
            if (current_Object != current_obj)
            {
                List<Vector3> object_edge = Navi_object_Editing(current_obj, space);
                selectPoint.AddRange(object_edge);
                object_edge.Add(object_edge[0]);
                list_object_edge.Add(object_edge);
            }
            
        }
        Point_list = selectPoint;
        //----------------------------------
        Navi_property_list = new List<NAVI_Property>();
        Navi_property_list = Delaunay_triangulation(Point_list, 1600);
        //----BLOCK TRIANGLE--------
        List<NAVI_Property> block_list = new List<NAVI_Property>();
        for (int j = 0; j < list_object_edge.Count; j++)
        {
            for (int i = 0; i < Navi_property_list.Count; i++)
            {
                Vector3 tri = Navi_property_list[i].Triangle[3];
                int incheck = Point_inside_convexhull(list_object_edge[j], tri);
                if (incheck == 1)
                {
                    block_list.Add(Navi_property_list[i]);
                }
            }
        }
        block_edge_list = new List<Vector3[]>();
        for (int i = 0; i < block_list.Count; i++)
        {
            Vector3[] new_edge1 = { block_list[i].Triangle[0], block_list[i].Triangle[1] };
            Vector3[] new_edge2 = { block_list[i].Triangle[1], block_list[i].Triangle[2] };
            Vector3[] new_edge3 = { block_list[i].Triangle[2], block_list[i].Triangle[0] };
            block_edge_list.Add(new_edge1);
            block_edge_list.Add(new_edge2);
            block_edge_list.Add(new_edge3);
            block_list[i].block = true;
        }
        //----------------------------------
        Navi_Property_GiveID();
        Navi_Property_neighbor_searching();
        Navi_Integrity_Check();
        MESH_Delanuary_triangulation(Navi_property_list);
    }
    public List<Vector3> Navi_object_Editing(GameObject current_obj, float space)
    {
        //--------------Object_point_list----------------
        List<Vector3> object_pos_list = new List<Vector3>();
        Vector3[] v_mesh = current_obj.GetComponent<MeshFilter>().mesh.vertices;
        int V_mesh_length = v_mesh.Length;
        for (int i = 0; i < V_mesh_length; i++)
        {
            Vector3 new_point = v_mesh[i];
            Vector3 World_pos = GetVertexWorldPosition(new_point * space, current_obj.transform);
            Vector3 World_edit_point = new Vector3(World_pos.x, 0, World_pos.z);
            if (!object_pos_list.Contains(World_edit_point))
            {
                object_pos_list.Add(World_edit_point);                
            }           
        }
        //------convex hull edge list--------
        List<Vector3> convex_hull = ConvexHull(object_pos_list);
        //------------------------------------
        return convex_hull;
    }
    public Vector3 GetVertexWorldPosition(Vector3 vertex, Transform owner)
    {
        return owner.localToWorldMatrix.MultiplyPoint3x4(vertex);
    }      
    public List<Vector3> ConvexHull(List<Vector3> c_point_list)
    {        
        List<Vector3> convex_hull_list = new List<Vector3>();

        c_point_list.Sort(new ZCompare());//x-->z-->y-
        Vector3 p1 = c_point_list[0];

        x_coordiate_angle Anglesort = new x_coordiate_angle();//seta> 
        Anglesort.co_pos = c_point_list[0];
        c_point_list.Sort(Anglesort);
        c_point_list.Reverse();

        int length = c_point_list.Count;
        int n = 0;
        while (n < length)
        {
            Vector3 p2 = c_point_list[n];

            bool comp = true;
            int length2 = c_point_list.Count;
            for (int i = 0; i < length2; i++)
            {
                Vector3 current_point = c_point_list[i];
                if (current_point != p1 && current_point != p2)
                {
                    Vector3 p3 = current_point;
                    int inside = CrossProduct(p1, p2, p3);
                    if (inside == -1)
                    {
                        comp = false;
                        break;
                    }
                }
            }
            if (comp == true)
            {
                convex_hull_list.Add(p1);
                p1 = p2;
            }
            n++;
        }
        return convex_hull_list;
    }    
    public int Point_inside_convexhull(List<Vector3> convex_edge_list, Vector3 p3) //-1 outside 1 inside 
    {
        int length = convex_edge_list.Count-1;//need sort for edge serialize and last point must be first point;
        int return_value = 1;
        for (int i = 0; i < length ; i++)
        {
            Vector3 p1 = convex_edge_list[i];
            Vector3 p2 = convex_edge_list[i + 1];
            if (CrossProduct(p1, p2, p3) == -1)
            {
                return_value = -1;
                break;
            }
        }
        return return_value;
    }    
    //MAKE POINT <TILE>MAP [BASE]
    public void Make_pointerMap()
    {
        PointMapReset();
        BASE_point = new List<Vector3>();
        for (int j = 0; j < point_count_z; j++)
        {
            for (int i = 0; i < point_count_x; i++)
            {
                Vector3 point = new Vector3(start_Coordinates.x + i * point_interval,
                    start_Coordinates.y,
                    start_Coordinates.z + j * point_interval);

                BASE_point.Add(point);
            }
        }
        Debug.Log("Finish Making Map : " + BASE_point.Count);
    }
    public void PointMapReset()
    {
        BASE_point = new List<Vector3>();
        Point_list = new List<Vector3>();
    }
    //A* ALGORITHM FOR DELAUNAY_TRIANGLE MESH [Dynamics]
    public void A_node_point_start(Vector3 start, Vector3 target)
    {
        //start and target reverse
        target_node = IncludeTri(target);
        start_node = IncludeTri(start);
        if (target_node != null && start_node != null)
        {
            A_node_point_clear();           
            open_tri_l.Add(start_node);
            close_tri_l.Add(start_node);
            A_node_point_algorithm(start, target);
        }
    }
    public void A_node_point_clear()
    {
        int length = Navi_property_list.Count;
        for (int j = 0; j < length; j++)
        {
            Navi_property_list[j].F = 0;
            Navi_property_list[j].G = 0;
            Navi_property_list[j].H = 0;
            Navi_property_list[j].parent_id = -1;
        }
        open_tri_l = new List<NAVI_Property>();
        close_tri_l = new List<NAVI_Property>();
        Debug.Log("value clear");
    }
    public void A_node_point_algorithm(Vector3 start, Vector3 target)
    {

        NAVI_Property s_tri = start_node;
        while (!open_tri_l.Contains(target_node))
        {
            //near checker Add G value 
            //Debug.Log("<" + s_tri.Triangle[3] + ">");
            int length;
            if (s_tri.near_triange != null)
            {
                length = s_tri.near_triange.Count;
            } 
            else
            {
                break;
            }
            for (int j = 0; j < length; j++)
            {                
                int current_id = s_tri.near_triange[j].ID;
                NAVI_Property near_tri = Navi_property_list[current_id];

                if (!open_tri_l.Contains(near_tri) && !close_tri_l.Contains(near_tri))
                {
                    if (near_tri.parent_id != -1)
                    {
                        float H = Mathf.Sqrt(
                          (target.x - near_tri.Triangle[3].x) * (target.x - near_tri.Triangle[3].x)
                          + (target.y - near_tri.Triangle[3].y) * (target.y - near_tri.Triangle[3].y)
                          + (target.z - near_tri.Triangle[3].z) * (target.z - near_tri.Triangle[3].z)
                          );
                        float G
                            = Navi_property_list[near_tri.parent_id].G
                            + s_tri.near_triange[j].dis;
                        near_tri.G = G;
                        near_tri.H = H;
                        near_tri.F = H + G;
                    }
                    else
                    {
                        float H = Mathf.Sqrt(
                          (target.x - near_tri.Triangle[3].x) * (target.x - near_tri.Triangle[3].x)
                          + (target.y - near_tri.Triangle[3].y) * (target.y - near_tri.Triangle[3].y)
                          + (target.z - near_tri.Triangle[3].z) * (target.z - near_tri.Triangle[3].z)
                          );
                        float G
                            = s_tri.near_triange[j].dis;
                        near_tri.G = G;
                        near_tri.H = H;
                        near_tri.F = H + G;
                        near_tri.parent_id = s_tri.Id;
                    }
                    open_tri_l.Add(near_tri);
                }
            }
            if (open_tri_l.Contains(target_node))
                break;
            //Open List Check
            float F = 99999;
            NAVI_Property select_tri_neigh = new NAVI_Property();
            for (int k = 0; k < open_tri_l.Count; k++)
            {
                NAVI_Property compare = open_tri_l[k];
                if (!close_tri_l.Contains(compare))
                {                   
                    if (compare.F <= F)
                    {
                        F = compare.F;
                        select_tri_neigh = compare;
                    }
                }
            }
            //select tri    
            if (select_tri_neigh != null)
            {
                close_tri_l.Add(select_tri_neigh);
                s_tri = select_tri_neigh;
            }
            else
            {
                break;
            }
        }
        Debug.Log("FINISH");
        if(open_tri_l.Contains(target_node))
            A_node_poin_algorithm_show_way(start,target);
    }
    public void A_node_poin_algorithm_show_way(Vector3 start, Vector3 target)//funnel
    {
        mv_Tri = new List<Vector3>();
        List<Vector3> mv = new List<Vector3>();
        mv_funnel = new List<Vector3>();
        Line_poin = new List<Vector3>();
        NAVI_Property current = target_node;

        mv_Tri.Add(target);
        while (current != start_node)
        {   
            Vector3 tp;
            float d1, d2, d; ;

            for(int i=0; i < current.near_triange.Count; i++)
            {
                if(current.near_triange[i].ID == Navi_property_list[current.parent_id].Id)
                {
                    Vector3 ep1 = new Vector3(current.near_triange[i].e1_x, current.near_triange[i].e1_y, current.near_triange[i].e1_z);
                    Vector3 ep2 = new Vector3(current.near_triange[i].e2_x, current.near_triange[i].e2_y, current.near_triange[i].e2_z);
                    Line_poin.Add(ep1);
                    Line_poin.Add(ep2);
                    
                    d1 = vector_dis(target, ep1);
                    d2 = vector_dis(target, ep2);
                    d = Mathf.Min(d1, d2);
                    
                    if (d == d1)
                        tp = ep1;
                    else 
                        tp = ep2;
                    mv.Add(tp);
                    break;
                }
            }

            mv_Tri.Add(current.Triangle[3]);
            current = Navi_property_list[current.parent_id];
        }

        Vector3 funnel_origin = new Vector3(target.x, 0, target.z);
        for (int i =0; i< mv.Count; i++)
        {
            Vector3[] Edge = { funnel_origin, mv[i] };
            for (int j = 0; j < block_edge_list.Count; j++)
            {
                int Mp = EdgeCrossAndMAkePoint(block_edge_list[j], Edge);
                if (Mp == 1)
                {
                    mv_funnel.Add(funnel_origin);
                    mv_funnel.Add(mv[i - 1]);
                    funnel_origin = mv[i - 1];
                    break;
                }
            }
        }
        mv_funnel.Add(funnel_origin);
        mv_funnel.Add(start);
        Debug.Log("FINISH SHOWING");
    }       
    //NAVIMESH
    public void Navi_Property_GiveID()
    {
        ///Give number
        int length = Navi_property_list.Count;
        for (int d = 0; d < length; d++)
        {
            Navi_property_list[d].Id = d;
        }
    }
    public void Navi_Property_neighbor_searching()//EDIT REQUIRE
    {
        ///Checking same edge
        int length = Navi_property_list.Count;        
        for (int i =0; i< length; i++)
        {
            NAVI_Property select_navi_state = Navi_property_list[i];
            if (select_navi_state.block == false)
            {
                //Debug.Log(select_navi_state.Triangle[3]);
                select_navi_state.near_triange = new List<IDnDIS>();
                for (int j = 0; j < length; j++)
                {
                    NAVI_Property compare_navi_state = Navi_property_list[j];
                    if(compare_navi_state.block == false && compare_navi_state != select_navi_state)
                    {
                        Vector3[] sameEdge = new Vector3[2];
                        int samePoint = 0;
                        for (int k = 0; k < 3; k++)
                        {
                            if (select_navi_state.Triangle[k] == compare_navi_state.Triangle[0]
                                || select_navi_state.Triangle[k] == compare_navi_state.Triangle[1]
                                || select_navi_state.Triangle[k] == compare_navi_state.Triangle[2])
                            {
                                samePoint++;
                                if (samePoint <= 2)
                                {
                                    sameEdge[samePoint - 1] = select_navi_state.Triangle[k];
                                }
                            }
                        }
                        if (samePoint == 2)
                        {
                            IDnDIS near_tri_inf = new IDnDIS();
                            //---------ID---------
                            near_tri_inf.ID = Navi_property_list[j].Id;
                            Vector3 select_dis_compare = select_navi_state.Triangle[3] - compare_navi_state.Triangle[3];
                            //---------DIS--------
                            float dis = Mathf.Sqrt(
                                select_dis_compare.x * select_dis_compare.x
                                + select_dis_compare.y * select_dis_compare.y
                                + select_dis_compare.z * select_dis_compare.z);
                            near_tri_inf.dis = dis;
                            //--------EDGE--------
                            near_tri_inf.e1_x = sameEdge[0].x;
                            near_tri_inf.e1_y = sameEdge[0].y;
                            near_tri_inf.e1_z = sameEdge[0].z;

                            near_tri_inf.e2_x = sameEdge[1].x;
                            near_tri_inf.e2_y = sameEdge[1].y;
                            near_tri_inf.e2_z = sameEdge[1].z;
                            //---------ADD--------
                            select_navi_state.near_triange.Add(near_tri_inf);                           
                        }
                    }
                }
            }
        }
    }
    public void Navi_Integrity_Check()
    {
        if (Navi_property_list != null)
        {
            int length = Navi_property_list.Count;
            for (int i = 0; i < length; i++)
            {
                if(Navi_property_list[i].Id != i)
                {
                    Debug.LogWarning("FAIL : [" + i + "] ID is different");
                }
            }
        }
        else
        {
            Debug.Log("Navi property list is empty");
        }
    }
    public NAVI_Property IncludeTri(Vector3 c_pos)
    {
        if (Navi_property_list != null)
        {
            NAVI_Property select_trianlge = new NAVI_Property();
            //what is the triangle include this point;
            int range = Navi_property_list.Count;
            for (int i = 0; i < range; i++)
            {

                int a = CrossProduct(Navi_property_list[i].Triangle[0], Navi_property_list[i].Triangle[1], c_pos);
                int b = CrossProduct(Navi_property_list[i].Triangle[1], Navi_property_list[i].Triangle[2], c_pos);
                int c = CrossProduct(Navi_property_list[i].Triangle[2], Navi_property_list[i].Triangle[0], c_pos);
                int d = a + b + c;//1+1+1 =3,1+1+0 = 2, 1+1+(-1) = 1, 
                if (d >= 2)
                {
                    select_trianlge = Navi_property_list[i];
                    break;
                }
            }
            if (select_trianlge != null && select_trianlge.block == false)
            {
                return select_trianlge;
            }
            else
            {
                Debug.Log("COULDN'T FIND TRIANGLE");
                return null;
            }
        }
        else
        {
            Debug.LogWarning("Navi_Property_list is null");
            return null;
        }
    }
    public float vector_dis(Vector3 A, Vector3 B)
    {
        return Mathf.Sqrt( (A.x - B.x) * (A.x - B.x) + (A.z - B.z) * (A.z - B.z));
    }
    //MAKING MESH BY DELAUNAY_TRIANGLE ONLY FOR THE VECTOR2 X,Z      
    public List<NAVI_Property> Delaunay_triangulation(List<Vector3> point_list,int Maximum)
    {
        List<Vector3[]> OpenEdgeList = new List<Vector3[]>();
        List<String> CloseEdgeList = new List<String>();
        List<NAVI_Property> new_Navi_List = new List<NAVI_Property>();
        List<Vector3> Triangle_center_list = new List<Vector3>();

        point_list.Sort(new ZCompare());//x-->z-->y-
        Vector3 p0 = point_list[0];

        x_coordiate_angle Anglesort = new x_coordiate_angle();//seta> 
        Anglesort.co_pos = point_list[0];
        point_list.Sort(Anglesort);

        Vector3 pointselect = point_list[1];
        Vector3[] Edge_r = { pointselect, p0 }; //p2->p3 Vector

        OpenEdgeList.Add(Edge_r);

        int Maximum_counter = 0; // for the Maximum
        while (Maximum_counter < Maximum)
        {
            Maximum_counter++;
            List<Vector3> CCW_list = new List<Vector3>();
            Vector3[] selectEdge = new Vector3[2];

            if (OpenEdgeList.Count == 0)
                break;
            else
            {               
                selectEdge = OpenEdgeList[0];
                OpenEdgeList.RemoveAt(0);
            }
            String stringEdge = NodeToString(selectEdge[0], selectEdge[1]);
            bool select = true;
            if (CloseEdgeList.Count > 0 && CloseEdgeList.Contains(stringEdge))//check it was already in Close List;
                    select = false;
            if (select == true)
            {
                int point_list_length = point_list.Count;
                for (int i = 0; i < point_list_length; i++)
                {
                    if (point_list[i] != selectEdge[0] && point_list[i] != selectEdge[1])
                    {
                        int result_cross = CrossProduct(selectEdge[0], selectEdge[1], point_list[i]);
                        if (result_cross == 1)
                        {
                            CCW_list.Add(point_list[i]);
                        }
                    }
                }

                int ccw_length = CCW_list.Count;
                if (ccw_length > 0)
                {
                    for (int j = 0; j < ccw_length; j++)
                    {
                       
                        Vector3 selectPoint = CCW_list[j];
                        float[] circle = circumscribed_circle(selectEdge[0], selectEdge[1], selectPoint);
                        Vector3 cir_center = new Vector3(circle[0], 0, circle[1]);

                        bool include = false;
                        for (int k = 0; k < ccw_length; k++)
                        {
                            if (CCW_list[k] != selectEdge[0] && CCW_list[k] != selectEdge[1] && CCW_list[k] != selectPoint)
                            {
                                int result_include = isitincircle(cir_center, circle[2], CCW_list[k]);
                                if (result_include == 1)
                                {
                                    include = true;
                                    break;
                                }
                            }
                        }
                        Vector3 Tri_center = (selectEdge[0]+ selectEdge[1] + selectPoint) / 3;
                        if (include == false && !Triangle_center_list.Contains(Tri_center))
                        {
                            Triangle_center_list.Add(Tri_center);
                            Vector3[] Edge1 = { selectEdge[1], selectPoint };//p2 -> p3
                            Vector3[] Edge2 = { selectPoint, selectEdge[0] };//p3 -> p1

                            Vector3[] Edge2_r = { selectEdge[0], selectPoint };//p1 -> p3
                            Vector3[] Edge1_r = { selectPoint, selectEdge[1] };//p3 -> p2
                            Vector3[] Edge0_r = { selectEdge[1], selectEdge[0] };//p2 -> p1
                            

                            String selectEdge_s = NodeToString(selectEdge[0], selectEdge[1]);
                            String Edge1_s = NodeToString(Edge1[0], Edge1[1]);
                            String Edge2_s = NodeToString(Edge2[0], Edge2[1]);

                            String Edge1_r_s = NodeToString(Edge1_r[0], Edge1_r[1]);
                            String Edge2_r_s = NodeToString(Edge2_r[0], Edge2_r[1]);
                            String Edge0_r_s = NodeToString(Edge0_r[0], Edge0_r[1]);
                            //Debug.Log("MAKE!!")
                            //---list--
                            CloseEdgeList.Add(selectEdge_s);
                            CloseEdgeList.Add(Edge1_s);
                            CloseEdgeList.Add(Edge2_s);
                            //---------OPen list obligation
                            if (!CloseEdgeList.Contains(Edge1_r_s))
                            {
                                OpenEdgeList.Add(Edge1_r);
                            }
                            if (!CloseEdgeList.Contains(Edge2_r_s))
                            {
                                OpenEdgeList.Add(Edge2_r);
                            }
                            if (!CloseEdgeList.Contains(Edge0_r_s))
                            {
                                OpenEdgeList.Add(Edge0_r);
                            }
                            ///------------Triangle_State---------
                            Vector3[] Triangle_State = new Vector3[5];
                            Triangle_State[0] = selectEdge[0];
                            Triangle_State[1] = selectEdge[1];
                            Triangle_State[2] = selectPoint;
                            Triangle_State[3] = Tri_center;
                            Triangle_State[4] = cir_center;
                            //----FOR THE A* algoithm-------
                            NAVI_Property new_triangle = new NAVI_Property();
                            new_triangle.Triangle = Triangle_State;
                            new_triangle.Id = -1;
                            new_triangle.block = false;
                            new_triangle.F = -1;
                            new_triangle.G = -1;
                            new_triangle.H = -1;
                            new_triangle.parent_id = -1;
                            new_Navi_List.Add(new_triangle);
                            break;
                        }
                    }
                }
            }
        }
        return new_Navi_List;
    }
    //Delaunay_mesh creating  
    public void MeshRESET()
    {
        if (gameObject.GetComponent<MeshFilter>() == null)
            gameObject.AddComponent<MeshFilter>();

        if (gameObject.GetComponent<MeshRenderer>() == null)
            gameObject.AddComponent<MeshRenderer>();
        if (mesh == null)
            mesh = new Mesh();
        gameObject.GetComponent<MeshFilter>().mesh = mesh;
        gameObject.GetComponent<MeshRenderer>().material = Mapmat;
    }//MESH FIRST MAKEING METHOD
    public void MehsClear()
    {
        mesh.Clear();
    }
    public void MESH_Delanuary_triangulation(List<NAVI_Property> triangle_list)
    {
        MeshRESET();
        mesh.Clear();
        mesh.name = "WOO XZ MEHS";
        List<Vector3> _Vertax = new List<Vector3>();
        List<int> _Tri = new List<int>();
        
        
        int number = triangle_list.Count;
        for (int i = 0; i < number; i++)
        {
            Vector3[] Triangle = triangle_list[i].Triangle;
            if (CrossProduct(Triangle[0], Triangle[1], Triangle[2]) == -1)
            {
                _Vertax.Add(Triangle[0]);
                _Vertax.Add(Triangle[1]);
                _Vertax.Add(Triangle[2]);
                _Tri.Add(2 + 3 * i);
                _Tri.Add(1 + 3 * i);
                _Tri.Add(0 + 3 * i);
            }
            else if (CrossProduct(Triangle[0], Triangle[1], Triangle[2]) == 1)
            {
                _Vertax.Add(Triangle[0]);
                _Vertax.Add(Triangle[1]);
                _Vertax.Add(Triangle[2]);
                _Tri.Add(0 + 3 * i);
                _Tri.Add(1 + 3 * i);
                _Tri.Add(2 + 3 * i);
            }
            else
            {
                _Vertax.Add(Triangle[0]);
                _Vertax.Add(Triangle[1]);
                _Vertax.Add(Triangle[2]);
                _Tri.Add(0 + 3 * i);
                _Tri.Add(2 + 3 * i);
                _Tri.Add(1 + 3 * i);
            }
            //Debug.Log(Navi_property_list[i].Triangle[0] + "," + Navi_property_list[i].Triangle[1] + "," + Navi_property_list[i].Triangle[2] + " : " + Navi_property_list[i].Triangle[3]);
        }
        mesh.vertices = _Vertax.ToArray();
        mesh.triangles = _Tri.ToArray();
        mesh.RecalculateNormals();
        mesh.RecalculateBounds();
        mesh.Optimize();
    }
    //ABOUT EDGE
    public Vector3[] Edge_editor(Vector3 p1, Vector3 p2)
    {
        Vector3 arrow = p2 - p1;//(1,0,1)-(0,0,0)//(2,0,2)-(1,0,1)//(0,0,0)-(1,0,1)
        Vector3[] v_Edge = new Vector3[2];
        v_Edge[0] = p1;//(0,0,0) // (1,0,1) // (0,0,0)
        v_Edge[1] = arrow;//(1,0,1) //(1,0,1) // (-1,0,-1)
        return v_Edge;
    }
    public int EdgeCrossAndMAkePoint(Vector3[] ve1, Vector3[] ve2)//1 crosspoint O -1 crosspoint X 0 Same;
    {
        Vector3 A = ve1[0];
        Vector3 B = ve1[1];
        Vector3 C = ve2[0];
        Vector3 D = ve2[1];
        /* 
         * p(t) = (1-t)A + tB 0<=t<=1
         * p(t) = (1-s)C + tD 0<=s<=1
         * A.x + t*(B.x-A.x) = C.x + s*(D.x-C.x);
         * A.y + t*(B.y-A.y) = C.y + s*(D.y-C.y);
         * t = ((D.x-C.x)*(A.y-C.y) - (D.y - C.y)*(A.x-C.x)) / ((D.y - C.y)*(B.x -A.x)-(D.x-C.x)*(B.y -A.y));
         * s = ((B.x-A.x)*(A.y-C.y) - (B.y - A.y)*(A.x-C.x)) / ((D.y - C.y)*(B.x -A.x)-(D.x-C.x)*(B.y -A.y));
         * x = A.x + s(B.x - A.x);
         * y = A.y + s(B.y - A.y);
         */
        double under = (D.z - C.z) * (B.x - A.x) - (D.x - C.x) * (B.z - A.z);
        if (under == 0)
        {
            return 0;
        }
        else
        {
            double t = ((D.x - C.x) * (A.z - C.z) - (D.z - C.z) * (A.x - C.x)) / under;
            double S = ((B.x - A.x) * (A.z - C.z) - (B.z - A.z) * (A.x - C.x)) / under;
            if (t <= 0 || t >= 1 || S <= 0 || S >= 1)
                return -1;
            else
            {
                return 1;
            }
        }
    }
    public int ForEdge_isthisPointOnLine(Vector3[] ve1, Vector3[] ve2, float cx, float cz)//return 1 include iin line return -1 NO
    {
        float ve1_high_x = Mathf.Max(ve1[0].x, ve1[1].x);
        float ve1_low_x = Mathf.Min(ve1[0].x, ve1[1].x);

        float ve2_high_x = Mathf.Max(ve2[0].x, ve2[1].x);
        float ve2_low_x = Mathf.Min(ve2[0].x, ve2[1].x);

        float ve1_high_z = Mathf.Max(ve1[0].z, ve1[1].z);
        float ve1_low_z = Mathf.Min(ve1[0].z, ve1[1].z);

        float ve2_high_z = Mathf.Max(ve2[0].z, ve2[1].z);
        float ve2_low_z = Mathf.Min(ve2[0].z, ve2[1].z);

        if (ve1_low_x < cx && cx < ve1_high_x
         && ve1_low_z < cz && cz < ve1_high_z)
        {
            if (ve2_low_x < cx && cx < ve2_high_x
             && ve2_low_z < cz && cz < ve2_high_z)
            {
                return 1;
            }
            else
                return -1;
        }
        else
        {
            return -1;
        }
    }
    //ABOUT CIRCLE AND TRIANGLE
    public float[] circumscribed_circle(Vector3 p1, Vector3 p2, Vector3 p3)//p1.x > p2.x, p3.x > p2.x
    {
        Vector3 CA = p1 - p3;
        Vector3 CB = p2 - p3;

        float CAXCAZ = CA.x * CA.x + CA.z * CA.z;
        float CBXCBZ = CB.x * CB.x + CB.z * CB.z;
        float CAXCBZCAZCBX = 2 * (CA.x * CB.z - CA.z * CB.x);//cross product
        if (CAXCBZCAZCBX != 0)
        {
            float c_x = p3.x + (CB.z * CAXCAZ - CA.z * CBXCBZ) / CAXCBZCAZCBX;
            float c_z = p3.z + (CA.x * CBXCBZ - CB.x * CAXCAZ) / CAXCBZCAZCBX;
            float[] circle = new float[3];
            circle[0] = c_x;
            circle[1] = c_z;
            circle[2] =Mathf.Sqrt((p3 .x- c_x) * (p3 .x- c_x) + (p3.z - c_z) * (p3.z - c_z));
            return circle;
        }
        else
        {
            return null;
        }
         
    }
    public int isitincircle(Vector3 circle_center,float circle_r, Vector3 p)//-1 No / 1 has / 0 same circle
    {
        Vector3 cir_p = (p - circle_center);
        float cir_p_r = Mathf.Sqrt((cir_p.x * cir_p.x) + (cir_p.z * cir_p.z));
        if (cir_p_r < circle_r)
        {
            return 1;
        }
        else if(cir_p_r == circle_r)
        {
            return 0;
        }
        else
            return -1;
    }
    //Vector Inner product and cross product
    public Vector3 Vector3_Inner_product(Vector3 p1, Vector3 p2)
    {
        Vector3 innervec = new Vector3(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);
        return innervec;
    }
    public Vector3 Vector3_Cross_product(Vector3 p1, Vector3 p2)
    {
        Vector3 outvec = new Vector3(p1.y * p2.z - p1.z * p2.y, p1.z * p2.x - p1.x * p2.z, p1.x * p2.y - p1.y * p2.x);
        return outvec;
    }
    //ABOUT POINT AND FACE
    public int CrossProduct(Vector3 p1, Vector3 p2, Vector3 p3)//Reverse clock -1  clock 1 nothing 0
    {
        Vector3 A = p2 - p1;
        Vector3 B = p3 - p2;
        //Cross product
        Vector3 Cross
            = new Vector3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
        if (Cross.y > 0)//left x up z vertical y
            return 1; // Clock
        else if (Cross.y < 0)
            return -1; // reverse Clock
        else //(Cross.z == 0)
            return 0; // on the line
    }

    public String NodeToString(Vector3 p1, Vector3 p2)
    {
        String Edge =
                      p1.x.ToString() + ","
                    + p1.y.ToString() + ","
                    + p1.z.ToString() + "/"
                    + p2.x.ToString() + ","
                    + p2.y.ToString() + ","
                    + p2.z.ToString() ;
        return Edge;
    }
    public bool Complanate_Checker(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p)
    {
        float A = p1.y * (p2.z - p3.z) + p2.y * (p3.z - p1.z) + p3.y*(p1.z - p2.z);
        float B = p1.z * (p2.x - p3.x) + p2.z * (p3.x - p1.x) + p3.z * (p1.x - p2.x);
        float C = p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);
        float D = 
            - (
            p1.x * (p2.y * p3.z - p3.y * p2.z)
            + p2.x * (p3.y * p1.z - p1.y*p3.z)
            + p3.x * (p1.y * p2.z - p2.y * p1.z)
            );
        if (A * p.x + B * p.y + C * p.z + D == 0)
        {
            Debug.Log("complanate");
            return true;
        }
        else
            return false;
    }
    
}

public class Sort_for_Vertext : IComparer<Vector3>
{
    public Vector3 O;
    public int Compare(Vector3 L, Vector3 R)
    {
        return (Vector3.Distance(L, O)).CompareTo(Vector3.Distance(R, O));
    }
}

[System.Serializable]
public class TRIANGLEDATA 
{   //1
    public float Tri_state_1_x;
    public float Tri_state_1_y;
    public float Tri_state_1_z;
    //2
    public float Tri_state_2_x;
    public float Tri_state_2_y;
    public float Tri_state_2_z;
    //3
    public float Tri_state_3_x;
    public float Tri_state_3_y;
    public float Tri_state_3_z;
    //middle point
    public float Tri_state_4_x;
    public float Tri_state_4_y;
    public float Tri_state_4_z;
    //For voronoi
    public float Tri_state_5_x;
    public float Tri_state_5_y;
    public float Tri_state_5_z;
}
[System.Serializable]
public class EDGEDATA
{
    public float Edge1_x;
    public float Edge1_y;
    public float Edge1_z;

    public float Edge2_x;
    public float Edge2_y;
    public float Edge2_z;
}
[System.Serializable]
public class NAVIDATA
{
    public int ID;
    public TRIANGLEDATA tri_data;
    public List<EDGEDATA> edge_data;
    public List<IDnDIS> Neighborhood_data;
    public bool block;
}
[System.Serializable]
public class IDnDIS
{
    public int ID;
    public float dis;

    public float e1_x;
    public float e1_y;
    public float e1_z;

    public float e2_x;
    public float e2_y;
    public float e2_z;
}

//----------------
public class NAVI_Property //RESULT OF WHAT WE WANT TO FIND
{
    public int Id;
    public bool block;

    public Vector3[] Triangle; //Delaunay_triangulation [0] p1 [1] p2 [2] p3 [3] centerPoint
    public List<IDnDIS> near_triange;
    public float H;  //made in A*
    public float G; //made in A*
    public float F;             //made in A*
    public int parent_id;//made in A*
}
public class Sort_point_dis : IComparer<Vector3>
{
    public Vector3 tPos;
    public int Compare(Vector3 t1, Vector3 t2)
    {
        return (Vector3.Distance(t1, tPos)).CompareTo(Vector3.Distance(t2, tPos));
    }
}
public class Vector3Compare : IComparer<Vector3>
{
    public int Compare(Vector3 a, Vector3 b)
    {
        if (a.x < b.x)
        {
            return -1;
        }
        else if (a.x == b.x)
        {
            if (a.z < b.z)
            {
                return -1;
            }
            else if (a.z == b.z)
            {
                if (a.y < b.y)
                {
                    return -1;
                }
                else if (a.y == b.y)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
}
//----------------
public class ZCompare : IComparer<Vector3>
{
    public int Compare(Vector3 a, Vector3 b)
    {
        if (a.z < b.z)
        {
            return -1;
        }
        else if (a.z == b.z)
        {
            if (a.x < b.x)
            {
                return -1;
            }
            else if (a.x == b.x)
            {
                if (a.y < b.y)
                {
                    return -1;
                }
                else if (a.y == b.y)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
}
public class x_coordiate_angle : IComparer<Vector3>
{
    public Vector3 co_pos;
    public float GetAngle(Vector3 p1)
    {
        float seta = 0;
        if (p1 == co_pos)
        {
            seta = 0;
        }
        else
        {            
            Vector3 x = new Vector3(1, 0, 0);
            Vector3 p = p1 - co_pos;
            float Dot = Vector3.Dot(p, x);
            if (Dot != 0)
            {
                seta = Mathf.Acos(Dot / (Mathf.Sqrt(x.x * x.x + x.z * x.z) * Mathf.Sqrt(p.x * p.x + p.z * p.z)));
            }
            else
            {
                seta =1.57079633f;
            }
        }
        return seta;
    }
    public int Compare(Vector3 a, Vector3 b)
    {
        float A = GetAngle(a);
        float B = GetAngle(b);
        if(a == co_pos)
        {
            return -1;
        }
        else if (b == co_pos)
        {
            return -1;
        }
        else if (A > B)
        {
            return 1;
        }
        else if (A < B)
        {
            return -1;
        }
        else
        {
            float dis_A = Vector3.Distance(co_pos, a);
            float dis_B = Vector3.Distance(co_pos, b);
            if (dis_A > dis_B)
            {
                return -1;
            }
            else
            {
                return 1;
            }
        }
    }
}
