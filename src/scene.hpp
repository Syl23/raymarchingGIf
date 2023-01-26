struct Scene
{
    glm::vec3 cubePos;
    glm::vec3 cubeVec;
    glm::vec3 cubeRotation;

    glm::vec3 spherePos;
    float sphereSize;

    // std::vector<std::pair<float,        glm::vec3>> spheres;
    // std::vector<std::pair<glm::vec3, glm::vec3>>  animations;
};

Scene defautlScene(){
    Scene scene;

    scene.cubePos = glm::vec3(0.0f,0.0f,3.0f);
    scene.cubeVec = glm::vec3(0.5f,0.5f,0.5f);

    scene.spherePos  = glm::vec3(0.0f,0.0f,3.0f);
    scene.sphereSize = 1.0f;

    return scene;
}

float map(float val, float mins, float maxs, float mine, float maxe){
    return((val-mins)/(maxs-mins) * (maxe-mine) + mine );
}

void animate(Scene& scene, float t){
    float st = std::sin(map(t,0,1,0,2*3.141592)); 
    scene.sphereSize = map(st,-1,1,scene.cubeVec[0],glm::length(scene.cubeVec));

    float cYang = map(t,0,1,0,4*3.141592);
    scene.cubeRotation.y = cYang;

    float cXang = map(t,0,1,0,2*3.141592);
    scene.cubeRotation.x = cXang;
}

float smoothMin(float a, float b, float k) {
  float h = std::max(k - std::abs(a - b), 0.0f) / k;
  return std::min(a, b) - h * h * h * k * 1.0f / 6.0f;
}

float distanceCube(const glm::vec3& point, const glm::vec3& cubeCenter, const glm::vec3& cubeSize, const glm::vec3& rotation) {
    glm::mat4 rotationMatrix = glm::eulerAngleZYX(rotation.z, rotation.y, rotation.x);
    glm::vec3 d = glm::vec3(glm::abs(rotationMatrix*glm::vec4(point - cubeCenter,1.0))) - cubeSize;
    return glm::length(glm::max(d, 0.0f));
}

float dist(const Scene& scene, const glm::vec3& point)
{
    float sphereDistance = glm::length(point - scene.spherePos) - scene.sphereSize;
    float cubeDistance = distanceCube(point, scene.cubePos, scene.cubeVec, scene.cubeRotation);

    //return std::max(-sphereDistance, cubeDistance);
    //return std::min(sphereDistance, cubeDistance);
    return smoothMin(sphereDistance, cubeDistance, 0.5);
}

glm::vec3 normale(const Scene& scene, const glm::vec3& point)
{
    float epsilon = 0.001f; // Précision pour la normale

    // Calcul de la dérivée partielle de la distance par rapport à x, y, z
    float distance = dist(scene, point);
    
    float dx = dist(scene, point + glm::vec3(epsilon, 0.0f, 0.0f)) - distance;
    float dy = dist(scene, point + glm::vec3(0.0f, epsilon, 0.0f)) - distance;
    float dz = dist(scene, point + glm::vec3(0.0f, 0.0f, epsilon)) - distance;

    return glm::normalize(glm::vec3(dx, dy, dz));   
}