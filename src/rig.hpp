struct Scene
{
    std::vector<std::pair<float,glm::vec3>> bones; // length and orientation
    std::vector<std::pair<int, int>>  connection; // A is the father of B
    std::vector<std::pair<float,std::pair<glm::vec3,glm::vec3>>> segs;
};

Scene defautlScene(){
    Scene scene;

    scene.bones.push_back(std::make_pair(1.0,glm::vec3(0.0f,0.0f,0.0f)));
    scene.bones.push_back(std::make_pair(1.0,glm::vec3(0.0f,0.0f,0.0f)));
    scene.bones.push_back(std::make_pair(1.0,glm::vec3(0.0f,0.0f,0.0f)));
    scene.bones.push_back(std::make_pair(1.0,glm::vec3(0.0f,0.0f,0.0f)));
    scene.bones.push_back(std::make_pair(1.0,glm::vec3(0.0f,0.0f,0.0f)));
    scene.bones.push_back(std::make_pair(1.0,glm::vec3(0.0f,0.0f,0.0f)));

    scene.connection.push_back(std::make_pair(0,1));
    scene.connection.push_back(std::make_pair(0,2));
    scene.connection.push_back(std::make_pair(0,3));
    scene.connection.push_back(std::make_pair(3,4));
    scene.connection.push_back(std::make_pair(3,5));

    return scene;
}

float map(float val, float mins, float maxs, float mine, float maxe){
    return((val-mins)/(maxs-mins) * (maxe-mine) + mine );
}
float distanceToSegment(glm::vec3 point, glm::vec3 segmentStart, glm::vec3 segmentEnd) {
    glm::vec3 segment = segmentEnd - segmentStart;
    glm::vec3 pointToStart = point - segmentStart;
    float dotProduct = glm::dot(pointToStart, segment);
    if (dotProduct <= 0.0f) {
        // Le point se trouve avant le début du segment
        return glm::length(pointToStart);
    }

    float segmentLength = glm::length(segment);
    if (dotProduct >= segmentLength) {
        // Le point se trouve après la fin du segment
        return glm::length(point - segmentEnd);
    }

    // Le point se trouve sur le segment
    glm::vec3 projection = segmentStart + dotProduct / segmentLength * segment;
    return glm::length(point - projection);
}

void computeSegs(Scene & scene){
    float defSegsSize = 0.2;

    scene.segs.clear();

    auto origin = glm::vec3(0,-1,0);

    glm::mat4 rotationMatrix = glm::eulerAngleZYX(scene.bones[0].second.z, scene.bones[0].second.y, scene.bones[0].second.x);

    glm::vec3 A = origin;
    glm::vec3 B = glm::vec3(glm::vec4(A,1.0) + rotationMatrix * glm::vec4(glm::vec3(1,0,0) * scene.bones[0].first,1.0));

    scene.segs.push_back(std::make_pair(defSegsSize,std::make_pair(A,B)));

    for(int i = 0 ; i < scene.connection.size() ; i ++){
        glm::vec3 A = scene.segs[scene.connection[i].first].second.second;
        glm::mat4 rotationMatrix = glm::eulerAngleZYX(scene.bones[scene.connection[i].second].second.z, scene.bones[scene.connection[i].second].second.y, scene.bones[scene.connection[i].second].second.x);
        //glm::mat4 fatherRotationMatrix = glm::eulerAngleZYX(scene.bones[scene.connection[i].first].second.z, scene.bones[scene.connection[i].first].second.y, scene.bones[scene.connection[i].first].second.x);
        glm::vec3 B = glm::vec3(glm::vec4(A,1.0) + rotationMatrix * glm::vec4(glm::vec3(1,0,0) * scene.bones[scene.connection[i].second].first,1.0));
        scene.segs.push_back(std::make_pair(defSegsSize,std::make_pair(A,B)));
    }
}


float DE(glm::vec3 pos) {
	glm::vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;

    int Iterations = 10;
    float Bailout = 10;
    float Power = 4;


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
		z+=pos;
	}
	return 0.5*log(r)*r/dr;
}


void animate(Scene& scene, float t){
    float PI = 3.1415926535;

    float ang0 = map(sin(t*PI*2)  ,-1,1, PI/4        , PI   - PI/8);   //arms
    float ang1 = map(sin(2*t*PI*2),-1,1, PI/4 + PI/16, PI/4 - PI/16); //legs

    for(int i = 0 ; i < 6 ; i ++){
        scene.bones[i].second[2] = PI/2;
    }
    scene.bones[1].second[2] += ang0;
    scene.bones[2].second[2] += -ang0;

    scene.bones[4].second[2] += ang1;
    scene.bones[5].second[2] += -ang1;

    computeSegs(scene);
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
    return DE(point);
    auto dist = 999999.0f;

    for(int i = 0 ; i < scene.segs.size() ; i ++){
        auto s    = scene.segs[i].second;
        auto size = scene.segs[i].first;

        //dist = std::min(dist, distanceToSegment(point, s.first, s.second)-size);
        dist = smoothMin(dist, distanceToSegment(point, s.first, s.second)-size, 0.2);
    }

    return dist;
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