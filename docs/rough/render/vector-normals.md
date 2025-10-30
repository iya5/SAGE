    Does each vertex have its own normals or does each triangle have its own normals?

Like so often the answer is: "It depends". Since a normal is defined as being the vector perpendicular to all vectors within a given plane (in N dimensions), you need a plane to calculate a normal. A vertex position is just a point and thus singular, so you actually need a face to calculate the normal. Thus, naively, one could assume that normals are per face as the first step in normal calculation is determining the face normals, by evaluating the cross product of the faces edges.
Say you have a triangle with points A, B, C, then these points have position vectors ↑A, ↑B, ↑C and the edges have vectors ↑B - ↑A and ↑C - ↑A so the face normal vector is ↑Nf = (↑B - ↑A) × (↑C - ↑A)
Note that the magnitude of ↑Nf as it's stated above is directly proportional to the face's area.
In smooth surfaces vertices are shared between faces (or you could say those faces share a vertex). In that case the normal at the vertex is not one of the face normals of the faces it is part of, but a linear combination of them:

↑Nv = ∑ p ↑Nf ; where p is a weighting for each face.
One could either assume a equal weighting between the participating face normals. But it makes more sense to assume that the larger a face is, the more it contributes to the normal.
Now recall that you normalize by a vector ↑v by scaling it with it's recipocal length: ↑vi = ↑v/|↑v|. But as already told the length of the face normals already depends on the face's area. So the weighting factor p given above is already contained in the vector itself: Its length, aka magnitude. So we can get the vertex normal vector by simply summing up all the face normals.
In lighting calculations the normal vector must be unit length, i.e. normalized to be useable. So after summing up, we normalize the newly found vertex normal and use that.
The carefull reader may have noticed I specifically said smooth surfaces share vertices. And in fact, if you have some creases / hard edges in your geometry, then the faces on either side don't share vertices. In OpenGL a vertex is the whole combination of

    position
    normal
    (colour)
    N texture coordinates
    M further attributes

You change one of these and you got a completely different vertex. Now some 3D modelers see a vertex only as a point's position and store the rest of those attributes per face (Blender is such a modeler). This saves some memory (or considerable memory, depending on the number of attributes). But OpenGL needs the whole thing, so if working with such a mixed paradigm file you will have to decompose it into OpenGL compatible data first. Have a look at one of Blender's export scripts, like the PLY exporter to see how it's done.

https://stackoverflow.com/a/6661242


---

why are we calculating only 2 edge vectors?
that's because we want to find the vector perpendicular to the face, meaning we
need only two edge vectors to get the cross product or the normal vector.

therefore, given a triangle of vectors A B C,

we only need the edge vector A-B and A-C

B-C doesn't really give any new information because its span lies in the linear
combination of A-B and A-C, thus no new information is given and it is a waste
of calculation anyways.

we only need 2 vectors that are linearly indepedent to create the plane thus
giving a perpendicular vector to that plane (or in this case, the face of the
triangle)
