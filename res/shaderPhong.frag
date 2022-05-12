varying vec3 surfaceNormal;  // Surface normal
varying vec3 vertexPosition;       // Vertex position

uniform vec3 ambientColour;
uniform vec3 diffuseColour;
uniform vec3 specularColour;

uniform vec3 lightPosition; // Light position

uniform float ambientAmount;   // Ambient reflection coefficient
uniform float diffuseAmount;   // Diffuse reflection coefficient
uniform float specularAmount;   // Specular reflection coefficient
uniform float amountOfShine; // Shininess



void main() {

  vec3 Norm = normalize(surfaceNormal);
  vec3 LightVector = normalize(lightPosition - vertexPosition);

  //Lambert's cosine law
  float specular = 0.0;
  float lamb = max(dot(LightVector, Norm), 0.0);
  

  if(lamb > 0.0) {
    vec3 ReflectedLV = reflect(-LightVector, Norm);      // Reflected light vector
    vec3 VectorViewer = normalize(-vertexPosition); // Vector to viewer


    // Compute the specular term
    float specAngle = max(dot(ReflectedLV, VectorViewer), 0.0);
    specular = pow(specAngle, amountOfShine);
  }
  
  gl_FragColor = vec4((ambientAmount * ambientColour) + (diffuseAmount * lamb * diffuseColour) +
                      (specularAmount * specular * specularColour), 1.0)  ;

}
