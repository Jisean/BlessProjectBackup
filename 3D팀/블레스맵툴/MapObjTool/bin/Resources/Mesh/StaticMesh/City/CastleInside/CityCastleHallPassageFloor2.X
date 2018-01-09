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
  "LV_Hieracon_City_CastleFloor01_D_KJH.tga";
 }
}

Frame LV_Hieracon_City_HallPassageFloor02_SM_KJH {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh LV_Hieracon_City_HallPassageFloor02_SM_KJH {
  4;
  753.39232;328.174746;-474.57907;,
  593.34118;328.174694;-265.166541;,
  753.39238;328.174746;-265.167232;,
  593.34112;328.174694;-474.57869;;
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
   3033316358,
   1065353216,
   704268341,
   3030199885,
   2786081920,
   3212836864,
   3212836864,
   3033316358,
   882716676,
   1083998208,
   1008549888,
   1065353216,
   1007206400,
   3033316358,
   1065353216,
   704268341,
   3030199889,
   2786082688,
   3212836864,
   3212836864,
   3033316358,
   882716680,
   1080811520,
   1065369600,
   939524096,
   1065353216,
   3033316358,
   1065353216,
   704268341,
   3030153714,
   503316480,
   3212836864,
   3212836864,
   3033316358,
   882671639,
   1080819712,
   1008549888,
   939524096,
   1007206400,
   3033316358,
   1065353216,
   704268316,
   3030153700,
   2659188736,
   3212836864,
   3212836864,
   3033316358,
   882670056,
   1083998208,
   1065369600,
   1065345024,
   1065353216;
  }
 }
}