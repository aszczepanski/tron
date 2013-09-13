#include <client/box.h>
using namespace glm;

//cubeVertices - tablica ze wspó³rzêdnymi wierzcho³ków.
//cubeColors - tablica z kolorami wierzcho³ków
//cubeVertexCount - liczba wierzcho³ków

float boxVertices[] = {
  0,0,0,
  0,1,0,
  1,1,0,
  1,0,0,

  0,0,1,
  0,1,1,
  1,1,1,
  1,0,1,

  0,0,0,
  0,0,1,
  1,0,1,
  1,0,0,

  0,1,0,
  0,1,1,
  1,1,1,
  1,1,0,

  0,0,0,
  0,0,1,
  0,1,1,
  0,1,0,

  1,0,0,
  1,0,1,
  1,1,1,
  1,1,0
};


float boxColors[] = {
  0.15,0.15,0.15, 0.15,0.15,0.15, 0.15,0.15,0.15, 0.15,0.15,0.15,
  0.4,0.4,0.4, 0.4,0.4,0.4, 0.4,0.4,0.4, 0.4,0.4,0.4,
  0.35,0.35,0.35, 0.35,0.35,0.35, 0.35,0.35,0.35, 0.35,0.35,0.35,
  0.3,0.3,0.3, 0.3,0.3,0.3, 0.3,0.3,0.3, 0.3,0.3,0.3,
  0.25,0.25,0.25, 0.25,0.25,0.25, 0.25,0.25,0.25, 0.25,0.25,0.25,
  0.2,0.2,0.2, 0.2,0.2,0.2, 0.2,0.2,0.2, 0.2,0.2,0.2,


};

float boxTexCoords[] = {
0,0, 1,0, 1,1, 0,1,
0,0, 1,0, 1,1, 0,1,
0,0, 1,0, 1,1, 0,1,
0,0, 1,0, 1,1, 0,1,
0,0, 1,0, 1,1, 0,1,
0,0, 1,0, 1,1, 0,1
};

float boxNormals[] = {
  0,0,-1,
  0,0,-1,
  0,0,-1,
  0,0,-1,

  0,0,1,
  0,0,1,
  0,0,1,
  0,0,1,

  0,-1,0,
  0,-1,0,
  0,-1,0,
  0,-1,0,

  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,

  -1,0,0,
  -1,0,0,
  -1,0,0,
  -1,0,0,

  1,0,0,
  1,0,0,
  1,0,0,
  1,0,0
};



namespace client {

Box::Box(vec3 dimensions){
  _dimensions = dimensions;
}

Box::~Box(){}

void Box::draw(mat4 M, GLuint texture){
  for(int i = 0; i < 24 * 3; i+=3){
    boxVertices[i+0] *= _dimensions.x;
    boxVertices[i+1] *= _dimensions.y;
    boxVertices[i+2] *= _dimensions.z;
  }

  glLoadMatrixf(glm::value_ptr(World::getV() * M));

  glEnable(GL_NORMALIZE);

  float amb[] = {0.3, 0.3, 0.3, 0.1},
        dif[] = {0.5, 0.5, 0.5, 0.1},
        spe[] = {0.5, 0.5, 0.5, 1.0},
        shi  = 50.0;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb );
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif );
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe );
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shi );

  glBindTexture(GL_TEXTURE_2D, texture);

  if(texture){
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    /*float amb[] = {0.9, 0.9, 0.9, 1.0},
          dif[] = {0.5, 0.5, 0.5, 0.2},
          spe[] = {0.5, 0.5, 0.5, 1.0},
          shi  = 50.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb );
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif );
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe );
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shi );*/
  }
  else{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  //glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState( GL_NORMAL_ARRAY );

  glVertexPointer(3,GL_FLOAT,0,boxVertices);
  glColorPointer(3,GL_FLOAT,0, boxColors);
  glTexCoordPointer( 2, GL_FLOAT, 0, boxTexCoords);
  glNormalPointer(GL_FLOAT, 0,boxNormals);
  glDrawArrays(GL_QUADS,0, 24);

  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  //glDisableClientState(GL_COLOR_ARRAY);

  for(int i = 0; i < 24 * 3; i++)
    if( boxVertices[i] != 0) boxVertices[i] /= boxVertices[i];
  glBindTexture(GL_TEXTURE_2D, NULL);
}

}  // namespace client
