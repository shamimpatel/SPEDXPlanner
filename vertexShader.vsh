uniform mat4 mvpMatrix;
attribute vec4 vertex;
attribute vec3 colour;
varying vec3 fragmentColor;

void main(void)
{
    gl_Position = mvpMatrix * vertex;
    fragmentColor = colour;
}
