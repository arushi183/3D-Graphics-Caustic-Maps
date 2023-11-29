#version 330 core
const float causticsFactor = 0.15;

in vec3 oldPosition;
in vec3 newPosition;
in float waterDepth;
in float depth;


void main() {
  float causticsIntensity = 0.;

  if (depth >= waterDepth) {
    float oldArea = length(dFdx(oldPosition)) * length(dFdy(oldPosition));
    float newArea = length(dFdx(newPosition)) * length(dFdy(newPosition));

    float ratio;

    // Prevent dividing by zero (debug NVidia drivers)
    if (newArea == 0.) {
      // Arbitrary large value
      ratio = 2.0e+20;
    } else {
      ratio = oldArea / newArea;
    }

    causticsIntensity = causticsFactor * ratio;
  }

  gl_FragColor = vec4(causticsIntensity, 0., 0., depth);
}