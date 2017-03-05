# RayTracing
Implementation of RayTracing algorithm.

This project has been done with Visual Studio 2015, with visual C++ 2010 compiler.

# Global illumination - How to run it
You need to configure the main.cpp line 316.

```C++
// 2.1 intializes the geometry (choose only one initialization)
	//initDiffuse(scene);
	//initSpecular(scene);
	//initDiffuseSpecular(scene);
	//initRefraction(scene);
	initEmissive(scene);

// 3 - Computes the scene
	scene.compute(1,100);	//first : number of rebound | second : number of random rays send for global illumination
```

Then run it and wait.

