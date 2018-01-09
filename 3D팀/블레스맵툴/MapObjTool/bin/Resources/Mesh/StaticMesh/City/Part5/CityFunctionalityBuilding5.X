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
  "LV_Hieracon_City_CastleWallAddWindow_D_KMS.tga";
 }
}

Frame LV_Hieracon_City_FunctionalityBuilding08_SM_KMS {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh LV_Hieracon_City_FunctionalityBuilding08_SM_KMS {
  8;
  200.000026;-7.000034;-0.000015;,
  -200.000000;-7.000009;150.000000;,
  -200.000000;-7.000002;0.000000;,
  200.000064;-7.000037;150.000000;,
  200.000000;-7.000030;-150.000026;,
  -200.000000;-7.000002;0.000000;,
  -200.000000;-6.999994;-150.000000;,
  200.000026;-7.000034;-0.000015;;
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
   { material_0 }
  }

  DeclData {
   5;
   2;0;3;0;,
   2;0;6;0;,
   2;0;7;0;,
   1;0;5;0;,
   1;0;5;1;;
   104;
   3014530171,
   3212836864,
   3008154830,
   1065353216,
   3014530171,
   2987765564,
   840302598,
   3008154830,
   1065353216,
   1069260800,
   1060904960,
   939524096,
   1056546816,
   3014530171,
   3212836864,
   3008154830,
   1065353216,
   3014530171,
   2997809319,
   850329602,
   3008154831,
   1065353216,
   3203325952,
   1011367936,
   1065345024,
   1064599552,
   3014530171,
   3212836864,
   3008154830,
   1065353216,
   3014530171,
   3005470473,
   857989121,
   3008154831,
   1065353216,
   3203334144,
   1060904960,
   1065345024,
   1056546816,
   3013187993,
   3212836864,
   2998871427,
   1065353216,
   3013187993,
   2604606851,
   2147483648,
   2998871427,
   1065353216,
   1069268992,
   1011367936,
   939524096,
   1064599552,
   3015872348,
   3212836864,
   3008154829,
   1065353216,
   3015872348,
   3011102548,
   863618900,
   3008154830,
   1065353216,
   1069260800,
   1065156608,
   939524096,
   1017561088,
   3015872348,
   3212836864,
   3008154829,
   1065353216,
   3015872348,
   3012946353,
   865462704,
   3008154831,
   1065353216,
   3203334144,
   1049100288,
   1065345024,
   1056546816,
   3015872348,
   3212836864,
   3008154829,
   1065353216,
   3015872347,
   3013859082,
   866375433,
   3008154831,
   1065353216,
   3203334144,
   1065156608,
   1065345024,
   1017561088,
   3014530171,
   3212836864,
   2998871426,
   1065353216,
   3014530171,
   3005470473,
   857986824,
   2998871427,
   1065353216,
   1069260800,
   1049100288,
   939524096,
   1056546816;
  }
 }
}