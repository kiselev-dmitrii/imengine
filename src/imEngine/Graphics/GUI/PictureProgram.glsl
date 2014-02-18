imEngine::String pictureProgramSource = R"(

///### VERTEX SHADER ###///
in vec2 in_zero;

out vec2 size;

uniform vec2 u_windowSize;
uniform vec2 u_position;
uniform vec2 u_size;

vec2 winToScreen(vec2 winVec, vec2 winSize) {
        return vec2(-1 + 2*winVec.x/winSize.x, 1 - 2*winVec.y/winSize.y);
}

void main() {
        gl_Position = vec4(winToScreen(in_zero + u_position, u_windowSize), 0.0, 1.0);
        size = u_size * (2.0/u_windowSize);
}

///### GEOMETRY SHADER ###///
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec2 size[];

out vec2 TexCoord;

void main() {
        vec2 pos = gl_in[0].gl_Position.xy;
        float depth = gl_in[0].gl_Position.z;

        // Left bottom
        gl_Position = vec4(pos.x, pos.y - size[0].y, depth, 1.0);
        TexCoord = vec2(0.0, 0.0);
        EmitVertex();

        // Right bottom
        gl_Position = vec4(pos.x + size[0].x, pos.y - size[0].y, depth, 1.0);
        TexCoord = vec2(1.0, 0.0);
        EmitVertex();

        // Left top
        gl_Position = vec4(pos.x, pos.y, depth, 1.0);
        TexCoord = vec2(0.0, 1.0);
        EmitVertex();

        // Rigth top
        gl_Position = vec4(pos.x + size[0].x, pos.y, depth, 1.0);
        TexCoord = vec2(1.0, 1.0);
        EmitVertex();

        EndPrimitive();
}

///### FRAGMENT SHADER ###///
layout (location = 0) out vec4 im_outColor;

uniform sampler2D       u_texture;
#ifdef DEPTH_TEXTURE
        uniform float   u_nearDistance;
        uniform float   u_farDistance;
#endif //DEPTH_TEXTURE
uniform float           u_opacity;

in vec2 TexCoord;

#ifdef DEPTH_TEXTURE
float normalizeDepth(float near, float far, float depth) {
        return (2.0*near) / (far+near - depth*(far-near));
}
#endif //DEPTH_TEXTURE

void main() {
        vec4 color = texture2D(u_texture, TexCoord);

#ifdef DEPTH_TEXTURE
        float depth = normalizeDepth(u_nearDistance, u_farDistance, color.r);
        im_outColor = vec4(vec3(depth), u_opacity);
#else
        im_outColor = vec4(color.rgb, color.a * u_opacity);
#endif //DEPTH_TEXTURE

}

)";
