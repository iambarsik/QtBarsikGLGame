#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;
varying vec3 v_normal;

uniform float red;
uniform float green;
uniform float blue;

uniform bool isTextured;

void main()
{
    if(isTextured)
        gl_FragColor = texture2D(texture, v_texcoord);
    else
        gl_FragColor = vec4(red,green,blue,1.0);
}


