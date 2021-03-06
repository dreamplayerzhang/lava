

#include <thread>
#include "../../no_rt_util.h"
#include "../../tbl.hpp"
#include "../LavaFlow.hpp"
//#include "../str_util.hpp"

struct vert { f32 p[3],n[3],c[4],tx[2]; };
//using IvTbl = tbl<vert>;

using IvTbl = tbl;

static std::atomic<bool> hasRun = false;

extern "C"
{
  //const char*  InNames[] = {"Cube Indexed Verts", nullptr};
  //const char*  InTypes[] = {"IdxVerts",           nullptr};
  const char* OutNames[] = {"Cube Indexed Verts",   nullptr};
  const char* OutTypes[] = {"IdxVerts",             nullptr};

  void MakeCube_construct()
  {
    hasRun = false;
  }
  void MakeCube_destruct()
  {
    hasRun = false;
  }

  uint64_t MakeCube(LavaParams const* inout_lp, LavaFrame const* in, lava_threadQ* out) noexcept
  {
    using namespace std;

    //if( hasRun.exchange(true) ) return 1;

    tbl      iv;
    tbl indices = {0u,       1u,       2u };
    tbl      px = { -1.f,  -0.17f, -0.58f };
    tbl      py = { -1.f,  -1.0f,   1.0f  };
    tbl      pz = {  0.f,   0.f,   -0.f   };
    tbl      nx = {  0.f,   0.f,    0.f   };
    tbl      ny = {  0.f,   0.f,    0.f   };
    tbl      nz = { -1.f,  -1.f,   -1.f   };
    tbl      cr = {  1.f,   1.f,    1.f   };
    tbl      cg = {  1.f,   1.f,    1.f   };
    tbl      cb = {  1.f,   1.f,    1.f   };
    tbl      ca = {  1.f,   1.f,    1.f   };
    tbl      tx = {  0.f,   0.f,    0.f   };
    tbl      ty = {  0.f,   0.f,    0.f   };
    u32    mode        = (u32)0x0004; // (u64)GL_TRIANGLES;  // #define GL_TRIANGLES 0x0004
    u64 typenum        =  *((u64*)"IdxVerts");
    iv("type")         = typenum;
    iv("mode")         = mode;
    iv("indices")      = &indices;
    iv("positions x")  = &px;
    iv("positions y")  = &py;
    iv("positions z")  = &pz;
    iv("normals x")    = &nx;
    iv("normals y")    = &ny;
    iv("normals z")    = &nz;
    iv("colors red")   = &cr;
    iv("colors green") = &cg;
    iv("colors blue")  = &cb;
    iv("colors alpha") = &ca;
    iv("texture coordinates x") = &tx;
    iv("texture coordinates y") = &ty;
    iv.flatten();

    u64   flatSize = iv.sizeBytes();
    void* outAlloc = inout_lp->mem_alloc(flatSize);
    memcpy(outAlloc, iv.memStart(), flatSize);

    LavaOut lo;
    LavaVal lv;
    lv.value    = (u64)outAlloc; //(u64)iv.memStart(); 
    lv.type     = LavaArgType::MEMORY;
    lo.key.slot = 0;
    lo.val      = lv;
    out->push(lo);

    //lo.val  =  
    //auto lo = LavaTblToOut(inout_lp, iv);
    //lo.key.slot = 0;
    //out->push(lo);

    return 1;
  }

  LavaNode LavaNodes[] =
  {
    {
      MakeCube,                                      // function
      MakeCube_construct,                            // constructor
      MakeCube_destruct,                             // destructor
      (uint64_t)LavaNode::MSG,                       // node_type  
      "MakeCube",                                    // name
      nullptr,                                       // in_names
      OutNames,                                      // out_names
      nullptr,                                       // in_types 
      OutTypes,                                      // out_types 
      0                                              // version 
    },                                             

    LavaNodeListEnd                                  // This is a constant that has all the members of the LavaNode struct set to 0 or nullptr - it is used as a special value that ends the static list of LavaNodes. This negates the need for a separate static variable that gives the size of the list, which would be have to be kept in sync and therefore be error prone.
  };

  __declspec(dllexport) LavaNode* GetLavaNodes()
  {
    return (LavaNode*)LavaNodes;
  }
}










//printf("\n MakeCube run \n");
//tbl<f32> idxVerts = tbl<f32>::make_borrowed(inout_lp->mem_alloc, 
//
// todo:  this will allocate using malloc and realloc - need to change tbl to use allocation, reallocation, and free functions in the type
//IvTbl lftTri = {              // array of vert structs
//  {{-1.0, -1.0f, 0.0f},       //pos
//  {0.0f, 0.0f, -1.0f},        //norm
//  {1.0f, 1.0f, 1.0f, 1.0f},   //color
//  {0.0f, 0.0f}},              //texCoord
//
//  {{-0.17f, -1.0f, 0.0f},     //pos
//  {0.0f, 0.0f, -1.0f},        //norm
//  {1.0f, 1.0f, 1.0f, 1.0f},   //color
//  {0.0f, 0.0f}},              //texCoord
//
//  {{-0.58f, 1.0f, 0.0f},      //pos
//  {0.0f, 0.0f, -1.0f},        //norm
//  {1.0f, 1.0f, 1.0f, 1.0f},   //color
//  {0.0f, 0.0f}}               //texCoord
//};
//
//auto typenum    =  "IdxVerts";
//lftTri("type")  =  *((u64*)typenum);
//lftTri("mode")  =  (u64)0x0004; // (u64)GL_TRIANGLES;  // #define GL_TRIANGLES 0x0004
//tu32 ind        =  {0, 1, 2};
//lftTri("IND")   =  &ind; 
//
//lftTri.flatten();
//
//auto lo = LavaTblToOut(inout_lp, lftTri);
//lo.key.slot = 0;
//out->push(lo);

