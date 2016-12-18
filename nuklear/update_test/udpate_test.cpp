
#include "../IndexedVerts.cpp"               // todo: make IndexedVerts into an hpp file or a single header file style lib

#include <thread>
#include <chrono>
#include <algorithm>
#include "../no_rt_util.h"
#include "update_test_data.hpp"
#include "update_test_generators.hpp"

namespace {

using std::thread;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;
using ms = duration<double, std::milli>;

IndexedVerts* ChanImgToIndexedVerts(ChanImg ci, std::initializer_list<i64> chns={})
{
  using namespace std;
  
  auto w       =  ci.width();
  auto h       =  ci.height();
  auto _chans  =  chns.size()>0? chns.size() : ci.chans();
  auto chans   =  max<size_t>(4, _chans);
  auto sz      =  4;
  auto iv      =  (IndexedVerts*)IndexedVertsCreate(0,6,IV_QUADS,sz,sz,w,h,chans);
  
  TO(sz,i)
  {
    Vertex& v = iv->verts[i];
    TO(4,c) v.color[c]    = 1.f;
    TO(3,p) v.position[p] = 0.f;
    iv->indices[i] = i;
  }
  iv->verts[1].texCoord[0] = iv->verts[1].position[0] = 1.f;
  iv->verts[2].texCoord[0] = iv->verts[2].position[0] = 1.f;
  iv->verts[0].texCoord[1] = iv->verts[2].position[1] = 1.f;
  iv->verts[1].texCoord[1] = iv->verts[3].position[1] = 1.f;

  if( chns.size() > 0 )
  {
    i64 imgChan = 0;
    for(auto c : chns)
    {
      if(c >= ci.chans()) {
        TO(iv->imgHeight,y) TO(iv->imgWidth,x) {     
          *iv_px(iv,x,y,imgChan)  =  0.f;
        }
      }
      else {
        TO(iv->imgHeight,y) TO(iv->imgWidth,x) {     
          *iv_px(iv,x,y,imgChan)  =  ci(x,y,c);
        }
      }
      ++imgChan;
    }
    if(chns.size() < 4)
      TO(iv->imgHeight,y) TO(iv->imgWidth,x)
        *iv_px(iv,x,y,3) = 1.f;
  }
  else
  {
    TO(iv->imgHeight,y) TO(iv->imgWidth,x) TO(_chans,c) {     
      *iv_px(iv,x,y,c)  =  ci(x,y,c);
    }
    if(_chans < 4)
      TO(iv->imgHeight,y) TO(iv->imgWidth,x)
        *iv_px(iv,x,y,3) = 1.f;    
  }

  return iv;
}

}

int    main(void)
{
  new (&db) simdb("test", 1024, 1<<12);        // 4096x1024 is 4MB inititialize the DB with placement new into the data segment
  
  ui64 rightLen, cubeLen;
  vec<ui8> rightData = makeTriangle(rightLen, false);
  vec<ui8>  cubeData = makeCube(cubeLen);

  //bool ok = db.put("shape", cubeData);

  ChanImg grad(256, 256, 4, 0.f);
  auto pxcnt = 256*256*3;
  TO(pxcnt,i) grad[i] = i/(float)pxcnt;
  //TO(pxcnt,i) grad[i] = 0.5f;

  auto iv = ChanImgToIndexedVerts(move(grad), {0,1,2});
  
  ui64 sz=0;
  IndexedVertsSave(iv, nullptr, &sz);
  vec<i8> gradIvBuf(sz);
  IndexedVertsSave(iv, gradIvBuf.data(), &sz);

  auto idx = db.put("Image Gradient", gradIvBuf);
  assert(idx != simdb::FAILED_PUT);

  //while(true)
  //{
  //  sleep_for( milliseconds(1) );
  //  db.put("shape", cubeData);
  //
  //  sleep_for( milliseconds(1) );    
  //  db.put("shape", rightData);
  //}
  
  return 0;
}




//using std::this_thread;
//
//void       genTestGeo(simdb* db)
//{
//  // Create serialized IndexedVerts
//  size_t leftLen, rightLen, cubeLen;
//  vec<ui8>  leftData = makeTriangle(leftLen,   true);
//  vec<ui8> rightData = makeTriangle(rightLen, false);
//  vec<ui8>  cubeData = makeCube(cubeLen);
//
//  // Store serialized IndexedVerts in the db
//  str  leftTriangle = "leftTriangle";
//  str rightTriangle = "rightTriangle";
//  str          cube = "cube";
//
//  db->put(leftTriangle,   leftData);
//  db->put(rightTriangle, rightData);
//  db->put(cube, cubeData);
//}

//
//genTestGeo(&db);

//#include <map>
//using std::map;
//using std::vector;

//map<int,vector<int>> m;
//for(int i=0; i<10000000; ++i){
//  new (&m) map<int,vector<int>>();
//  m[0] = vector<int>(1000);
//}
