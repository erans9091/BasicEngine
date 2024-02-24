#include "RayTracer.h"
#include "Color.h"
#include "Ray.h"
#include "Vector.h"
#include "Light.h"
#include "ObjectShape.h"
#include <fstream>
#include <sstream>
#include <iostream>
#define MAXRECURSIONLEVEL 5
#define MAXDIST 1000
#define MAXiNTENSITY 255
#define COLCHANNELSNUM 4


void RayTracer::RayTrace() {

    for (int y = 0; y < height; y++) {
        int colChannsOutOfRange = 0;

        for (int x = 0; x < width; x++) {
            float pixelCoordX = topLeft.getX() + (xDelta * x) + (xDelta / 2); //the xDelta / 2 term is written in order to shoot the ray from the CENTER OF THE PIXEL
            float pixelCoordY =topLeft.getY() - ( (yDelta * y) + (yDelta / 2));//the yDelta / 2 term is written in order to shoot the ray from the CENTER OF THE PIXEL
            Point currPixelPoint = Point(pixelCoordX, pixelCoordY, 0.0); // maybe add a member - z which corresponds to the z value the screen plane is laid upon
            Vector incomingRayDirection = currPixelPoint - cam.getP(); //directed from camera pos to screen coord
            Ray rayThroughScreen = Ray(incomingRayDirection, cam.getP());
            Color finalPixelColor = RecursiveRayTrace(rayThroughScreen, 1);
            finalPixelColor.ClampColor();
            unsigned char color_data[4] = { (int)(finalPixelColor.getR() * MAXiNTENSITY) , (int)(finalPixelColor.getG() * MAXiNTENSITY) , (int)(finalPixelColor.getB() * MAXiNTENSITY), (int)(finalPixelColor.getA() * MAXiNTENSITY) };


            /**/
            if (finalPixelColor.getR() > 1 || finalPixelColor.getR() < 0) {
                std::cout<< "pixel: x= " << x << " y= " << y << " R channel out of range! = " << finalPixelColor.getR() << " clamped: " << (int) color_data[0] << std::endl;
            }
            if (finalPixelColor.getG() > 1 || finalPixelColor.getG() < 0) {
                std::cout << "pixel: x= " << x << " y= " << y <<  "G channel out of range! =" << finalPixelColor.getG() << " clamped: " <<(int) color_data[1] << std::endl;
            }
            if (finalPixelColor.getB() > 1 || finalPixelColor.getB() < 0) {
                std::cout << "pixel: x= " << x << " y= " << y  << "B channel out of range! =" << finalPixelColor.getB() << " clamped: " <<(int) color_data[2] << std::endl;
            }
            //if (finalPixelColor.getR() > 0 || finalPixelColor.getR() < 0) {
             //   std::cout << "R channel out of range!" << finalPixelColor.getA() << std::endl;
            //}





          
          //  int* coldat = (int *)color_data;
          //  std::cout << "Current location in screen coords is: x=" << pixelCoordX << " y= " << pixelCoordY << " " << std::endl;
          //  std::cout << "for current pixel: r=" << (int)(finalPixelColor.getR() * MAXiNTENSITY) << "g =" << (int)(finalPixelColor.getG() * MAXiNTENSITY) << " b=" << (int)(finalPixelColor.getB() * MAXiNTENSITY) << " a=" << (int)(finalPixelColor.getA() * MAXiNTENSITY) << std::endl;
          // std::cout << "for current pixel normalized: r=" <<finalPixelColor.getR() << "g =" << finalPixelColor.getG()  << " b=" << finalPixelColor.getB() << " a=" <<finalPixelColor.getA() << std::endl;
// 
           // std::cout << "for pixel: (x,y)current color in rgba is: " << *coldat << " " << std::endl;

          /*
            for (int i = 0; i < COLCHANNELSNUM; i++) {
                
                if (color_data[i] > 255) {
                    color_data[i] = 255;
                }
                else if (color_data[i] < 0) {
                    color_data[i] = 0;
                }
            }
          */

            int lineLengthBytes = width * COLCHANNELSNUM;
            texture_data[(y * lineLengthBytes) + (x * COLCHANNELSNUM)]= color_data[0];
            texture_data[ (y * lineLengthBytes) + (x * COLCHANNELSNUM) +1] = color_data[1];
            texture_data[(y * lineLengthBytes) + (x * COLCHANNELSNUM) + 2] = color_data[2];
            texture_data[(y * lineLengthBytes) + (x * COLCHANNELSNUM) +3] = 255; 
            
        }
    }
    

}