//
//{nullptr, (uint64_t)LavaNode::NONE, nullptr, nullptr, nullptr, nullptr, nullptr, 0}

//Print(lftTri);
//
//Println("before flatten");
//Print(lftTri);
//
//Println("IND tbl"); 
//Print(ind);
//Println();
//
//Print(lftTri);

//inout_lp->outputs = 1;
//out[0] = LavaTblToOut(inout_lp, lftTri);
//out[0].key.slot = 0;

//printf("\n\n dropping null dereference bomb \n\n");
//int*  bomb  =  nullptr;
//int    wat  =  *bomb;
//printf("\n\n bomb dropped \n\n");

//
//this_thread::sleep_for( chrono::milliseconds(500) );

//uint64_t MakeCube(LavaParams* inout_lp, LavaVal* in, LavaOut* out) noexcept
//uint64_t MakeCube(LavaParams* inout_lp, LavaFrame* in, LavaOut* out) noexcept

//void Print(vert const& v)
//{
//  Print("p: ",    v.p[0],  " ", v.p[1], " ", v.p[2]);
//  Print("  n: ",  v.n[0],  " ", v.n[1], " ", v.n[2]);
//  Print("  c: ",  v.c[0],  " ", v.c[1], " ", v.c[2], " ", v.c[3]);
//  Print("  tx: ", v.tx[0], " ", v.tx[1]);
//  Println();
//}
//
//template<class T> void PrintTbl(tbl<T> const& t)
//{
//  Println();
//  i8* memst = (i8*)t.memStart();
//  Println("tbl check: ", memst[0]," ",memst[1]); 
//  Println("owned: ",      t.owned(),
//    " sizeBytes: ",       t.sizeBytes(),
//    " capacity: ",        t.capacity(), 
//    " size: ",            t.size(),  "\n",
//    " map_capacity: ",    t.map_capacity(),
//    " elems: ",           t.elems(), "\n",
//    " child_capacity: ",  t.child_capacity(),
//    " childData: ",       t.childData() );
//
//  Println();
//  Println("-map-");
//  auto e = t.elemStart();
//  TO(t.map_capacity(),i) if( !e[i].isEmpty() ){
//    Print("hash: ", e[i].hsh.hash);
//    Print("  k: ", e[i].key, "   v: ", e[i].val);
//    Print("  type: ", e[i].hsh.type, "  ");
//    if(e[i].hsh.type & HshType::TABLE)   Print("TABLE ");
//    if(e[i].hsh.type & HshType::CHILD)   Print("CHILD ");
//    if(e[i].hsh.type & HshType::INTEGER) Print("INTEGER ");
//    Println();
//  }
//  Println();
//}
//
//void Print(tbl<u32> const& t)
//{
//  PrintTbl(t);
//
//  Println("-array-");
//  TO(t.size(),i){
//    Print(t[i], " ");
//  }
//  Println();
//}
//
//void Print(IvTbl const& iv)
//{
//  PrintTbl(iv);
//
//  Println("-array-");
//  TO(iv.size(),i){
//    Print(iv[i]);
//  }
//  Println();
//}

//void* outmem = inout_lp->mem_alloc(lftTri.sizeBytes());
//memcpy(outmem, lftTri.memStart(), lftTri.sizeBytes());
//
//inout_lp->outputs = 1;
//out[0].value = (u64)outmem;
//out[0].type  = LavaArgType::MEMORY;

//printf("\n lava heap: %llu \n", (u64)lava_thread_heap);
//LavaHeapInit();
//printf("\n lava heap: %llu \n", (u64)lava_thread_heap);

//tbl<u8> filePath(256);
//memset(filePath.data(), 0, 256);
//const char* pthstr = "H:\\projects\\lava\\lava\\tst.txt";
//auto        pthlen = strlen(pthstr);
//memcpy(filePath.data(), pthstr, pthlen+1);

//printf("\n pthstr: %s \n  pthlen: %d  \n filePath.data(): %s \n\n", pthstr, (int)pthlen, filePath.data());
//
//void* outmem = inout_lp->mem_alloc(filePath.sizeBytes());
//memcpy(outmem, filePath.memStart(), filePath.sizeBytes());
//
//((tbl<u8>)outmem).owned(false);

// tbl_str filePathStr = "C:\\projects\\lava\\";
// auto dbid = simdb.put(nodeId, filePathStr);
// LavaArg out[0] = dbid;
//
//this_thread::sleep_for( chrono::milliseconds(1000) );

//const char* MakeCubeTypes[] = {"tbl_str"};
//  
//const char* InNames[]   = {"File Path", nullptr};
//const char* InTypes[]   = {"tbl_str",   nullptr};

//LavaNode LavaNodes[] =
//{
//  {MakeCube, (uint64_t)LavaNode::MSG,         // function, node_type  
//  0, 1, "MakeCube",                           // inputs, outputs, name                                   
//  nullptr, MakeCubeTypes,                     // in_types, out_types 
//  0, 0},                                         // version, id
//
//  {nullptr, (uint64_t)LavaNode::NONE, 0,0, nullptr, nullptr,nullptr, 0, 0}
//};

//((tbl<u8>*)out[0].value)->owned(false);
//
//free( (void*)out[0].value );
//
//this_thread::sleep_for( chrono::milliseconds(3000) );
//
//free( (void*)out[0].value );
