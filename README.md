## MyTinyRenderer

No third party to construct graphic pipeline

- first set up frame on windows ,others os will be later on

-  Bresenham’s line algorithm 

Rasterization algorithm

- line sweeping
- barycentric coordinates



Now I faced the design problem on vector, How to design vector class 

it just like:

1.can maintain the array ,supported index access,also for x ,y,z,w access

2.the most efficient way ,like no extra space

should I contain a union class in vector class?

this page discussed about [a union in a class ](https://gamedev.net/forums/topic/517279-c-making-a-union-inside-a-class/4362271/?page=1)

finally I refer to the implement just like [this](https://github.com/ssloy/tinyrenderer/blob/master/geometry.h)

### ref

[https://github.com/ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer)