unsigned char* RayTracer::getTextureData() {
    return this->texture_data;
}

Color RayTracer::RecursiveRayTrace(Ray incomingRay, int recursionLevel) {
    intersectionData  * intersectData = SceneIntersectionWithRay(incomingRay);
    Color finaColor = Color(0, 0, 0, 0); // INITIAL AMBIENT COLOR OF THE SCENE
    if (intersectData) {
       Point intersectionP = intersectData->interP;
       ObjectShape* intersectedShape = objects[intersectData->shapeIntersectedIdx];
       finaColor= finaColor + sceneLights[0]->CalcColorAtPoint(intersectionP, intersectedShape->normalAtPoint(intersectionP), intersectedShape); //AMBIENT!!
       for (int i = 1; i < this->sceneLights.size(); i++) { // 
           Color lightFromLightSource = sceneLights[i]->CalcColorAtPoint(intersectionP, intersectedShape->normalAtPoint(intersectionP), intersectedShape);
           // construct the intersection ray and check if in shadow! - do this for all lights that are not ambient lights of the scene
           // I.E NON-AMBIENT LIGHT SOURCE
               Ray shadowRay = sceneLights[i]->occlusionRay(intersectionP);
               int shadowCoeff = InShadow(shadowRay, intersectData->shapeIntersectedIdx);
               if (!shadowCoeff) {
                //   std::cout << "in shadow!" << std::endl;
               }
               lightFromLightSource = lightFromLightSource.Modulate(float(shadowCoeff));
           
           finaColor = finaColor + lightFromLightSource;
       }
       delete intersectData; //clean alloc mem for struct
    }
    else { //  dosen't intersect any object - return the ambient color of the scene!
        return this->sceneLights[0]->getIntensity();
    }

    return finaColor;
    //HERE THE RECURSIVE PART SHOULD BE
}

intersectionData * RayTracer::SceneIntersectionWithRay(Ray ray) {
    intersectionData * interData=nullptr;
    int intersectedShapeIdx = - 1;
    float minDist=MAXDIST;
    Point finalInterPoint;
    int didIntersect = 0; //boolean value - is there any intersection of an object and the ray

    for (int i = 0; i < this->objects.size(); i++) { // check if ray intersects with some object - pick the object closest to the ray base point
        std::vector<Point> currintersections = objects[i]->intersectionWithRay(ray);
        if (currintersections.size() > 0) { //if intersected with object
            didIntersect = 1;
            Point p = currintersections[0];
            float currDist = (ray.basePoint - p).Norm();
            if (currDist < minDist) {
                finalInterPoint = p;
                minDist = currDist;
                intersectedShapeIdx = i;
            }
          
        }
    }

    if (didIntersect) {
        interData = new intersectionData;
        interData->interP = finalInterPoint;
        interData->shapeIntersectedIdx = intersectedShapeIdx;
    }

    return interData;
}


int RayTracer::InShadow(Ray intersectionRay, int intersectedShapeIdx) { // return 0 if in shadow - 1 otherwise!
    int shadow = 1;

    for (int i = 0; i < objects.size(); i++) {
        if (i != intersectedShapeIdx) {
            std::vector<Point> currIntersections = objects[i]->intersectionWithRay(intersectionRay);
            if (currIntersections.size() > 0) {
                shadow = 0;
                break;
            }
        }
      //  std::cout << "in shadow= " << shadow << std::endl;
    
    }
    return shadow;
}




