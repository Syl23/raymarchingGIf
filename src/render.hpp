glm::vec3 rayMarch(Scene scene, glm::vec3 cameraPosition, glm::vec3 pixelVector, bool * intersect)
{
    int maxIteration = 100;
    float t = 0.0f; // Paramètre de temps pour le rayon
    float maxDistance = 100.0f; // Distance maximale pour le rayon
    float epsilon = 0.01f; // Précision pour le rayon
    *intersect = false;

    // Boucle de ray marching
    for (int i = 0; i < maxIteration ; i++)
    {
        glm::vec3 point = cameraPosition + t * pixelVector; // Point courant sur le rayon
        float distance = dist(scene, point); // Distance minimale au boules
        if (distance < epsilon)
        {
            *intersect = true;
            return point;
        }
        if (t > maxDistance)
        {
            return glm::vec3(0.0f);
        }
        t += distance;
    }
    return glm::vec3(0.0f);
}

glm::vec3 renderPixel(Scene scene, glm::vec3 cameraPosition, glm::vec3 pixelVector){
    glm::vec3 colorRes;
    glm::vec3 pos;
    bool intersect;

    pos = rayMarch(scene, cameraPosition, pixelVector, &intersect);
    if(intersect){
        colorRes = 0.5f*normale(scene, pos) + glm::vec3(0.5);
    }else{
        colorRes = glm::vec3(1,1,1);
    }

    return colorRes;
}

std::vector<glm::vec3> renderImage(Scene scene, int width, int height, glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f))
{
    auto res = std::vector<glm::vec3>();    

    // Boucle à travers tous les pixels de l'image
    for (int y = -height/2; y < height/2; y++)
    {
        for (int x = -width/2; x < width/2; x++)
        {
            // Coordonnées du pixel (x, y) dans l'image
            float u = (float)x / (float)width;
            float v = (float)y / (float)width;

            // Calcul du vecteur dirigé depuis la caméra vers le pixel (x, y)
            glm::vec3 pixelVector = glm::normalize(glm::vec3(u, v, 1.0f));

            res.push_back(renderPixel(scene, cameraPosition,pixelVector));
        }
    }

    return res;
}