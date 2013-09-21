#include <client/trail.h>
using glm::vec3;
using glm::mat4;

namespace client{

Trail::Trail(PLAYER_INFO position, std::vector<TURN_INFO>& turns, int color)
  : position_(position), turns_(turns), color_(color) {

}


//cubeVertices - tablica ze wspó³rzêdnymi wierzcho³ków.
//cubeColors - tablica z kolorami wierzcho³ków
//cubeVertexCount - liczba wierzcho³ków

float trailVertices[] = {
  0,0,0,
  0,0,0.9,
  0,1,0.9,
  0,1,0,

  0.1,0,0,
  0.1,0,0.9,
  0.1,1,0.9,
  0.1,1,0,

  0.01, 0, 1,
  0.09, 0, 1,
  0.09, 1, 1,
  0.01, 1, 1,


  0, 0, 0.9,
  0.01, 0, 1,
  0.01, 1, 1,
  0, 1, 0.9,


  0.1, 0, 0.9,
  0.09, 0, 1,
  0.09, 1, 1,
  0.1, 1, 0.9


};

float trailTexCoords[] = {
0,0, 1,0, 1,1, 0,1,
0,0, 1,0, 1,1, 0,1,

0,0,  0.1,0,   0.1, 1,  0,1,

0,0,  0.1,0,   0.1, 1,  0,1,
0,0,  0.1,0,   0.1, 1,  0,1
};

float trailNormals[] = {

  -1,0,0,
  -1,0,0,
  -1,0,0,
  -1,0,0,

  1,0,0,
  1,0,0,
  1,0,0,
  1,0,0,

  0,0,1,
  0,0,1,
  0,0,1,
  0,0,1,


  -1,0,1,
  -1,0,1,
  -1,0,1,
  -1,0,1,

  1,0,1,
  1,0,1,
  1,0,1,
  1,0,1
};

int trailVerticesCount = 20;





Trail::~Trail() {}

void Trail::Draw(){
  for(int i = 0; i < turns_.size(); i++){
    TURN_INFO turn = turns_[i];

    vec3 begin = vec3(turn.move.x, turn.move.y, 0);
    vec3 end;

    if (i != turns_.size()-1)
      end = vec3(turns_[i+1].move.x, turns_[i+1].move.y, 0);
    else
      end = vec3(position_.x, position_.y, 0);

    vec3 size = end - begin;


    // If we draw along X
    if( fabs(size.x) > fabs(size.y)) {
      std::swap(size.x, size.y);
      mat4 M;
      if (size.y < 0)
        M = World::transform(begin + vec3(0.05f, -0.05f,0), 0,0,90);
      else
        M = World::transform(begin + vec3(-0.05f, 0.05f,0) , 0,0,-90);
      size.y = fabs(size.y) + 0.1;

      DrawLine(M, size, "red");
    }

    // If we draw along Y
    else if( fabs(size.x) < fabs(size.y)) {
      mat4 M;
      if (size.y < 0)
        M = World::transform(begin + vec3(0.05f, 0.05f,0), 0,0,180);
      else
        M = World::transform(begin + vec3(-0.05f, -0.05f,0), 0,0,0);
      size.y = fabs(size.y) + 0.1;

      DrawLine(M, size, "red");
    }
  }

}

void Trail::DrawLine(mat4 M, vec3 dimensions, std::string texture){
  for(int i = 0; i < trailVerticesCount * 3; i+=3){
    //trailVertices[i+0] *= dimensions.x;
    trailVertices[i+1] *= dimensions.y;
    //trailVertices[i+2] *= dimensions.z;
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
  glEnableClientState(GL_NORMAL_ARRAY);

  glVertexPointer(3,GL_FLOAT,0,trailVertices);
  glTexCoordPointer( 2, GL_FLOAT, 0, trailTexCoords);
  glNormalPointer(GL_FLOAT, 0,trailNormals);
  glDrawArrays(GL_QUADS,0, trailVerticesCount);

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  //glBindTexture(GL_TEXTURE_2D, NULL);

  for(int i = 0; i < trailVerticesCount * 3; i+=3)
    if(trailVertices[i+1] != 0) trailVertices[i+1] = 1;

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);


}

}  // namespace client
