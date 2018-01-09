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


Material LV_Hieracon_Object_Props01_MI_NWCREX {
 0.588000;0.588000;0.588000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "LV_Hieracon_Object_Props01_D_NWCREX.tga";
 }
}

Frame LV_Hieracon_Object_Props06_SM_NWCREX {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh LV_Hieracon_Object_Props06_SM_NWCREX {
  8;
  -28.878784;0.084453;-0.000001;,
  28.878784;0.084445;46.487635;,
  28.878784;0.084453;-0.000001;,
  -28.878784;0.084445;46.487635;,
  -28.878784;0.084461;-46.487635;,
  28.878784;0.084453;-0.000001;,
  28.878784;0.084461;-46.487635;,
  -28.878784;0.084453;-0.000001;;
  4;
  3;0,1,2;,
  3;1,0,3;,
  3;4,5,6;,
  3;5,4,7;;

  MeshMaterialList {
   1;
   4;
   0,
   0,
   0,
   0;
   { LV_Hieracon_Object_Props01_MI_NWCREX }
  }

  DeclData {
   5;
   2;0;3;0;,
   2;0;6;0;,
   2;0;7;0;,
   1;0;5;0;,
   1;0;5;1;;
   104;
   0,
   1065353216,
   875575305,
   1065353216,
   0,
   0,
   2147483648,
   875575305,
   3212836864,
   1065377792,
   989044736,
   1023393792,
   1056989184,
   0,
   1065353216,
   875575305,
   1065353216,
   0,
   0,
   2147483648,
   875575305,
   3212836864,
   1072373760,
   1051475968,
   1064894464,
   1019568128,
   0,
   1065353216,
   875575305,
   1065353216,
   0,
   0,
   2147483648,
   875575305,
   3212836864,
   1072373760,
   989044736,
   1064894464,
   1056989184,
   0,
   1065353216,
   875575305,
   1065353216,
   0,
   0,
   2147483648,
   875575305,
   3212836864,
   1065377792,
   1051475968,
   1023393792,
   1019568128,
   0,
   1065353216,
   875575305,
   1065353216,
   0,
   0,
   0,
   875575305,
   3212836864,
   994893824,
   1051475968,
   1023393792,
   1065000960,
   0,
   1065353216,
   875575305,
   1065353216,
   0,
   0,
   0,
   875575305,
   3212836864,
   1062625280,
   989044736,
   1064894464,
   1056989184,
   0,
   1065353216,
   875575305,
   1065353216,
   0,
   0,
   0,
   875575305,
   3212836864,
   1062625280,
   1051475968,
   1064894464,
   1065000960,
   0,
   1065353216,
   875575305,
   1065353216,
   0,
   0,
   0,
   875575305,
   3212836864,
   994893824,
   989044736,
   1023393792,
   1056989184;
  }
 }
}