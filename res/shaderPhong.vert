attribute vec3 position;
attribute vec3 normal;
uniform mat4 projection, modelView, normalMatrix;

varying vec3 surfaceNormal;
varying vec3 vertexPosition;

void main(){

  vec4 vertPos4 = modelView * vec4(position, 1.0);
  vertexPosition = vec3(vertPos4) / vertPos4.w;
  
  surfaceNormal = vec3(normalMatrix * vec4(normal, 0.0));
  gl_Position = projection * vertPos4;
}
