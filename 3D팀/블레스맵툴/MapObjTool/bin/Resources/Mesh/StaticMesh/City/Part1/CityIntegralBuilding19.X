xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template VertexElement {
 <f752461c-1e23-48f6-b9f8-8350850f336f>
 DWORD Type;
 DWORD Method;
 DWORD Usage;
 DWORD UsageIndex;
}

template DeclData {
 <bf22e553-292c-4781-9fea-62bd554bdd93>
 DWORD nElements;
 array VertexElement Elements[nElements];
 DWORD nDWords;
 array DWORD data[nDWords];
}


Material material_0 {
 0.588000;0.588000;0.588000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "LV_Hieracon_City_IntegralBuilding08_D_LBR.tga";
 }
}

Frame LV_Hieracon_City_IntegralBuilding13A_SM_LBR {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh LV_Hieracon_City_IntegralBuilding13A_SM_LBR {
  9;
  -38.000000;1.999985;-154.000000;,
  38.000000;1.999985;-154.000000;,
  -38.000000;1.999985;-255.656192;,
  38.000000;1.999985;-255.656192;,
  38.000000;1.555862;-256.100301;,
  -38.000000;1.555862;-256.100301;,
  -38.000000;1.999985;-255.656192;,
  38.000000;-8.000000;-256.100301;,
  -38.000006;-8.000000;-256.100301;;
  6;
  3;0,1,2;,
  3;1,3,2;,
  3;2,3,4;,
  3;4,5,6;,
  3;5,4,7;,
  3;7,8,5;;

  MeshMaterialList {
   1;
   6;
   0,
   0,
   0,
   0,
   0,
   0;
   { material_0 }
  }

  DeclData {
   5;
   2;0;3;0;,
   2;0;6;0;,
   2;0;7;0;,
   1;0;5;0;,
   1;0;5;1;;
   117;
   0,
   1065353216,
   0,
   1065353216,
   0,
   0,
   0,
   0,
   3212836864,
   991723520,
   3210944512,
   939524096,
   939524096,
   0,
   1065353216,
   0,
   1065353216,
   0,
   0,
   0,
   0,
   3212836864,
   1067671552,
   3210944512,
   1065328640,
   939524096,
   0,
   1065353101,
   3144856932,
   1065353216,
   0,
   0,
   0,
   3144856932,
   3212836749,
   991723520,
   1062289408,
   939524096,
   1063821312,
   0,
   1064076121,
   3200511790,
   1065353216,
   0,
   0,
   0,
   3200511790,
   3211559769,
   1067671552,
   1062289408,
   1065328640,
   1063821312,
   0,
   1053028089,
   3211559780,
   1065353216,
   0,
   0,
   0,
   3211559780,
   3200511738,
   1067671552,
   1062535168,
   1065328640,
   1063919616,
   0,
   1053028142,
   3211559769,
   1065353216,
   0,
   0,
   0,
   3211559769,
   3200511791,
   991723520,
   1062535168,
   939524096,
   1063919616,
   0,
   1060439283,
   3207922931,
   1065353216,
   0,
   0,
   0,
   3207922931,
   3207922931,
   991723520,
   1062289408,
   939524096,
   1063821312,
   0,
   0,
   3212836864,
   1065353216,
   0,
   0,
   0,
   3212836864,
   0,
   1067671552,
   1065222144,
   1065328640,
   1065345024,
   0,
   0,
   3212836864,
   1065353216,
   0,
   0,
   0,
   3212836864,
   0,
   991723520,
   1065222144,
   939524096,
   1065345024;
  }
 }
}