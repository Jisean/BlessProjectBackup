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


Material Material__77 {
 0.588000;0.588000;0.588000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "LV_Hieracon_City_CastleWall03_D_KJH.tga";
 }
}

Frame LV_Hieracon_City_HallPassage02_SM_KJH {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh LV_Hieracon_City_HallPassage02_SM_KJH {
  8;
  753.39251;328.174746;-265.167258;,
  593.34125;614.56083;-265.166566;,
  753.39251;614.56090;-265.167258;,
  593.34125;328.174694;-265.166566;,
  753.39238;614.56090;-474.57910;,
  593.34118;328.174694;-474.57872;,
  753.39238;328.174746;-474.57910;,
  593.34118;614.56090;-474.57872;;
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
   { Material__77 }
  }

  DeclData {
   5;
   2;0;3;0;,
   2;0;6;0;,
   2;0;7;0;,
   1;0;5;0;,
   1;0;5;1;;
   104;
   3063097121,
   0,
   3212836864,
   1065353216,
   886384779,
   3063097121,
   886384779,
   3212836864,
   2884963769,
   3219824640,
   1068367872,
   1065320448,
   1057562624,
   3063097121,
   0,
   3212836864,
   1065353216,
   886384779,
   3063097121,
   886384779,
   3212836864,
   2884963769,
   3224690688,
   3201613824,
   3087007744,
   1065353216,
   3063097121,
   0,
   3212836864,
   1065353216,
   885832705,
   3063097121,
   885832705,
   3212836864,
   2884329087,
   3219824640,
   3201613824,
   3087007744,
   1057562624,
   3063097121,
   0,
   3212836864,
   1065353216,
   885832705,
   3063097121,
   885832705,
   3212836864,
   2884329087,
   3224690688,
   1068367872,
   1065320448,
   1065353216,
   908482757,
   0,
   1065353216,
   3212836864,
   3017831175,
   908482757,
   870347527,
   3212836864,
   2861684843,
   3230105600,
   3200491520,
   3087007744,
   1056055296,
   908482757,
   0,
   1065353216,
   3212836864,
   3028963236,
   908482757,
   881479588,
   3212836864,
   2872295220,
   3226443776,
   1068654592,
   1065320448,
   1007951872,
   908482757,
   0,
   1065353216,
   3212836864,
   3033316357,
   908482757,
   885832709,
   3212836864,
   2877622505,
   3230105600,
   1068654592,
   1065320448,
   1056055296,
   908482758,
   0,
   1065353216,
   3212836864,
   0,
   908482758,
   0,
   3212836864,
   0,
   3226443776,
   3200491520,
   3087007744,
   1007951872;
  }
 }
}