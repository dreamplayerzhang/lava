
#include "../../no_rt_util.h"
#include "../../tbl.hpp"
#include "../LavaFlow.hpp"

enum Slots
{
  // This is an example enumeration that is meant to be helpful, though is not strictly neccesary. Referencing slots by a name will generally be less error prone than using their index and remembering what each index is for
  
  SLOT_0 = 0,        
  SLOT_1 = 1
  //SLOT_2 = 2,
  //SLOT_3 = 3
};

extern "C"
{
  const char*  InNames[]  = {"|_Name_| Slot In",   nullptr};            // This array contains the names of each input slot as a string that can be used by the GUI.  It will show up as a label to each slot and be used when visualizing.
  const char*  InTypes[]  = {"IdxVerts",           nullptr};            // This array contains the type that each slot of the same index will accept as input.
  const char* OutNames[]  = {"|_Name_| Slot Out",  nullptr};            // This array contains the names of each output slot as a string that can be used by the GUI.  It will show up as a label to each slot and be used when visualizing.
  const char* OutTypes[]  = {"IdxVerts",           nullptr};            // This array contains the types that are output in each slot of the same index

  uint64_t |_Name_|(LavaParams* lp, LavaFrame* in, lava_threadQ* out) noexcept
  {
    using namespace std;

    u32 i=0;
    while( LavaNxtPckt(in, &i) )
    {
      tbl inputTbl( (void*)(in->packets[i].val.value) );

      for(auto& kv : inputTbl){  // this loop demonstrates how to iterate through non-empty map elements
      }	

      // out->push( LavaTblToOut(outputTbl, SLOT_0) );      // this demonstrates how to output a tbl into the first output slot
    }

    return 1;
  }

  LavaNode LavaNodes[] =
  {
    {
      |_NAME_|,                                      // function
      |_NAME_|_construct,                            // constructor
      |_NAME_|_destruct,                             // destructor      
      LavaNode::MSG,                                 // node_type  
      "|_NAME_|",                                    // name
      InNames,                                       // in_names
      OutNames,                                      // out_names
      InTypes,                                       // in_types 
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

