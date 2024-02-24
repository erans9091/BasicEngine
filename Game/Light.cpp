#include "Light.h"
#include "Color.h"
#include "ObjectShape.h"
#include "Ray.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
Point Light::cameraPosition = Point();
 
	Light::Light(float r, float g, float b, float a) : intensity(r,g,b,a) {

	}

	Color Light::getIntensity() const {
		return intensity;
	}

	

	Ray Ambient::occlusionRay(Point& intersectionWithObject) { // this will be of no use anyway - but have to be implemented because it's a pure virtual func
		return Ray(Vector(0, 0, 0), intersectionWithObject);
	}

	//Ambient::Ambient(float r, float g, float b, float a) : Light(r, g, b, a) {	};

	//Ambient::~Ambient() {};




	Color Ambient::CalcColorAtPoint( Point& p, Vector& normalAtPoint, ObjectShape* shape) {
		return shape->ambient * intensity; // Iambient * Kambient
	}

	Ambient::~Ambient() {}


	//NOTE: the direction vectors for light sources can be non-normalized so normalized them after initialization of this members

	Directional::Directional(float r, float g, float b, float a, Vector lightDir) : Light(r, g, b, a), lightDirection(lightDir) { lightDirection.NormalizeVec(); };
	Directional::~Directional() {};

	Spotlight::Spotlight(float r, float g, float b, float a, Vector lightDir, Point lightIncidenceP, float cosCutoffAngle) : Light(r, g, b, a), lightDirection(lightDir), lightIncidencePoint(lightIncidenceP) ,cosCutoffAngle(cosCutoffAngle)
	{ 
		lightDirection.NormalizeVec(); 
	};
	Spotlight::~Spotlight() {};

	Color Directional::CalcColorAtPoint(Point& p, Vector & normalAtPoint, ObjectShape* shape) {
		Vector negatedDirection = this->lightDirection;
		float angleLightDirAndNormal = negatedDirection * normalAtPoint;
		angleLightDirAndNormal = angleLightDirAndNormal; // N * L    clamp to zero if surface is facing away from the light
		angleLightDirAndNormal = fmaxf(angleLightDirAndNormal, 0.0); // N * L    clamp to zero if surface is facing away from the light
		//angleLightDirAndNormal = fabsf(angleLightDirAndNormal);
		Color lightIntensityAtPoint = this->intensity;//intensity is constant for directional light!


		Vector reflectionVector = normalAtPoint.operator*(2.0).operator*(negatedDirection.operator*(normalAtPoint)) - negatedDirection; //2*N * (N * L) -L , where in our case L= -lightDirection
		reflectionVector.NormalizeVec(); //R
		Vector intersectionToViewerVec = cameraPosition - p; //V
		intersectionToViewerVec.NormalizeVec();

		int isNormalFacingAwayFromLightDir = angleLightDirAndNormal > 0 ? 1 : 0;

		float angleReflectVecToViewerVec = reflectionVector * intersectionToViewerVec; // V*R
		//angleReflectVecToViewerVec = fmaxf(angleReflectVecToViewerVec, 0.0); //clamp to zero if angle greater than a right angle
		//angleReflectVecToViewerVec = fabsf(angleReflectVecToViewerVec); //c
		Color diffuseReflectionIntensity = lightIntensityAtPoint.Modulate(angleLightDirAndNormal); // I * (N * L)
		float specCoeff = powf(angleReflectVecToViewerVec, shape->ambient.getA()); // specular intensity coefficient =  (V * R) ^ n ,where n is the 
		Color specularReflectionIntensity = lightIntensityAtPoint.Modulate(specCoeff).Modulate(isNormalFacingAwayFromLightDir);
		//Color specularReflectionIntensity = lightIntensityAtPoint.Modulate(specCoeff);
		//debug
		diffuseReflectionIntensity = diffuseReflectionIntensity * shape->getDiffuse(p);
		specularReflectionIntensity = specularReflectionIntensity * shape->specular;
		

		return diffuseReflectionIntensity + specularReflectionIntensity ; // check where to derive the coefficenits : constant , linear, quadratic
	}

	Ray Directional::occlusionRay(Point& intersectionWithObject) {
		return Ray(this->lightDirection.Negate(), intersectionWithObject); // the occlusion ray is the ray that starts from the intersection point , and goes in the opposite direction of the light direction vector
	}
		


	Color Spotlight::CalcColorAtPoint(Point & p, Vector & normalAtPoint, ObjectShape* shape ) {
		Vector negatedDirection = this->lightDirection;
		Vector vecFromPixelToLight = p - lightIncidencePoint    ; // L=P-Q , where Q is the intersection point
		vecFromPixelToLight.NormalizeVec(); //normalized 

		Vector reflectionVector = ((normalAtPoint * 2.0) * (vecFromPixelToLight * normalAtPoint)) - vecFromPixelToLight; // 2* N * (N * L) - L
		//Vector reflectionVector = normalAtPoint.operator*(2.0).operator*(vecFromPixelToLight.operator*(normalAtPoint)) - vecFromPixelToLight; // R
		reflectionVector.NormalizeVec();//normalized 
		Vector intersectionToViewerVec = cameraPosition - p ; // V
		intersectionToViewerVec.NormalizeVec();  //normalized 


		//float angleLightDirToLightVector = negatedDirection * vecFromPixelToLight; // -R * L
		
		//float angleLightDirToLightVector = negatedDirection * vecFromPixelToLight;
		float angleLightDirToLightVector = fmaxf(0.0, negatedDirection * vecFromPixelToLight); // -R * L
		float angleNormalAndToLightVec = vecFromPixelToLight * normalAtPoint; //N * L
		angleNormalAndToLightVec = fmaxf(0.0, angleNormalAndToLightVec);

		if (angleNormalAndToLightVec >0 && angleLightDirToLightVector > cosCutoffAngle) {
			std::cout << "fml";
		}

		//angleNormalAndToLightVec = fmaxf(0.0, angleNormalAndToLightVec); //clamp to zero if normal direction is facing away from the light direction

		int isNormalFacingAwayFromLight = angleNormalAndToLightVec > 0 ? 1 : 0; //bool indicator


		//float lightRatio = fmaxf(0.0, angleLightDirToLightVector); //clamp to zero if the dot product is negative - i.e object facing away from the light
		float lightRatio = angleLightDirToLightVector > cosCutoffAngle ? angleLightDirToLightVector : 0.0; //light dosent reach the point of the angle between to light vector and prefered light direction is greater than cutoff angle!
		//float lightRatio = fabsf (angleLightDirToLightVector); //clamp to zero if the dot product is negative - i.e object facing away from the light
		float angleReflectVecToViewerVec = reflectionVector * intersectionToViewerVec; // V * R
		angleReflectVecToViewerVec = fmaxf(0.0, angleReflectVecToViewerVec);
		//angleReflectVecToViewerVec = fabsf(angleReflectVecToViewerVec);

		Color lightIntensityAtPoint = intensity.Modulate(lightRatio);
		Color diffuseReflectionIntensity = lightIntensityAtPoint.Modulate(angleNormalAndToLightVec);
		float specCoeff = powf(angleReflectVecToViewerVec, shape->diffuse.getA());
		Color specularReflectionIntensity = lightIntensityAtPoint.Modulate(specCoeff).Modulate(isNormalFacingAwayFromLight); //the last modulation clamps to zero if normal is facing away from the light direction
		//Color specularReflectionIntensity = lightIntensityAtPoint.Modulate(powf(angleReflectVecToViewerVec  ,shape->diffuse.getA()));
		diffuseReflectionIntensity = diffuseReflectionIntensity * shape->getDiffuse(p);
		specularReflectionIntensity = specularReflectionIntensity * shape->specular;

		return diffuseReflectionIntensity + specularReflectionIntensity; //including the diffuse component reflection
	}
	
	Ray Spotlight::occlusionRay(Point& intersectionWithObject) {
		return Ray(this->lightIncidencePoint - intersectionWithObject, intersectionWithObject); //the occlusion ray base point is the light incident point, and the vector with direction from light point to intersection point
	}
