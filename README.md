# LAVA

LAVA stands for Live Asynchronous Visual Architecture. 

Before describing what each of these terms mean and how they fit together,
 there is something important to emphasize - LAVA is designed to **_both_** significantly **_speed up development_**
AND as a simple and easy way to create **_signifcant amounts of concurrency_**. 
It is written in C++11 and is meant to work with any language that can compile a shared library that exposes standard function calls.
Many of the fundamental building blocks have been created as single file libraries with no dependencies other than the C++11 standard library. Examples include: 

## Fundamental Principles:

#### 1. A program is composed of message passing nodes and data flow nodes. This enables many desirable features: 
 - A clear picture of the high level structure of a program along with a way to plan the program structure in a precise way.
 - Truely modular pieces that have that clearly defined inputs and output with no side effects.
 - Detailed information about each node including input and output types, percentage of CPU time etc.

#### 2. The ability to update nodes live.
 - Since data is separated from execution in a clear way, a recompile of a shared library can trigger an update while the program is running, meaning a program can be changed live, without restarting. 

#### 3. The ability to freeze the input to any node.
 - Combined with live updating of nodes, this allows rapid development of a section of the program since a compile can automatically update a node and the frozen data can be run through automatically, showing the output. 

#### 4. Per node interrupt handling enables the program to continue running live, even if one node crashes. 
 - Crashes are shown clearly and don't hurt workflow.  Running threads will simply skip packets destined for the crashed node and so the overall program will have minimal disruption. 

#### 5. A visual interface for both message passing and data flow nodes.
 - This reduces the complexity of understanding how a program fits together as well as giving easy and intuitive feedback on high level information such as node crashes or the amount of CPU time spent running each node.



## Tools:

### Fissure:

![alt text](https://github.com/LiveAsynchronousVisualizedArchitecture/lava/blob/master/craftsman_fissure.png "Fissure is the node graph UI.  I can be used to construct a graph of nodes, run the program, visualize outputs, see node errors and view timing information about the nodes.")

Fissure is the node graph UI.  I can be used to construct a graph of nodes, run the program, visualize outputs, see node errors and view timing information about the nodes.


### Visualizer:

![alt text](https://github.com/LiveAsynchronousVisualizedArchitecture/lava/blob/master/craftsman_visualizer001.png "Tables that are in the IdxVerts format (3D geometry with optional normals, vertex colors, uvs, and a color texture map)  will be picked up by the visualizer and displayed with openGL.")

Tables that are in the IdxVerts format (3D geometry with optional normals, vertex colors, uvs, and a color texture map)  will be picked up by the visualizer and displayed with openGL.


### Brandisher:

![alt text](https://github.com/LiveAsynchronousVisualizedArchitecture/lava/blob/master/craftsman_brandisher001.png "The brandisher is a tool for viewing tables and their sub-tables in shared memory. It can display a graph the arrays' values as well as basic statistics about the arrays.")

The brandisher is a tool for viewing tables and their sub-tables in shared memory. It can display a graph of the arrays' values as well as basic statistics about the arrays.  This screen shot shows that while some of the points making up the craftsman model are less than 0 on the Y axis, most are above 0.  We can also see the minimum, maximum, average (mean), most common (mode), and median (middle) values as well as the variance.   This is a useful way to get a high level view on arrays containing too many values to be looked at directly as text. 


## Libraries:

LAVA is made out of very few componenents (with tbl.hpp as technically optional, though extremely valuable).  They are all single header files and have no external dependencies outside of C++11 standard libraries. 

####  LavaFlow.hpp
The core that loads nodes dynamically and runs them with the packets of data they produce.  Each thread will simply call the LavaLoop() function to enter the main loop and start executing nodes with their packets.

####  simdb.hpp
Lock free, shared memory key value store for exceptionally fast, concurrent, inter-process communication  

####  tbl.hpp
 - A fast and flexible data structure that combines a vector with a string->number/tbl robinhood hash-map.  
 - The vector can take on any intrinsic numeric type, which is then type checked and range checked during debug mode.   
 - It is always stored in contiguous memory, which means that it has no separate serialized representation. 
 - It can also store nested tbls, creating a simple way to make trees and compound data structures that are full featured, without having to define new structs or classes. 
 - These compound data structures are already a single allocation, always serialized and can be read clearly in a tool like Brandisher to understand the format without needing extensive documentation.  
 - Due to their single span of memory they can easily be written to files and/or read directly from files using memory mapping.
 - The tbl struct contains a pointer to the main data as well as pointrs for allocation, reallocation and free functions. This allows tbls to carry their memory allocation functions with them, which is valuable for use with shared libraries (so that memory allocation functions in the main program don't get mixed with free functions in the shared library and vice versa).

## Examples:

![alt text](https://github.com/LiveAsynchronousVisualizedArchitecture/lava/blob/master/rays_shadeRayHits001.png "Rays generated from a camera and traced to find their collisions with a 3D model using the embree library.")
 
This is an example of rays generated from a camera and traced to find their collisions with a 3D model using the embree library.
  - An .obj model is loaded 
  - The model is passed to a message node that uses the embree library to sort the geometry into a BVH (bounding volume heirarchy acceleration structure for ray tracing).   
  - The camera node generates rays and passes them to the scene node to be traced.  
  - The message node traces the rays and outputs a visualization of the rays colliding with the geometry. 
  - A final node takes the traced rays and computes the outgoing ray from the ray hit point and normal.
