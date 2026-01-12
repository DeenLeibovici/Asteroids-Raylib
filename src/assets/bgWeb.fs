#version 100
precision mediump float;

uniform float iTime;
uniform float iMousex;
uniform float iMousey;

void main(){
    float speed = .041;
    float scale = 0.002;

    vec4 p = gl_FragCoord * scale;   

    for(int i=1; i<10; i++){
        p.x+=0.5/float(i)*sin(float(i)*3.*p.y+iTime*speed)+iMousex/1000.;
        p.y+=0.3/float(i)*cos(float(i)*3.*p.x+iTime*speed)+iMousey/1000.;
    }

    float r=cos(p.x+p.y+1.)*.5+.5;
    float g=sin(p.x+p.y+1.)*.5+.5;
    float b=(sin(p.x+p.y)+cos(p.x+p.y))*.85+.5;

    vec3 color = vec3(r,g,b);
    gl_FragColor = vec4(color,1.0);
}