#include <Math/Vector3.h>
#include <Geometry/Ray.h>
#include <Geometry/Triangle.h>
#include <Geometry/CastedRay.h>
#include <stdlib.h>
#include <iostream>
#include <Geometry/RGBColor.h>
#include <Geometry/Material.h>
#include <Geometry/PointLight.h>
#include <Geometry/Camera.h>
#include <Geometry/Cube.h>
#include <Geometry/Disk.h>
#include <Geometry/Cylinder.h>
#include <Geometry/Cone.h>
#include <Geometry/Sphere.h>
#include <Visualizer/Visualizer.h>
#include <Geometry/Scene.h>
#include <Geometry/Cornel.h>
#include <Geometry/BoundingBox.h>
//#include <omp.h>

using Geometry::RGBColor;
using Geometry::Cone;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void initDiffuse(Geometry::Scene & scene)
///
/// \brief	Adds a Cornell Box with diffuse material on each walls to the scene. This Cornel box
/// 		contains two cubes.
///
/// \author	F. Lamarche, Université de Rennes 1
/// \date	03/12/2013
///
/// \param [in,out]	scene	The scene.
////////////////////////////////////////////////////////////////////////////////////////////////////
void initDiffuse(Geometry::Scene & scene)
{
	// MURS
	Geometry::Material * floor		= new Geometry::Material(RGBColor(), RGBColor(1, 1, 1), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * mirror		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 1, 1), 1000, RGBColor(), 0.0);
	// OBJETS DANS LA SCENE
	Geometry::Material * red		= new Geometry::Material(RGBColor(), RGBColor(1, 0, 0), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * green		= new Geometry::Material(RGBColor(), RGBColor(0, 1, 0), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * blue		= new Geometry::Material(RGBColor(), RGBColor(0, 0, 1), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * purple		= new Geometry::Material(RGBColor(), RGBColor(1, 0, 1), RGBColor(), 1, RGBColor(), 0.0);
	// CONSTRUCTION DES MURS
	Geometry::Cornel geo(floor, floor, floor, floor, floor, floor); 

	geo.scaleX(10);
	geo.scaleY(10);
	geo.scaleZ(10);
	scene.add(geo);


	Geometry::Cube cube1(red);
	cube1.translate(Math::Vector3(0, 1, 1));
	scene.add(cube1);
	
	Geometry::Cube cube2(green);
	cube2.translate(Math::Vector3(0, 1, -1));
	scene.add(cube2);

	Geometry::Cube cube3(blue);
	cube3.translate(Math::Vector3(0, -1, 1));
	scene.add(cube3);

	Geometry::Cube cube4(purple);
	cube4.translate(Math::Vector3(0, -1, -1));
	scene.add(cube4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void initSpecular(Geometry::Scene & scene)
///
/// \brief	Adds a Cornel box in the provided scene. Walls are specular and the box contains two 
/// 		cubes.
///
/// \author	F. Lamarche, Université de Rennes 1
/// \date	03/12/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
void initSpecular(Geometry::Scene & scene)
{
	// MURS
	Geometry::Material * floor		= new Geometry::Material(RGBColor(), RGBColor(1, 1, 1), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * mirror		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 1, 1), 1000, RGBColor(), 0.0);
	// OBJETS DANS LA SCENE
	Geometry::Material * red		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 0, 0), 20, RGBColor(), 0.0);
	Geometry::Material * green		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(0, 1, 0), 20, RGBColor(), 0.0);
	Geometry::Material * blue		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(0, 0, 1), 20, RGBColor(), 0.0);
	Geometry::Material * purple		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 0, 1), 20, RGBColor(), 0.0);
	// CONSTRUCTION DES MURS
	Geometry::Cornel geo(mirror, mirror, mirror, mirror, mirror, mirror);

	geo.scaleX(10);
	geo.scaleY(10);
	geo.scaleZ(10);
	scene.add(geo);


	Geometry::Cube cube1(red);
	cube1.translate(Math::Vector3(0, 1, 1));
	scene.add(cube1);
	
	Geometry::Cube cube2(green);
	cube2.translate(Math::Vector3(0, 1, -1));
	scene.add(cube2);

	Geometry::Cube cube3(blue);
	cube3.translate(Math::Vector3(0, -1, 1));
	scene.add(cube3);

	Geometry::Cube cube4(purple);
	cube4.translate(Math::Vector3(0, -1, -1));
	scene.add(cube4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void initDiffuseSpecular(Geometry::Scene & scene)
///
/// \brief	Adds a Cornel box in the provided scene. The cornel box as diffuse and specular walls and 
/// 		contains two boxes.
///
/// \author	F. Lamarche, Université de Rennes 1
/// \date	03/12/2013
///
/// \param [in,out]	scene	The scene.
////////////////////////////////////////////////////////////////////////////////////////////////////
void initDiffuseSpecular(Geometry::Scene & scene)
{
	// MURS
	Geometry::Material * floor		= new Geometry::Material(RGBColor(), RGBColor(1, 1, 1), RGBColor(), 1000, RGBColor(), 0.0);
	Geometry::Material * mirror		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 1, 1), 100, RGBColor(), 0.0);
	// OBJETS DANS LA SCENE
	Geometry::Material * red		= new Geometry::Material(RGBColor(), RGBColor(1, 0, 0), RGBColor(), 20, RGBColor(), 0.0);
	Geometry::Material * green		= new Geometry::Material(RGBColor(), RGBColor(0, 1, 0), RGBColor(), 20, RGBColor(), 0.0);
	Geometry::Material * blue		= new Geometry::Material(RGBColor(), RGBColor(0, 0, 1), RGBColor(), 20, RGBColor(), 0.0);
	Geometry::Material * purple		= new Geometry::Material(RGBColor(), RGBColor(1, 0, 1), RGBColor(), 20, RGBColor(), 0.0);
	// CONSTRUCTION DES MURS
	Geometry::Cornel geo(floor, floor, mirror, mirror, mirror, mirror);

	geo.scaleX(10);
	geo.scaleY(10);
	geo.scaleZ(10);
	scene.add(geo);

	
	Geometry::Cube cube1(red);
	cube1.translate(Math::Vector3(0, 1, 1));
	scene.add(cube1);
	
	Geometry::Cube cube2(green);
	cube2.translate(Math::Vector3(0, 1, -1));
	scene.add(cube2);

	Geometry::Cube cube3(blue);
	cube3.translate(Math::Vector3(0, -1, 1));
	scene.add(cube3);

	Geometry::Cube cube4(purple);
	cube4.translate(Math::Vector3(0, -1, -1));
	scene.add(cube4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void initEmissive(Geometry::Scene & scene)
///
/// \brief	Adds a Cornel box in the provided scene. The cornel box as diffuse and specular walls and 
/// 		contains two boxes.
///
/// \author	F. Lamarche, Université de Rennes 1
/// \date	03/12/2013
///
/// \param [in,out]	scene	The scene.
////////////////////////////////////////////////////////////////////////////////////////////////////
void initRefraction(Geometry::Scene & scene)
{
	// MURS
	Geometry::Material * floor		= new Geometry::Material(RGBColor(), RGBColor(1, 1, 1), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * mirror		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 1, 1), 100, RGBColor(), 0.0);
	// OBJETS DANS LA SCENE
	Geometry::Material * red		= new Geometry::Material(RGBColor(), RGBColor(1, 0, 0), RGBColor(), 20, RGBColor(), 0.0);
	Geometry::Material * green		= new Geometry::Material(RGBColor(), RGBColor(0, 1, 0), RGBColor(), 20, RGBColor(), 0.0);
	Geometry::Material * blue		= new Geometry::Material(RGBColor(), RGBColor(0, 0, 1), RGBColor(), 20, RGBColor(), 0.0);
	Geometry::Material * purple		= new Geometry::Material(RGBColor(), RGBColor(1, 0, 1), RGBColor(), 20, RGBColor(), 0.0);
	
	Geometry::Material * ice		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 1, 1), 1, RGBColor(), 1.309);			// GLACE
	Geometry::Material * water		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 1, 1), 1, RGBColor(), 1.333);			// EAU
	Geometry::Material * soda		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(88, 41, 0), 1, RGBColor(), 1.46);			// SODA
	Geometry::Material * flintGlass	= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 1, 1), 1, RGBColor(), 1.62);			// VERRE FLINT
	Geometry::Material * diamond	= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 1, 1), 1, RGBColor(), 2.42);			// DIAMANT
	// CONSTRUCTION DES MURS
	Geometry::Cornel geo(floor, floor, mirror, mirror, mirror, mirror);

	geo.scaleX(10);
	geo.scaleY(10);
	geo.scaleZ(10);
	scene.add(geo);

	Geometry::Cube cube1(red);
	cube1.translate(Math::Vector3(1, 0.7, -4));
	scene.add(cube1);
	
	Geometry::Cube cube2(green);
	cube2.translate(Math::Vector3(1, -0.7, -4));
	scene.add(cube2);

	Geometry::Cone cone1(100, water);
	cone1.translate(Math::Vector3(0, 0, -1));
	scene.add(cone1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void initEmissive(Geometry::Scene & scene)
///
/// \brief	Adds a Cornel box in the provided scene. The cornel box as diffuse and specular walls and 
/// 		contains two boxes.
///
/// \author	F. Lamarche, Université de Rennes 1
/// \date	03/12/2013
///
/// \param [in,out]	scene	The scene.
////////////////////////////////////////////////////////////////////////////////////////////////////
void initEmissive(Geometry::Scene & scene)
{
	// MURS
	Geometry::Material * floorRed		= new Geometry::Material(RGBColor(), RGBColor(1, 0, 0), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * floorGreen		= new Geometry::Material(RGBColor(), RGBColor(0, 1, 0), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * floorBlue		= new Geometry::Material(RGBColor(), RGBColor(0, 0, 1), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * floorYellow	= new Geometry::Material(RGBColor(), RGBColor(1, 1, 0), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * floorPurple	= new Geometry::Material(RGBColor(), RGBColor(1, 0, 1), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * floorOrange	= new Geometry::Material(RGBColor(), RGBColor(0, 1, 1), RGBColor(), 1, RGBColor(), 0.0);
	Geometry::Material * mirror			= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(1, 1, 1), 1000, RGBColor(), 0.0);
	// OBJETS DANS LA SCENE
	Geometry::Material * red		= new Geometry::Material(RGBColor(), RGBColor(1, 0, 0), RGBColor(1, 0, 0), 20, RGBColor(), 0.0);
	Geometry::Material * green		= new Geometry::Material(RGBColor(), RGBColor(0, 1, 0), RGBColor(0, 1, 0), 20, RGBColor(), 0.0);
	Geometry::Material * blue		= new Geometry::Material(RGBColor(), RGBColor(0, 0, 1), RGBColor(0, 0, 1), 20, RGBColor(), 0.0);
	Geometry::Material * purple		= new Geometry::Material(RGBColor(), RGBColor(1, 0, 1), RGBColor(1, 0, 1), 20, RGBColor(), 0.0);

	Geometry::Material * emissiveWhite		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(), 1, RGBColor(500, 500, 500), 0.0);
	Geometry::Material * emissiveGreen		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(), 1, RGBColor(0, 500, 0), 0.0);
	Geometry::Material * emissiveYellow		= new Geometry::Material(RGBColor(), RGBColor(), RGBColor(), 1, RGBColor(500, 500, 0), 0.0);
	// CONSTRUCTION DES MURS
	Geometry::Cornel geo(floorRed, floorGreen, floorOrange, floorPurple, floorYellow, floorBlue);

	geo.scaleX(10);
	geo.scaleY(10);
	geo.scaleZ(10);
	scene.add(geo);


	Geometry::Cube light1(emissiveWhite);
	light1.translate(Math::Vector3(4, -4, -4));
	scene.add(light1);

	Geometry::Cube light2(emissiveWhite);
	light2.translate(Math::Vector3(-4, 4, 4));
	scene.add(light2);
	
	Geometry::Cube cube1(red);
	cube1.translate(Math::Vector3(0, 1, -1));
	scene.add(cube1);

	Geometry::Cube cube2(blue);
	cube2.translate(Math::Vector3(0, 1, 1));
	scene.add(cube2);

	Geometry::Cube cube3(purple);
	cube3.translate(Math::Vector3(0, -1, -1));
	scene.add(cube3);

	Geometry::Cube cube4(green);
	cube4.translate(Math::Vector3(0, -1, 1));
	scene.add(cube4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void waitKeyPressed()
///
/// \brief	Waits until a key is pressed.
/// 		
/// \author	F. Lamarche, Université de Rennes 1
/// \date	03/12/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
void waitKeyPressed()
{
  SDL_Event event;
  bool done = false;
  while( !done ) {
    while ( SDL_PollEvent(&event) ) {
      switch (event.type) {
        case SDL_KEYDOWN:
        /*break;*/
        case SDL_QUIT:
          done = true;
        break;
        default:
        break;
      }
    }/*while*/
  }/*while(!done)*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int main(int argc, char ** argv)
///
/// \brief	Main entry-point for this application.
///
/// \author	F. Lamarche, Université de Rennes 1
/// \date	03/12/2013
///
/// \param	argc	Number of command-line arguments.
/// \param	argv	Array of command-line argument strings.
///
/// \return	Exit-code for the process - 0 for success, else an error code.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
	 //omp_set_num_threads(8);

	// 1 - Initializes a window for rendering
	Visualizer::Visualizer visu(600,600);
	//Visualizer::Visualizer visu(300,300);
	
	// 2 - Initializes the scene
	Geometry::Scene scene(&visu);

	// 2.1 intializes the geometry (choose only one initialization)
	//initDiffuse(scene);
	//initSpecular(scene);
	//initDiffuseSpecular(scene);
	//initRefraction(scene);
	initEmissive(scene);

	// 2.2 Adds point lights in the scene 
	{
		Geometry::PointLight pointLight(Math::Vector3(-2, 1, 1), RGBColor(0.5, 0.5, 0.5));
		scene.add(pointLight);
	}
	{
		Geometry::PointLight pointLight2(Math::Vector3(4, 0, 0), RGBColor(0.5, 0.5, 0.5));
		scene.add(pointLight2);
	}
	{
		Geometry::PointLight pointLight3(Math::Vector3(-3, 2, 1), RGBColor(0.5, 0.5, 0.5));
		scene.add(pointLight3);
	}
	{
		Geometry::Camera camera(Math::Vector3(-4.0f, 0.0, 0.0), Math::Vector3(0.0, 0.0, 0.0), 0.3f, 1.0f, 1.0f);
		scene.setCamera(camera);
	}

	// 3 - Computes the scene
	scene.compute(1,100);			// Sélectionner le nombre de rebonds et le nombre de rayon aléatoire lancé pour l'illuminatoin globale

	// 4 - waits until a key is pressed
	waitKeyPressed();

	return 0;
}