std::vector<str> splitString(const std::string& input, char delimiter) {
    std::vector<str> tokens;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
RayTracer::RayTracer(std::string path) : topLeft(Point(-1.0, 1.0,0.0)) , bottomRight(Point(1.0, -1.0, 0.0)) , width(256) , height(256)
{   
    float widthInScreenPlane = bottomRight.getX() - topLeft.getX();
    float heightInScreenPlan = topLeft.getY() - bottomRight.getY();
    this->xDelta = widthInScreenPlane / ( (float)width);
    this->yDelta = heightInScreenPlan / ((float)height);
    this->texture_data = new unsigned char[width * height * COLCHANNELSNUM];



    std::ifstream file(path);
    std::vector<str> lines;
    std::vector<str> spl;
    std::vector<float> intens, spot;
    Color c(0, 0, 0, 0);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open the file." << std::endl;
    }
    for (int i = 0; i < lines.size(); i++)
    {
        spl = splitString(lines[i], ' ');
        float par[4] = { std::stof(spl[1]),std::stof(spl[2]),std::stof(spl[3]),std::stof(spl[4]) };

        //debug
        std::cout << "current letter procs in main loop: " << lines[i][0] << std::endl;

        if (lines[i][0] == 'e')//camera
        {
            this->cam = Eye(Point(par[0], par[1], par[2]), par[3]);
            Light::cameraPosition = cam.getP();
        }
        else if (lines[i][0] == 'a')//ambient light (R,G,B,A)
        {
            this->sceneLights.push_back(new Ambient(par[0], par[1], par[2], par[3]));
        }
        else if (lines[i][0] == 'o' || lines[i][0] == 't' || lines[i][0] == 'r')//solid object r>0=sphere
        {
            c = popColor(lines, i);
            if (par[3] > 0)//sphere
            {
                this->objects.push_back(new Sphere(c, c, Color((float)0.7, (float)0.7, (float)0.7, 1),
                    par[3], Point(par[0], par[1], par[2]), lines[i][0]));
            }
            else if (par[3] <= 0)
            {
                objects.push_back(new Plane(c, c, Color((float)0.7, (float)0.7, (float)0.7, 1),
                    par[3], Vector(par[0], par[1], par[2]), lines[i][0]));
            }
           
        }
        else if (lines[i][0] == 'd')//light direction
        {
            intens = popIntens(lines, i);
            if (par[3] == 0) {//directional light 
                this->sceneLights.push_back(new Directional(intens[0], intens[1],
                    intens[2], intens[3],
                    Vector(par[0], par[1], par[2])));
            }
            else if (par[3] == 1.0) {//spot light
                spot = popSpot(lines, i);
                this->sceneLights.push_back(new Spotlight(intens[0], intens[1],
                    intens[2], intens[3],
                    Vector(par[0], par[1], par[2]),
                    Point(spot[0], spot[1], spot[2]) , spot[3]) );
            }

        }
    }
}
    
std::vector<float> RayTracer::popIntens(std::vector<str>& v, int idx)
{
    return popByLtr(v, idx, 'i');
}

std::vector<float> RayTracer::popSpot(std::vector<str>& v, int idx)
{
    return popByLtr(v, idx, 'p');
}

Color RayTracer::popColor(std::vector<str>& v, int idx)
{
    auto t = popByLtr(v, idx, 'c');
    return Color(t[0], t[1], t[2], t[3]);
}

std::vector<float> RayTracer::popByLtr(std::vector<str>& v, int idx, char l)
{
    std::vector<float> res;
    for (int i = idx; i < v.size(); i++)
    {
        if (v[i][0] == l)
        {
            std::cout << "current letter procs: " << v[i][0] << std::endl;
            auto spl = splitString(v[i], ' ');
            res.push_back(std::stof(spl[1]));
            res.push_back(std::stof(spl[2]));
            res.push_back(std::stof(spl[3]));
            res.push_back(std::stof(spl[4]));
            v.erase(v.begin() + i);
            break;
        }
    }
    return res;
}

