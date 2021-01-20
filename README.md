## MyTinyRenderer

No third party to construct graphic pipeline

- first set up frame on windows ,others os will be later on

-  Bresenham’s line algorithm 

Rasterization algorithm

- line sweeping
- barycentric coordinates



Now I faced the design problem on vector, How to design vector class 

it just like:

1.It can maintain the array ,supported index access,also for x ,y,z,w access

2.the most efficient way ,like no extra space

should I contain a union class in vector class?

this page discussed about [a union in a class ](https://gamedev.net/forums/topic/517279-c-making-a-union-inside-a-class/4362271/?page=1)

finally I refer to the implement just like [this](https://github.com/ssloy/tinyrenderer/blob/master/geometry.h)



### bug

I add a orbit camera,then MVP matrix must work ,it works,but still has little problems,

first scroll-down works doesn't like it should does

second when rotating or translating model ,it just didn't get perspective correctly, 

third  I want to do flip instead of copying pixels to window dc, I tried it,but failed!!!

I googled a lot ,but still doesn't know what;s the key ,may be I don't know win32 well.



### future

homogeneous clipping ---- it is so painful to read paper or search ,it looks like it covered a lot about the subject of "projective geometry", however,this subject did not has great reference to it.

may be it is the hardest part of graphics pipeline!!!!! 

perspective division ---it is simple ,just follow up clipping.

shader----how to construct ,it is another problem ,hope I can do it concisely  

### ref

[https://github.com/ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer)

