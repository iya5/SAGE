# TODO: Sage 3d Real-Time Rendering Engine

![IMPORTANT] keynote
``` 
[ x ] -> finished
[...] -> in progress
```

[...] project setup
    [ x ] setup opengl and opengl function loader
    [ x ] setup libraries for window management
    [ x ] setup logger
    [...] setup build system
    [   ] setup intermediate gui library (nuklear)

[...] create graphics math library (only what is necessary for now)

[...] setup window and input
    [...] setup glfw and open a window
    [...] setup basic hints and configurations
    [...] setup input

[ x ] render a triangle
    [ x ] setup rendering pipeline
        [ x ] create shaders for pipeline
            [ x ] vertex shader
            [ x ] fragment shader
            [   ] **(optional)** tesselation shader
            [   ] **(optional)** geometry shader
        [ x ] create shader parser
            [ x ] unify glsl shaders into a single file
        [ x ] setup vertices and memory buffers for GPU

[ x ] render a textured triangle
    [ x ] setup textures
        [ x ] parse texture files
    [ x ] edit rendering pipeline to send texture data to the gpu
    [ x ] edit vertex array to include uv vertices

[ x ] render a triangle in 3d space
    [ x ] setup camera function
        [ x ] setup math functions for coordinate space transformations
            [ x ] shaders convert from local space to clip space (automatically to NDC)
        [ x ] setup math functions for camera
            [ x ] setup inverse model transformation for camera
            [ x ] setup projection function (either orthographic/perspective)
                [ x ] create viewing frustum
        [ x ] setup camera movement
    [ x ] edit vertex array to include vertices in 3d space
    [ x ] reconfigure memory buffers and draw calls

[...] create debug tools
    [...] create world grid
        [   ] setup world grid in shader
    [   ] create gizmo manipulator (some help in ddiakoppoulos/tinygizmo)

[...] render basic lighting
    [...] create shaders for lighting (phong)
        [ x ] ambient
        [...] diffuse
        [   ] specular
    [   ] reconfigure memory buffers and vertex arrays to take in normals
    [   ] setup vertex array parser
        [   ] setup function for normal calculation (if needed)
    [   ] create gouraud shader
    [   ] setup shader for multiple light casters

[   ] render materials


[   ] render a model
    [   ] setup a model loader

[   ] render text

[   ] setup intermediate gui
    [   ] access transformation functions
    [   ] access file system
        [   ] load files
        [   ] save files

[   ]

[   ] render a scene

[   ] setup pbr (physically-based rendering)

[   ] optimizations

--- optional but pretty cool goals!

[   ] create and render procedural terrain
[   ] create and ren
