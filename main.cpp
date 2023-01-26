#include <iostream>
#include <vector>
#include <fstream>
#include <string> 

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

//#include "src/scene.hpp"
#include "src/rig.hpp"
#include "src/animation.hpp"
#include "src/tools.hpp"
#include "src/render.hpp"

int main()
{
    auto scene = defautlScene();

    int nbImg = 100;

    int width  = 500;
    int height = 500;

    int prog = 0;

    #pragma omp parallel for
    for(int i = 0 ; i < nbImg ; i++){
        float t = ((float)i) / nbImg;
        animate(scene,t);

        //glm::mat4 rotationMatrix = glm::eulerAngleZYX(0.0f,t*2*3.141592f,0.0f);

        glm::vec3 cameraPos = glm::vec3(0,0,-5);

        auto img = renderImage(scene, width,height,cameraPos);
        saveImageToPPM(img, width, height, "render/"+std::string(3 - std::to_string(i).length(), '0') + std::to_string(i)+".ppm");
        prog ++;
        affProgressBar(prog, 0, nbImg);
    }

    return 0;
}