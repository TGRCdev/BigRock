# BigRock [![Build Status](https://travis-ci.org/TGRCdev/BigRock.svg?branch=master)](https://travis-ci.org/TGRCdev/BigRock) [![Build status](https://ci.appveyor.com/api/projects/status/hf34kno5f57llcc6/branch/master?svg=true)](https://ci.appveyor.com/project/TigerCaldwell/bigrock/branch/master) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/ec715b44f8c243d093c75cde4de2f098)](https://www.codacy.com/app/TGRCdev/BigRock?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=TGRCdev/BigRock&amp;utm_campaign=Badge_Grade)

A 3D terrain library written in C++ (currently dependent on C++11), designed to be wrapped for any 3D game engine. It makes use of CSG primitives to place, remove and modify terrain in a sparse voxel octree. I plan to use multithreading and OpenCL to boost performance to run at real-time speeds on low-spec computers.

This library is currently incomplete, but is being worked on.

## Current Dependencies

- OpenGL Mathematics
    - <https://glm.g-truc.net/>
- FlatBuffers
    - <https://google.github.io/flatbuffers/>

![Prototype Image](https://user-images.githubusercontent.com/4079184/57591767-de752f80-74e7-11e9-854d-72f5aeb6f3b6.jpg)
