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
  "LV_Hieracon_City_CastleWall03_D_KJH.tga";
 }
}

Frame LV_Hieracon_City_HallPassageTop02_SM_KJH {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh LV_Hieracon_City_HallPassageTop02_SM_KJH {
  4;
  753.39251;614.56090;-265.167258;,
  593.34118;614.56090;-474.57872;,
  753.39238;614.56090;-474.57910;,
  593.34125;614.56083;-265.166566;;
  2;
  3;0,1,2;,
  3;1,0,3;;

  MeshMaterialList {
   1;
   2;
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
   52;
   0,
   3212836864,
   0,
   3153608020,
   0,
   1065352735,
   3212836383,
   0,
   3153608020,
   1066950656,
   999858176,
   1065353216,
   1007198208,
   0,
   3212836864,
   0,
   3153656351,
   0,
   1065352732,
   3212836380,
   0,
   3153656350,
   3145908224,
   1065410560,
   939524096,
   1065353216,
   0,
   3212836864,
   0,
   3153775255,
   0,
   1065352725,
   3212836373,
   0,
   3153775256,
   3159752704,
   1013915648,
   939524096,
   1007198208,
   885832660,
   3212836864,
   3030153703,
   3153489142,
   3030254316,
   1065352742,
   3212836390,
   3033238809,
   3153489142,
   1067024384,
   1065304064,
   1065353216,
   1065353216;
  }
 }
}