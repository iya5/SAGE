# ...

Rendering is done via the Graphics Pipeline.

The use of the term Graphics Pipeline or Render Pipeline are both synonymous 
with each other and will be used interchangeably

It starts with providing an array of vertices and the render pipeline that
transforms the vertices

The Graphics Pipeline consist of several stages shown below

![Graphics Pipeline](vulkan-pipeline-simplified.svg)

During the stage of the vertex shader, after processing the transformations
if there are any, OpenGL expects the coordinates to be in Normalized Device
Coordinates where each vertex is between -1.0 to 1.0. These are then the points
passed on to the rasterizer to transform the 3D space into the 2D screen

There are 5 main coordinate space transformations that are of importance

1. Local Space (Object Space)
2. World Space
3. View Space (Eye Space)
4. Clip Space
5. Screen Space

Each of these spaces are done by a series of linear transformations.
Take for example, a mesh with a local origin (in local space). 

To convert this mesh into world space, a model matrix transformation is done. 
The mesh is then place into the World coordinate space relative to the world's
origin. 

The next transformation done is from world space to eye space. This is done by
setting up a virtual camera with a position in world space, an up direction, and
forward direction. The virtual camera will be the basis for the next trasnformation
using the view matrix to convert the world space into eye space. Thus everything
is relative and facing the camera.

Eye space doesn't change how the screen will actually display the vertices but
only handles positioning everything relative to the defined virtual camera.
The projection matrix is what handles transformating what the camera is pointing
at to what is visible to the screen, called the Homomgenous Clip Space. It gets
its name by "clipping" geometry not part of the viewing frustrum created by
the projection matrix.

After that is done, Perspective Division is done to convert that into NDC
which is then scaled back up to the viewport space or the screen space

The resulting coordinates are sent to the rasterizer to convert them into
fragments
