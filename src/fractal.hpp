struct Scene
{
    int Iterations = 10;
    float Bailout = 3;
    float Power = 4;
};

Scene defautlScene(){
    Scene scene;

    return scene;
}

float map(float val, float mins, float maxs, float mine, float maxe){
    return((val-mins)/(maxs-mins) * (maxe-mine) + mine );
}

void animate(Scene& scene, float t){
    scene.Power = map(t,0,1,1,4);
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
    glm::vec3 z = point;
	float dr = 1.0;
	float r = 0.0;

    int Iterations = scene.Iterations;
    float Bailout = scene.Bailout;
    float Power = scene.Power;


	for (int i = 0; i < Iterations ; i++) {
		r = length(z);
		if (r>Bailout) break;
		
		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = glm::atan(z.y,z.x);
		dr =  pow( r, Power-1.0)*Power*dr + 1.0;
		
		// scale and rotate the point
		float zr = pow( r,Power);
		theta = theta*Power;
		phi = phi*Power;
		
		// convert back to cartesian coordinates
		z = zr*glm::vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=point;
	}
	return 0.5*log(r)*r/dr;
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