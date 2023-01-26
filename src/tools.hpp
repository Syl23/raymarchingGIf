void saveImageToPPM(const std::vector<glm::vec3>& image, int width, int height, const std::string& filename)
{
    std::ofstream file(filename);

    // En-tête PPM
    file << "P3" << std::endl;
    file << width << " " << height << std::endl;
    file << "255" << std::endl;

    // Boucle à travers tous les pixels de l'image
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Récupération des composants rouge, vert et bleu du pixel
            glm::vec3 pixel = image[y * width + x];
            int r = glm::clamp((int)(pixel.r * 255.0f),0,255);
            int g = glm::clamp((int)(pixel.g * 255.0f),0,255);
            int b = glm::clamp((int)(pixel.b * 255.0f),0,255);

            // Écriture des composants rouge, vert et bleu dans le fichier PPM
            file << r << " " << g << " " << b << " ";
        }
        file << std::endl;
    }

    file.close();
}

void affProgressBar(int val, int min, int max) {
    int barWidth = 70;
    float progress = (float)(val - min) / (float)(max - min);
    int pos = (int)(barWidth * progress);

    if(val != min){
        std::cout << "\033[A\033[K";
    }

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\n";
    std::cout.flush();
}