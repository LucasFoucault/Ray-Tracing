#ifndef _Geometry_Scene_H
#define _Geometry_Scene_H

#include <windows.h>
#include <Geometry/Geometry.h>
#include <Geometry/PointLight.h>
#include <Visualizer/Visualizer.h>
#include <Geometry/Camera.h>
#include <Geometry/BoundingBox.h>
#include <Math/RandomDirection.h>
#include <math.h>
#include <windows.h>
#include <Geometry/CastedRay.h>
#include <System/aligned_allocator.h>
#include <deque>
#include <limits>

using namespace std;

namespace Geometry
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Scene
	///
	/// \brief	An instance of a geometric scene that can be rendered using ray casting. A set of methods
	/// 		allowing to add geometry, lights and a camera are provided. Scene rendering is achieved by
	/// 		calling the Scene::compute method.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	03/12/2013
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Scene
	{
	protected:
		/// \brief	The visualizer (rendering target).
		Visualizer::Visualizer * m_visu;
		/// \brief	The scene geometry (basic representation without any optimization).
		std::deque<std::pair<BoundingBox, Geometry> > m_geometries;
		//Geometry m_geometry;
		/// \brief	The lights.
		std::deque<PointLight, aligned_allocator<PointLight, 16> > m_lights;
		/// \brief	The camera.
		Camera m_camera;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Scene::Scene(Visualizer::Visualizer * visu)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/12/2013
		///
		/// \param [in,out]	visu	If non-null, the visu.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Scene(Visualizer::Visualizer * visu)
			: m_visu(visu)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Scene::add(const Geometry & geometry)
		///
		/// \brief	Adds a geometry to the scene.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/12/2013
		///
		/// \param	geometry The geometry to add.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void add(const Geometry & geometry)
		{
			//m_geometry.merge(geometry) 
			BoundingBox box(geometry);
			m_geometries.push_back(::std::make_pair(box, geometry));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Scene::add(PointLight * light)
		///
		/// \brief	Adds a poitn light in the scene.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/12/2013
		///
		/// \param [in,out]	light	If non-null, the light to add.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void add(const PointLight & light)
		{
			m_lights.push_back(light);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Scene::setCamera(Camera const & cam)
		///
		/// \brief	Sets the camera.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/12/2013
		///
		/// \param	cam	The camera.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setCamera(Camera const & cam)
		{
			m_camera = cam;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	int intersectBoundingBox(Ray const & ray, int depth, int maxDepth)
		///
		/// \brief	Detecte une intersection entre le volume englobant et le rayon.
		///
		/// \author	L. Foucault & V. Goupoil, Université de Rennes 1
		/// \date	06/11/2015
		///
		/// \param	ray			Le rayon.
		/// \param	depth   	La profondeur courrante.
		/// \param	maxDepth	La profondeur maximum.
		///
		/// \return	L'indice de la liste de geometrie ou on detecte une intersection. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int intersectBoundingBox(Ray const & ray, int depth, int maxDepth)
		{
			for(int i = 0; i < m_geometries.size(); i++)
			{
				if(m_geometries[i].first.intersect(ray, depth, maxDepth))
				{
					return i;
				}
			} 

			return -1;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	RayTriangleIntersection * intersectTriangle(Ray const & ray)
		///
		/// \brief	Detecte l'intersection entre le triangle d'une geometrie et le rayon.
		///
		/// \author	L. Foucault & V. Goupoil, Université de Rennes 1
		/// \date	06/11/2015
		///
		/// \param	ray			Le rayon..
		///
		/// \return	Le RayTriangleIntersection representant l'intersction entre le rayon et le triangle.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RayTriangleIntersection intersectTriangle(Ray const & ray)
		{
			int indiceG = 0, indiceT = 0; //indice de la geometrie , du triangle
			float profondeurMin = std::numeric_limits<float>::max();

			CastedRay castedRay(ray.source(), ray.direction());

			//parcours de toutes les geometries
			for(int i=0; i<m_geometries.size(); i++)
			{
				const std::deque<Triangle, aligned_allocator<Triangle, 16> > & listTriangle = m_geometries[i].second.getTriangles();
				
				//parcours de tous les triangles de la geometrie courante
				for(int j=0; j< listTriangle.size(); j++)
				{	
					float profondeur,u,v;

					//si l'intersection entre le rayon et le triangle actuel est valide
					if(((m_geometries[i].second).getTriangles()[j]).intersection(ray, profondeur, u, v))
					{
						//si la distance entre la source du rayon et le point d'intercection
						//est inferieur à profondeurMin
						if (profondeur < profondeurMin)
						{	
							//alors on met a jour la profondeur et les indices
							profondeurMin = profondeur;
							indiceG = i;
							indiceT = j;
						}
					}
				}
			}

			return RayTriangleIntersection(&((this->m_geometries[indiceG].second).getTriangles()[indiceT]), &ray);
		}
	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	RGBColor sendRay(Ray const & ray, int depth, int maxDepth)
		///
		/// \brief	Sends a ray in the scene and returns the computed color
		///
		/// \author	L. Foucault & V. Goupoil, Université de Rennes 1
		/// \date	06/11/2015
		///
		/// \param	ray			Le rayon.
		/// \param	depth   	La profondeur courrante.
		/// \param	maxDepth	La profondeur maximum.
		///
		/// \return	La couleur du triangle intercepte par le rayon.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RGBColor sendRay(Ray const & ray, int depth, int maxDepth, int nbRandomRay)
		{
			RGBColor result(0, 0, 0);

			const RayTriangleIntersection rayTriangle = intersectTriangle(ray);

			if(rayTriangle.triangle()->material()->emissiveColor() != (0, 0, 0))
				result = rayTriangle.triangle()->material()->emissiveColor();
			
			if(depth < maxDepth)
			{
				//return result + getDiffuseIntensity(ray, rayTriangle, depth, maxDepth);														
				//return result + getSpecularIntensity(ray, rayTriangle, depth, maxDepth);
				//return result + getDiffuseIntensity(ray, rayTriangle, depth, maxDepth) + getSpecularIntensity(ray, rayTriangle, depth, maxDepth);
				return result + getIlluminationGlobaleDiffuseIntensity(ray, rayTriangle, depth, maxDepth, nbRandomRay) + getIlluminationGlobaleSpecularIntensity(ray, rayTriangle, depth, maxDepth, nbRandomRay);
			}
			else
			{
				return result;
			}
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	RGBColor getDiffuseIntensity(Ray const & ray, RayTriangleIntersection const & rayTriangle, int & depth, int & maxDepth)
		///
		/// \brief	
		/// 		
		/// \author	L. Foucault & V. Goupoil, Université de Rennes 1
		/// \date	13/11/2015
		///
		/// \param	ray						Le rayon.
		/// \param	rayTriangle				intersction entre le rayon et le triangle.
		///	\param	depth					profondeur conrante du rebond.
		///	\param	maxDepth				profondeur maximale des rebonds.
		///
		/// \return	La composante diffuse du point d'intersection.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RGBColor getDiffuseIntensity(Ray const & ray, RayTriangleIntersection const & rayTriangle, int & depth, int & maxDepth)
		{
			RGBColor diffuseColor(0, 0, 0);
			RGBColor shadow(0, 0,0);
			
			const Triangle *triangle = rayTriangle.triangle();
			float profondeur = rayTriangle.tRayValue();

			RGBColor couleurTriangle = triangle->material()->diffuseColor();
			
			if(couleurTriangle != 0)
			{
				for(int i = 0; i < m_lights.size(); i++)
				{
					// Calcul du point d'intersection entre le triangle et la source
					Math::Vector3 positionP = ray.source() + ray.direction() * profondeur;

					// Calcul de l'intensité de la source
					RGBColor Isource = m_lights[i].color();

					// Calcul du rayon L = lumière - point d'intersection / || lumière - point d'intersection ||
					Math::Vector3 rayonIncident = (m_lights[i].position() - positionP) / (m_lights[i].position() - positionP).norm();

					float cos = triangle->normal() * rayonIncident;		// Calcul des cosinus entre la normal et le rayon L

					if (rayonIncident * triangle->normal() < 0)			// Retourne la direction de la normal au plan si elle est du mauvais côté
						cos = cos * (-1);								// Inversion du cosinus	

					// Calcul de la distance entre la source et le point d'intersection
					float dsource = (m_lights[i].position() - positionP).norm();

					// Ajout des ombres
					Ray shadowRay(m_lights[i].position(), rayonIncident*(-1));
					const RayTriangleIntersection rayTriangleShadow = intersectTriangle(shadowRay);
					
					// Calcul de l'indice de refraction du materiau touché par le rayon
					float indiceRefraction = triangle->material()->indiceRefraction();
					
					if(indiceRefraction != 0.0f)
					{
						Math::Vector3 dirRefraction = triangle->refractionDirection(ray);
						diffuseColor = diffuseColor + getRefractionId(indiceRefraction, positionP, dirRefraction, depth, maxDepth);
					}
					// Si on traverse le même triangle que précedemment -> On retourne l'ombre
					else if (rayTriangleShadow.triangle() != triangle)
					{
						diffuseColor = diffuseColor + shadow;
					}
					else
					{
						// Calcul de la composante diffuse global : somme de toutes les composantes diffuses des sources lumineuses
						diffuseColor = diffuseColor + (Isource * couleurTriangle * cos / dsource);
					}
				}
			}

			return diffuseColor;
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	RGBColor getSpecularIntensity(Ray const & ray,RayTriangleIntersection const & rayTriangle, int & depth, int & maxDepth)
		///
		/// \brief	
		/// 		
		/// \author	L. Foucault & V. Goupoil, Université de Rennes 1
		/// \date	24/11/2015
		///
		/// \param	ray						Le rayon.
		/// \param	rayTriangle				intersction entre le rayon et le triangle.
		///	\param	depth					profondeur conrante du rebond
		///	\param	maxDepth				profondeur maximale des rebonds
		///
		/// \return	La composante speculaire du point d'intersection.
		////////////////////////////////////////////////////////////////////////////////////////////////////		
		RGBColor getSpecularIntensity(Ray const & ray, RayTriangleIntersection const & rayTriangle, int & depth, int & maxDepth)
		{
			RGBColor speculaireColor(0, 0, 0);
			RGBColor shadow = 0;
			
			const Triangle *triangle = rayTriangle.triangle();
			float profondeur = rayTriangle.tRayValue();
			int E = triangle->material()->specularExponent();

			RGBColor couleurTriangle = triangle->material()->specularColor();

			if (couleurTriangle != 0)
			{
				for (int i = 0; i < m_lights.size(); i++)
				{
					// Calcul du point d'intersection entre le triangle et la source
					Math::Vector3 positionP = ray.source() + ray.direction() * profondeur;

					// Calcul de l'intensité de la source
					RGBColor Isource = m_lights[i].color();

					// Calcul du rayon L = lumière - point d'intersection / || lumière - point d'intersection ||
					Math::Vector3 rayonIncident = (m_lights[i].position() - positionP) / (m_lights[i].position() - positionP).norm();

					float cos = (ray.direction()*(-1)) * (triangle->reflectionDirection(rayonIncident));		// Calcul des cosinus entre la normal et le rayon L

					if (rayonIncident * triangle->normal() < 0)			// Retourne la direction de la normal au plan si elle est du mauvais côté
						cos = cos * -1;									// Inversion du cosinus

					// Calcul de la distance entre la source et le point d'intersection
					float dsource = (m_lights[i].position() - positionP).norm();

					// Ajout des ombres
					Ray shadowRay(m_lights[i].position(), rayonIncident*(-1));
					const RayTriangleIntersection  rayTriangleShadow = intersectTriangle(shadowRay);

					// Calcul de l'indice de refraction du materiau touché par le rayon
					float indiceRefraction = triangle->material()->indiceRefraction();
					
					if(indiceRefraction != 0.0f)
					{
						Math::Vector3 dirRefraction = triangle->refractionDirection(ray);
						speculaireColor = speculaireColor + getRefractionId(indiceRefraction, positionP, dirRefraction, depth, maxDepth);
					}
					// Si on traverse le même triangle que précedemment -> On retourne l'ombre
					else if (rayTriangleShadow.triangle() != triangle)
					{
						speculaireColor = speculaireColor + shadow;
					}
					else
					{
						// Calcul la composante speculaire parfaite de la surface	
						Ray rayIdealSpeculaire((rayTriangle.intersection()), (rayTriangle.triangle()->reflectionDirection(rayTriangle.ray()->direction())));
						// Calcul de la composante speculaire global : somme de toutes les composantes speculaire des sources lumineuses
						speculaireColor = speculaireColor + (Isource * couleurTriangle * (pow(cos, E)) / dsource) + sendRay(rayIdealSpeculaire, depth + 1, maxDepth, 0);
					}
				}
			}
			return speculaireColor;

		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	RGBColor getIlluminationGlobaleDiffuseIntensity(Ray const & ray, RayTriangleIntersection const & rayTriangle, int depth, int maxDepth, int nbRandomRay)
		///
		/// \brief	
		/// 		
		/// \author	L. Foucault & V. Goupoil, Université de Rennes 1
		/// \date	08/01/2016
		///
		/// \param	ray						Le rayon.
		/// \param	rayTriangle				intersection entre le rayon et le triangle.
		///	\param	depth					profondeur conrante du rebond.
		///	\param	maxDepth				profondeur maximale des rebonds.
		///	\param	nbRandomRay				nombre de rayon aléatoire à lancer.
		///
		/// \return	La composante speculaire ideal du point d'intersection.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RGBColor getIlluminationGlobaleDiffuseIntensity(Ray const & ray, RayTriangleIntersection const & rayTriangle, int depth, int maxDepth, int nbRandomRay)
		{
			RGBColor emissiveDiffus(0, 0, 0);				// Définition de la composante speculaire à retourner 
			RGBColor shadow(0, 0, 0);						// Définition de la composante d'ombre
			
			const Triangle *triangle = rayTriangle.triangle();			// Triangle intersecté par le rayon 
			float profondeur = rayTriangle.tRayValue();					// Profondeur entre la source et le point d'intersection

			Math::Vector3 positionP = ray.source() + ray.direction() * profondeur;		// Calcul du point d'intersection entre le triangle et la source

			RGBColor couleurTriangle = triangle->material()->diffuseColor();	// Composante diffuse du materiau touché par le rayon 

			if (couleurTriangle != 0)									// Si la composante diffuse du triangle touché n'est pas nulle on procède au calcul
			{
				Math::Vector3 normal = triangle->normal();				// 

				if (triangle->reflectionDirection(ray) * normal < 0)	// 
					normal = -normal;									// 

				Math::RandomDirection randomRay(normal);				// Création d'une direction aléatoire 

				for (int i = 0; i < nbRandomRay; i++)					// Pour chaque rayon aléatoire lancé
				{
					Ray reflectedRay(positionP, randomRay.generate());		// Création du rayon à direction aléatoire en question

					const RayTriangleIntersection rayTriangleEmissive = intersectTriangle(reflectedRay);	// Obtention de l'intersection entre le rayon aléatoire et un triangle
					const Triangle *triangleEmissive = rayTriangleEmissive.triangle();						// Obtention du triangle touché par le rayon aléatoire

					RGBColor Isource = emissiveDiffus + sendRay(reflectedRay, depth + 1, maxDepth, nbRandomRay);		// Envoie du rayon aléatoire dans la scène et récupération de la couleur du triangle

					float profondeurEmissive = rayTriangleEmissive.tRayValue();								// Profondeur entre le point d'intersection et le triangle (source lumineuse)

					Math::Vector3 positionPEmissive = reflectedRay.source() + reflectedRay.direction() * profondeurEmissive;		// Calcul du point d'intersection entre le point d'intersection et le triangle (source lumineuse)

					Math::Vector3 rayonIncident = (positionPEmissive - positionP) / (positionPEmissive - positionP).norm();			// Calcul du rayon L = lumière - point d'intersection / || lumière - point d'intersection ||

					float cos = triangle->normal() * rayonIncident;		// Calcul des cosinus entre la normal et le rayon L

					if (rayonIncident * triangle->normal() < 0)			// Retourne la direction de la normal au plan si elle est du mauvais côté
						cos = cos * (-1);								// Inversion du cosinus		

					float dsource = (positionPEmissive - positionP).norm();								// Calcul de la distance entre la source et le point d'intersection
					RGBColor Id_source = ((Isource * couleurTriangle * cos) / dsource) / nbRandomRay;	// Calcul des composantes diffuses de la source lumineuse
						
					emissiveDiffus = emissiveDiffus + Id_source;										// Calcul de la composante diffuse global : somme de toutes les composantes diffuses des sources lumineuses

				}
			}

			return emissiveDiffus;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	RGBColor getIlluminationGlobaleSpecularIntensity(Ray const & ray, RayTriangleIntersection const & rayTriangle, int depth, int maxDepth, int nbRandomRay)
		///
		/// \brief	
		/// 		
		/// \author	L. Foucault & V. Goupoil, Université de Rennes 1
		/// \date	08/01/2016
		///
		/// \param	ray						Le rayon.
		/// \param	rayTriangle				intersection entre le rayon et le triangle.
		///	\param	depth					profondeur conrante du rebond.
		///	\param	maxDepth				profondeur maximale des rebonds.
		///	\param	nbRandomRay				nombre de rayon aléatoire à lancer.
		///
		/// \return	La composante speculaire ideal du point d'intersection.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RGBColor getIlluminationGlobaleSpecularIntensity(Ray const & ray, RayTriangleIntersection const & rayTriangle, int depth, int maxDepth, int nbRandomRay)
		{
			RGBColor emissiveSpeculare(0, 0, 0);			// Définition de la composante speculaire à retourner 
			RGBColor shadow(0, 0, 0);						// Définition de la composante d'ombre
			
			const Triangle *triangle = rayTriangle.triangle();			// Triangle intersecté par le rayon 
			float profondeur = rayTriangle.tRayValue();					// Profondeur entre la source et le point d'intersection
			const float E = triangle->material()->specularExponent();	// Exposant caractérisant la composante speculaire

			RGBColor couleurTriangle = triangle->material()->specularColor();	// Composante speculaire du materiau touché par le rayon 

			if (couleurTriangle != 0)									// Si la composante speculaire du triangle touché n'est pas nulle on procède au calcul
			{
				Math::RandomDirection randomRay(triangle->reflectionDirection(ray), E);		// Création d'une direction aléatoire 

				for (int i = 0; i < nbRandomRay; i++)					// Pour chaque rayon aléatoire lancé
				{
					Math::Vector3 positionP = ray.source() + ray.direction() * profondeur;		// Calcul du point d'intersection entre le triangle et la source
					Ray reflectedRay(positionP, randomRay.generate());							// Création du rayon à direction aléatoire en question

					const RayTriangleIntersection rayTriangleEmissive = intersectTriangle(reflectedRay);	// Obtention de l'intersection entre le rayon aléatoire et un triangle
					const Triangle *triangleEmissive = rayTriangleEmissive.triangle();						// Obtention du triangle touché par le rayon aléatoire

					RGBColor Isource = emissiveSpeculare + sendRay(reflectedRay, depth + 1, maxDepth, nbRandomRay);		// Envoie du rayon aléatoire dans la scène et récupération de la couleur du triangle

					float profondeurEmissive = rayTriangleEmissive.tRayValue();								// Profondeur entre le point d'intersection et le triangle (source lumineuse)

					Math::Vector3 positionPEmissive = reflectedRay.source() + reflectedRay.direction() * profondeurEmissive;		// Calcul du point d'intersection entre le point d'intersection et le triangle (source lumineuse)

					Math::Vector3 rayonIncident = (positionPEmissive - positionP) / (positionPEmissive - positionP).norm();			// Calcul du rayon L = lumière - point d'intersection / || lumière - point d'intersection ||

					float cos = (ray.direction()*(-1)) * (triangle->reflectionDirection(rayonIncident));		// Calcul des cosinus entre la normal et le rayon L

					if (rayonIncident * triangle->normal() < 0)			// Retourne la direction de la normal au plan si elle est du mauvais côté
						cos = cos * -1;									// Inversion du cosinus		

					float dsource = (positionPEmissive - positionP).norm();											// Calcul de la distance entre la source et le point d'intersection
					RGBColor Id_source = ((Isource * couleurTriangle * (pow(cos, E))) / dsource) / nbRandomRay;		// Calcul des composantes speculaires de la source lumineuse
						
					emissiveSpeculare = emissiveSpeculare + Id_source;												// Calcul de la composante speculaire global : somme de toutes les composantes speculaires des sources lumineuses
				}
			}

			return emissiveSpeculare;
		}
	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	RGBColor getRefractionId(RGBColor composante, float indiceRefraction, Math::Vector3 & ositionP, Math::Vector3 & dir_refraction, int depth, int maxDepth)
		///
		/// \brief	
		/// 		
		/// \author	L. Foucault & V. Goupoil, Université de Rennes 1
		/// \date	08/01/2016
		///
		/// \param	indiceRefraction		Indice de refraction du materiau.
		///	\param	positionP				Position de l'intersection entre le rayon et le triangle.
		///	\param	dirRefraction			Direction du rayon après refraction.
		///	\param	depth					Profondeur conrante du rebond.
		///	\param	maxDepth				Profondeur maximale des rebonds.
		///
		/// \return	La composante issue de la refraction du point d'intersection.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RGBColor getRefractionId(float indiceRefraction, Math::Vector3 & positionP, Math::Vector3 & dirRefraction, int depth, int maxDepth)
		{
			RGBColor refraction = (0, 0, 0);
			Ray refractedRay(positionP, dirRefraction);

			refraction = sendRay(refractedRay, depth, maxDepth, 0);
			
			return refraction;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Scene::compute(int maxDepth)
		///
		/// \brief	Computes a rendering of the current scene, viewed by the camera.
		/// 		
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/12/2013
		///
		/// \param	maxDepth	The maximum recursive depth.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void compute(int maxDepth, int nbRandomRay)
		{
			// Number of samples per axis for one pixel. Number of samples per pixels = subPixelSubdivision^2
			int subPixelDivision =  1; //50; //100;
			// Step on x and y for subpixel sampling
			float step = 1.0/subPixelDivision;
			// Table accumulating values computed per pixel (enable rendering of each pass)
			::std::vector<::std::vector<::std::pair<int, RGBColor> > > pixelTable(m_visu->width(), ::std::vector<::std::pair<int, RGBColor> >(m_visu->width(), ::std::make_pair(0, RGBColor())));

			// 1 - Rendering time
			LARGE_INTEGER frequency;        // ticks per second
			LARGE_INTEGER t1, t2;           // ticks
			double elapsedTime;
			// get ticks per second
			QueryPerformanceFrequency(&frequency);
			// start timer
			QueryPerformanceCounter(&t1);
			// Rendering pass number
			int pass = 0;
			// Rendering
			for(float xp=-0.5 ; xp<0.5 ; xp+=step)
			{
				for(float yp=-0.5 ; yp<0.5 ; yp+=step)
				{
					::std::cout<<"Pass: "<<pass<<::std::endl;
					++pass ;
					// Sends primary rays for each pixel (uncomment the pragma to parallelize rendering)
#pragma omp parallel for //schedule(dynamic)
					for(int y=0 ; y<m_visu->height() ; y++)
					{
						for(int x=0 ; x<m_visu->width() ; x++)
						{
							// Ray casting
							RGBColor result = sendRay(m_camera.getRay(((float)x+xp)/m_visu->width(), ((float)y+yp)/m_visu->height()), 0, maxDepth, nbRandomRay);
							// Accumulation of ray casting result in the associated pixel
							::std::pair<int, RGBColor> & currentPixel = pixelTable[x][y];
							currentPixel.first++;
							currentPixel.second = currentPixel.second + result;
							// Pixel rendering (simple tone mapping)
							m_visu->plot(x,y,pixelTable[x][y].second/pixelTable[x][y].first);
							// Updates the rendering context (per pixel)
							//m_visu->update();
						}
						// Updates the rendering context (per line)
						m_visu->update();
					}
					// Updates the rendering context (per pass)
					m_visu->update();
				}
			}
			// stop timer
			QueryPerformanceCounter(&t2);
			elapsedTime = (t2.QuadPart - t1.QuadPart) / frequency.QuadPart;
			::std::cout<<"time: "<<elapsedTime<<"s. "<<::std::endl;
		}
	};
}

#endif
