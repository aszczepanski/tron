#include <client/shadow.h>
#include <cstdio>
using glm::vec3;
using glm::mat4;

namespace client{

Shadow::Shadow(PLAYER_INFO position, std::vector<TURN_INFO>& turns, int color)
  : position_(position), turns_(turns), color_(color) {

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
  for(int i = 0; i < turns_.size(); i++){
    TURN_INFO turn = turns_[i];

    vec3 begin = vec3(turn.move.x, turn.move.y, 0);
    vec3 end;

    if (i != turns_.size()-1)
      end = vec3(turns_[i+1].move.x, turns_[i+1].move.y, 0);
    else
      end = vec3(position_.x, position_.y, 0);

    vec3 size = end - begin;


    mat4 M;

    // If we draw along X
    if( fabs(size.x) > fabs(size.y)) {
      std::swap(size.x, size.y);
      if (size.y < 0)
        M = World::transform(begin + vec3(0.05f, -0.05f,0), 0,0,90);
      else
        M = World::transform(begin + vec3(-0.05f, 0.05f,0) , 0,0,-90);
      size.y = fabs(size.y) + 0.1;

      DrawLine(M, size, "shadow", placement);
    }

    // If we draw along Y
    else if( fabs(size.x) < fabs(size.y)) {
      if (size.y < 0)
        M = World::transform(begin + vec3(0.05f, 0.05f,0), 0,0,180);
      else
        M = World::transform(begin + vec3(-0.05f, -0.05f,0), 0,0,0);
      size.y = fabs(size.y) + 0.1;

      DrawLine(M, size, "shadow", placement);
    }
    size.x = 0.1;

    if( i != turns_.size()-1){
      if ( (turns_[i].move.direction + 1) % 4  == (turns_[i+1].move.direction + 0) % 4) { // w prawo
        M = glm::translate(M, size);
        M = World::transform(M, vec3(0), 0, 0, 0);
        DrawCorner(M, "shadow", placement);

      }
      else {
        M = glm::translate(M, size);
        M = World::transform(M, vec3(-0.1,0,0), 0, 0, 90);
        DrawCorner(M, "shadow", placement);
      }
    }
  }
}

void Shadow::DrawLine(mat4 M, vec3 dimensions, std::string texture, int placement){
  for(int i = 0; i < shadowVerticesCount * 3; i+=3){
    //trailVertices[i+0] *= dimensions.x;
    shadowVertices[i+1] *= dimensions.y;
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

  glVertexPointer(3,GL_FLOAT,0, &shadowVertices[placement*shadowVerticesCount*3/2]);
  glTexCoordPointer( 2, GL_FLOAT, 0, &shadowTexCoords[placement*shadowVerticesCount*2/2]);
  glNormalPointer(GL_FLOAT, 0,&shadowNormals[placement*shadowVerticesCount*3/2]);
  glDrawArrays(GL_QUADS,0, shadowVerticesCount/2);

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  for(int i = 0; i < shadowVerticesCount * 3; i+=3)
    if(shadowVertices[i+1] != 0) shadowVertices[i+1] = 1;

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
