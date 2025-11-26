# TODO: Sage 3d Real-Time Rendering Engine

![IMPORTANT] keynote
``` 
[ x ] -> finished
[...] -> in progress
```

[ x ] project setup
    [ x ] setup opengl and opengl function loader
    [ x ] setup libraries for window management
    [ x ] setup logger
    [ x ] setup build system
    [ x ] setup intermediate gui library (nuklear)

[...] create graphics math library (only what is necessary for now)

[ x ] setup window and input
    [ x ] setup glfw and open a window
    [ x ] setup basic hints and configurations
    [ x ] setup input
        [ x ] setup callbacks
        [ x ] save input stream to diy built input manager?

[ x ] render a triangle
    [ x ] setup rendering pipeline
        [ x ] create shaders for pipeline
            [ x ] vertex shader
            [ x ] fragment shader
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

[...] render basic lighting
    [ x ] write a shader to calculate lighting based on the phong lighting model
        [ x ] ambient reflection
        [ x ] diffuse / lambertian reflection
        [ x ] specular reflection
    [ x ] reconfigure memory buffers and vertex arrays to take in normals
    [ x ] shade multiple light casters

[ x ] render simple materials
    [ x ] diffuse map
    [ x ] specular map
    [ x ] reconfigure texture functions to account for texture units in the GPU

[ x ] render a skybox
    [ x ] setup cubemap textures

[ x ] render a model
    [ x ] setup a model loader

[ x ] render a scene
    [ x ] setup a configurable scene file

[ x ] setup intermediate gui
    [ x ] access transformation functions
