#include <client/shadow.h>
#include <cstdio>
using glm::vec3;
using glm::mat4;
using std::swap;

namespace client{

Shadow::Shadow(PLAYER_INFO position, std::vector<TURN_INFO>& turns, int color)
  : position_(position), turns_(turns), color_(color),
vu_(0), vd_(0), tu_(0), td_(0), nu_(0), nd_(0), cu_(0), cd_(0){

}


//cubeVertices - tablica ze wspó³rzêdnymi wierzcho³ków.
//cubeColors - tablica z kolorami wierzcho³ków
//cubeVertexCount - liczba wierzcho³ków

float shadowVertices[] = {
  0.1,0,0.09,
  0.1,1,0.09,
  0.8,1,0.01,
  0.8,0,0.01,


  0,0,0.09,
  0,1,0.09,
  -0.7,1,0.01,
  -0.7,0,0.01,


  0.1,0,0.95,
  0.1,1,0.95,
  0.5,1,0.91,
  0.5,0,0.91,


  0,0,0.95,
  0,1,0.95,
  -0.4,1,0.91,
  -0.4,0,0.91,


};

float shadowTexCoords[] = {
 0,1, 0,0, 0.9,0, 0.9,1,
 0,1, 0,0, 0.9,0, 0.9,1,
 0,1, 0,0, 0.9,0, 0.9,1,
 0,1, 0,0, 0.9,0, 0.9,1
};

float shadowNormals[] = {
  0.09,0,1,
  0.09,0,1,
  0.09,0,1,
  0.09,0,1,

  -0.09,0,1,
  -0.09,0,1,
  -0.09,0,1,
  -0.09,0,1,

  0.05,0,1,
  0.05,0,1,
  0.05,0,1,
  0.05,0,1,

  -0.05,0,1,
  -0.05,0,1,
  -0.05,0,1,
  -0.05,0,1,

};

int shadowVerticesCount = 16;






float shadowCornerVertices[] = {
  0,0,0.09,
  0,0,0.09,
  0.7,0.7,0.01,
  0.7,0,0.01,

  0,0,0.09,
  0,0,0.09,
  0.7,0.7,0.01,
  0,0.7,0.01,


  0,0,0.95,
  0,0,0.95,
  0.4,0.4,0.91,
  0.4,0,0.91,

  0,0,0.95,
  0,0,0.95,
  0.4,0.4,0.91,
  0,0.4,0.91
};

float shadowCornerTexCoords[] = {
 0,1, 0,0, 0.9,0, 0.9,1,
 0,1, 0,0, 0.9,0, 0.9,1,
 0,1, 0,0, 0.9,0, 0.9,1,
 0,1, 0,0, 0.9,0, 0.9,1
};

float shadowCornerNormals[] = {
  0.09,0,1,
  0.09,0,1,
  0.09,0,1,
  0.09,0,1,

  0,0.09,1,
  0,0.09,1,
  0,0.09,1,
  0,0.09,1,

  0.05,0,1,
  0.05,0,1,
  0.05,0,1,
  0.05,0,1,

  0,0.05,1,
  0,0.05,1,
  0,0.05,1,
  0,0.05,1
};

int shadowCornerVerticesCount = 16;







Shadow::~Shadow() {}

void Shadow::Draw(int placement){
  std::string tex;
  if (position_.player_no == 0)
    tex = "p1shadow";
  else if (position_.player_no == 1)
    tex = "p2shadow";
  else if (position_.player_no == 2)
    tex = "p3shadow";

  for(int i = 0; i < turns_.size(); i++){
    TURN_INFO turn = turns_[i];

    vec3 begin = vec3(turn.move.x, turn.move.y, 0);
    vec3 end;

    if (i != turns_.size()-1)
      end = vec3(turns_[i+1].move.x, turns_[i+1].move.y, 0);
    else
      end = vec3(position_.x, position_.y, 0);

    vec3 size = end - begin;


    if( i != turns_.size()-1){
      if ( (turns_[i].move.direction + 1) % 4  == (turns_[i+1].move.direction + 0) % 4) { // w prawo

        if(turns_[i].move.direction == common::NORTH) //NORTH
          AddCorner(end + vec3(0.05f, 0.05f, 0), 1, 1);
        if(turns_[i].move.direction == common::EAST) //EAST
          AddCorner(end + vec3(-0.05f, 0.05f, 0), -1, 1);
        if(turns_[i].move.direction == common::SOUTH) //SOUTH
          AddCorner(end + vec3(-0.05f, -0.05f, 0), -1, -1);
        if(turns_[i].move.direction == common::WEST) //WEST
          AddCorner(end + vec3(0.05f, -0.05f, 0), 1, -1);
      }
      else {

        if(turns_[i].move.direction == common::NORTH) //NORTH
          AddCorner(end + vec3(-0.05f, 0.05f, 0), -1, 1);
        if(turns_[i].move.direction == common::EAST) //EAST
          AddCorner(end + vec3(-0.05f, -0.05f, 0), -1, -1);
        if(turns_[i].move.direction == common::SOUTH) //SOUTH
          AddCorner(end + vec3(0.05f, -0.05f, 0), 1, -1);
        if(turns_[i].move.direction == common::WEST) //WEST
          AddCorner(end + vec3(0.05f, 0.05f, 0), 1, 1);

      }
    }

    mat4 M;

    // If we draw along X
    if( fabs(size.x) > fabs(size.y)) {
      std::swap(size.x, size.y);
      if (size.y < 0){
        M = World::transform(begin + vec3(0.05f, -0.05f,0), 0,0,90);
        begin += vec3(0.05f, 0.05f,0);
        end += vec3(-0.05f, -0.05f,0);
        swap(begin, end);
      }
      else{
        M = World::transform(begin + vec3(-0.05f, 0.05f,0) , 0,0,-90);
        begin += vec3(-0.05f, -0.05f,0);
        end += vec3(0.05f, 0.05f,0);
      }
      AddX(begin, end);
    }

    // If we draw along Y
    else if( fabs(size.x) < fabs(size.y)) {
      if (size.y < 0){
        M = World::transform(begin + vec3(0.05f, 0.05f,0), 0,0,180);
        begin += vec3(0.05f, 0.05f,0);
        end += vec3(-0.05f, -0.05f,0);
        swap(begin, end);
      }
      else{
        M = World::transform(begin + vec3(-0.05f, -0.05f,0), 0,0,0);
        begin += vec3(-0.05f, -0.05f,0);
        end += vec3(0.05f, 0.05f,0);
      }
      AddY(begin, end);
    }


  }
  DrawAll(tex, 0);
  DrawAll(tex, 1);
}

void Shadow::AddY(vec3 begin, vec3 end) {
  double length = end.y - begin.y;
  for(int i = 0; i < shadowVerticesCount/2 * 3; i+=3){
    vd_.push_back(shadowVertices[i] + begin.x);
    vd_.push_back(shadowVertices[i+1] * length + begin.y);
    vd_.push_back(shadowVertices[i+2]);
  }
  for(int i = shadowVerticesCount/2 * 3; i < shadowVerticesCount * 3; i+=3){
    vu_.push_back(shadowVertices[i] + begin.x);
    vu_.push_back(shadowVertices[i+1] * length + begin.y);
    vu_.push_back(shadowVertices[i+2]);
  }

  for(int i = 0; i < shadowVerticesCount/2 / 4 * 8; i++) {
    td_.push_back(shadowTexCoords[i]);
  }
  for(int i = shadowVerticesCount/2 / 4 * 8; i < shadowVerticesCount / 4 * 8; i++) {
    tu_.push_back(shadowTexCoords[i]);
  }

  for(int i = 0; i < shadowVerticesCount/2 * 3; i++) {
    nd_.push_back(shadowNormals[i]);
  }
  for(int i = shadowVerticesCount/2 * 3; i < shadowVerticesCount * 3; i++) {
    nu_.push_back(shadowNormals[i]);
  }
  cd_+= shadowVerticesCount/2;
  cu_+= shadowVerticesCount/2;
}


void Shadow::AddX(vec3 begin, vec3 end) {
  swap(begin.x, begin.y);
  swap(end.x, end.y);
  AddY(begin, end);

  for(int i = vd_.size() - (shadowVerticesCount/2 * 3); i < vd_.size(); i+=3){
    swap(vd_[i], vd_[i+1]);
    swap(vu_[i], vu_[i+1]);
  }
  for(int i = nd_.size() - ( shadowVerticesCount/2 * 3); i < nd_.size(); i+=3) {
    swap(nd_[i], nd_[i+1]);
    swap(nu_[i], nu_[i+1]);
  }
}


void Shadow::AddCorner(vec3 pos, int x, int y) {
  for(int i = 0; i < shadowCornerVerticesCount/2 * 3; i+=3){
    vd_.push_back(shadowCornerVertices[i] * x + pos.x);
    vd_.push_back(shadowCornerVertices[i+1] * y + pos.y);
    vd_.push_back(shadowCornerVertices[i+2]);
  }
  for(int i = shadowCornerVerticesCount/2 * 3; i < shadowCornerVerticesCount * 3; i+=3){
    vu_.push_back(shadowCornerVertices[i] * x + pos.x);
    vu_.push_back(shadowCornerVertices[i+1] * y + pos.y);
    vu_.push_back(shadowCornerVertices[i+2]);
  }

  for(int i = 0; i < shadowCornerVerticesCount/2 / 4 * 8; i++) {
    td_.push_back(shadowCornerTexCoords[i]);
  }
  for(int i = shadowCornerVerticesCount/2 / 4 * 8; i < shadowCornerVerticesCount / 4 * 8; i++) {
    tu_.push_back(shadowCornerTexCoords[i]);
  }

  for(int i = 0; i < shadowCornerVerticesCount/2 * 3; i++) {
    nd_.push_back(shadowCornerNormals[i]);
  }
  for(int i = shadowCornerVerticesCount/2 * 3; i < shadowCornerVerticesCount * 3; i++) {
    nu_.push_back(shadowCornerNormals[i]);
  }
  cd_+= shadowCornerVerticesCount/2;
  cu_+= shadowCornerVerticesCount/2;
}



void Shadow::DrawAll(std::string texture, int placement){
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

  if(placement == 0) {
    glVertexPointer(3,GL_FLOAT,0, &vd_[0]);
    glTexCoordPointer( 2, GL_FLOAT, 0, &td_[0]);
    glNormalPointer(GL_FLOAT, 0, &nd_[0]);
    glDrawArrays(GL_QUADS,0, cd_);
  }
  else{
    glVertexPointer(3,GL_FLOAT,0, &vu_[0]);
    glTexCoordPointer( 2, GL_FLOAT, 0, &tu_[0]);
    glNormalPointer(GL_FLOAT, 0, &nu_[0]);
    glDrawArrays(GL_QUADS,0, cu_);
  }

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);


  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);


}


void Shadow::DrawCorner(mat4 M, std::string texture, int placement){
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

  glVertexPointer(3,GL_FLOAT,0,&shadowCornerVertices[placement*shadowCornerVerticesCount*3/2]);
  glTexCoordPointer( 2, GL_FLOAT, 0, &shadowCornerTexCoords[placement*shadowCornerVerticesCount*2/2]);
  glNormalPointer(GL_FLOAT, 0, &shadowCornerNormals[placement*shadowCornerVerticesCount*3/2]);
  glDrawArrays(GL_QUADS,0, shadowCornerVerticesCount/2);

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);


  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
}


}  // namespace client
