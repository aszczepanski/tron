#include <client/floor.h>
using namespace glm;

//cubeVertices - tablica ze wspó³rzêdnymi wierzcho³ków.
//cubeColors - tablica z kolorami wierzcho³ków
//cubeVertexCount - liczba wierzcho³ków

float floorVertices[] = {
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


float floorTexCoords[] = {
0,0, 12,0, 12,12, 0,12,
0,0, 12,0, 12,12, 0,12,
0,0, 12,0, 12,12, 0,12,
0,0, 12,0, 12,12, 0,12,
0,0, 12,0, 12,12, 0,12,
0,0, 12,0, 12,12, 0,12
};

float floorNormals[] = {
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

Floor::Floor(vec3 dimensions){
  _dimensions = dimensions;
}

Floor::~Floor(){}

void Floor::draw(mat4 M, std::string texture){
  for(int i = 0; i < 24 * 3; i+=3){
    floorVertices[i+0] *= _dimensions.x;
    floorVertices[i+1] *= _dimensions.y;
    floorVertices[i+2] *= _dimensions.z;
  }

  glLoadMatrixf(glm::value_ptr(World::getV() * M));

    TextureManager::setTexture(texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState( GL_NORMAL_ARRAY );

  glVertexPointer(3,GL_FLOAT,0,floorVertices);
  glTexCoordPointer( 2, GL_FLOAT, 0, floorTexCoords);
  glNormalPointer(GL_FLOAT, 0,floorNormals);
  glDrawArrays(GL_QUADS,0, 24);

  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  for(int i = 0; i < 24 * 3; i++)
    if( floorVertices[i] != 0) floorVertices[i] /= floorVertices[i];

  //glBindTexture(GL_TEXTURE_2D, NULL);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);

  }

}  // namespace client
