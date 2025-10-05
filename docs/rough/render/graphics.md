# ...

Rendering pixels onto a screen necessitates hand writing assembly such that we
manually write the pixels onto the frame buffer--VRAM which is the portion of RAM
dedicated for storing pixels and graphics---for the monitor to read and write
onto the screen. 

Layers of abstraction were created to relieve the tedious and low-level task of having to write
thousands of lines of assembly to display a frame.

This is software rendering which makes use of the CPU for generating images, however
GPUs became commonplace in every computer which would take on the role of software
rendering but on dedicated hardware built for graphics. The reason for this is because
CPUs weren't built to handle massive data-parallel workloads, and were instead best
at doing complex tasks. They also had very few cores in comparison to the GPU with
the majority of consumer CPUs having around 4 - 16, all of which are optimized for
handking complex tasks and branch-heavy logic. This pales to GPUs which can have
several thousands, all of which were built for parallel and data heavy tasks, all of which
aren't necessarily "complex" relative to the CPU but are many

Graphics Rendering however, requires many computations to be done, and in the best
case, in parallel. (Imaging rendering a frame on a screen sequentially, and having to do
this many times a second). Specifically in parallel because pixels do not interact
with each other and are very loosely dependent, thus parallel computation would best
allow pixels to be rendered at once in the screen

This makes it perfect for rendering, thus the result is Graphics Rendering

Read below
[on the graphics pipeline and rendering](rendering.md)
