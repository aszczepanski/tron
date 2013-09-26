#include <client/trail.h>
using glm::vec3;
using glm::mat4;
using std::swap;

namespace client{

Trail::Trail(PLAYER_INFO position, std::vector<TURN_INFO>& turns, int color)
  : position_(position), turns_(turns), color_(color), c_(0), v_(0), t_(0), n_(0){

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
  std::string tex;
  if (position_.player_no == 0)
    tex = "p1trail";
  else if (position_.player_no == 1)
    tex = "p2trail";
  else if (position_.player_no == 2)
    tex = "p3trail";

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
      if (size.y < 0){
        begin += vec3(0.05f, 0.05f,0);
        end += vec3(-0.05f, -0.05f,0);
        swap(begin, end);
      }
      else{
        begin += vec3(-0.05f, -0.05f,0);
        end += vec3(0.05f, 0.05f,0);
      }
      AddX(begin, end);
    }

    // If we draw along Y
    else if( fabs(size.x) < fabs(size.y)) {
      if (size.y < 0){
        begin += vec3(0.05f, 0.05f,0);
        end += vec3(-0.05f, -0.05f,0);
        swap(begin, end);
      }
      else{
        begin += vec3(-0.05f, -0.05f,0);
        end += vec3(0.05f, 0.05f,0);
      }
      AddY(begin, end);
    }
  }
  DrawAll(tex);
}


void Trail::AddY(vec3 begin, vec3 end) {
  double length = end.y - begin.y;
  for(int i = 0; i < trailVerticesCount * 3; i+=3){
    v_.push_back(trailVertices[i] + begin.x);
    v_.push_back(trailVertices[i+1] * length + begin.y);
    v_.push_back(trailVertices[i+2]);
  }
  for(int i = 0; i < trailVerticesCount / 4 * 8; i++) {
    t_.push_back(trailTexCoords[i]);
  }
  for(int i = 0; i < trailVerticesCount * 3; i++) {
    n_.push_back(trailNormals[i]);
  }
  c_+= trailVerticesCount;
}


void Trail::AddX(vec3 begin, vec3 end) {
  swap(begin.x, begin.y);
  swap(end.x, end.y);
  AddY(begin, end);

  for(int i = v_.size() - (trailVerticesCount * 3); i < v_.size(); i+=3){
    swap(v_[i], v_[i+1]);
  }
  for(int i = n_.size() - ( trailVerticesCount * 3); i < n_.size(); i+=3) {
    swap(n_[i], n_[i+1]);
  }
}


void Trail::DrawAll(std::string texture){
  printf("%d\n",(int)v_.size());
  printf("%d\n",(int)t_.size());
  printf("%d\n",(int)n_.size());
  printf("%d\n",c_);
  glLoadMatrixf(glm::value_ptr(World::getV() * World::center()));

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

  glVertexPointer(3,GL_FLOAT,0, &v_[0]);
  glTexCoordPointer( 2, GL_FLOAT, 0, &t_[0]);
  glNormalPointer(GL_FLOAT, 0, &n_[0]);
  glDrawArrays(GL_QUADS,0, c_);

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  //glBindTexture(GL_TEXTURE_2D, NULL);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
}

}  // namespace client
