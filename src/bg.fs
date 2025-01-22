#version 330

// uniform float time; // Pass time from the game
// uniform vec2 resolution; // Screen resolution

// void main() {
//     vec2 uv = gl_FragCoord.xy / resolution; // Normalize coordinates
//     float gradient =0.5 + 0.5 * cos(uv.y * 10.0 + time); // Add a wave effect
//     gl_FragColor = vec4(uv.x, gradient, uv.y, 1.0); // Cool color transition
// }

// Vertex Shader

// layout (location = 0) in vec2 aPos;
// void main()
// {
//     gl_Position = vec4(aPos, 0.0, 1.0);
// }

// Fragment Shader
// #version 330 core
out vec4 FragColor;
uniform float time;
uniform vec2 resolution;

// Random number generator based on coordinates
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Smooth noise function
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main() {
    vec2 uv = gl_FragCoord.xy / resolution;
    
    // Nebula background (smooth gradient)
    vec3 color = vec3(0.0);
    vec2 pos = uv * 10.0;
    float n = noise(pos);
    color += vec3(0.1, 0.2, 0.5) * n; // Blue nebula base
    color += vec3(0.8, 0.2, 0.6) * (1.0 - n); // Magenta highlights

    // Stars
    for (int i = 0; i < 10; i++) {
        vec2 starPos = vec2(random(vec2(float(i), time)), random(vec2(float(i) + 1.0, time + 1.0)));
        starPos *= vec2(1.0, resolution.y / resolution.x); // Adjust for aspect ratio
        float starSize = 0.002 + 0.003 * random(vec2(float(i) + 2.0, time));
        float d = distance(uv, starPos);
        float star = smoothstep(starSize, starSize * 0.5, d);
        color += vec3(1.0) * star;
    }

    // Twinkling effect
    float twinkle = 0.5 + 0.5 * sin(time * 5.0 + uv.x * 10.0 + uv.y * 10.0);
    color += vec3(0.8, 0.9, 1.0) * twinkle * 0.02;

    // Output the final color
    FragColor = vec4(color, 1.0);
}

