imEngine::String widgetProgramSource = R"(

///### VERTEX SHADER ###///
in vec2 in_offset;
in vec2 in_size;
in vec4 in_texCoords;

out vec4 texCoords;
out vec2 size;

uniform vec2 u_windowSize;
uniform vec2 u_position;

vec2 winToScreen(vec2 winVec, vec2 winSize) {
        return vec2(-1 + 2*winVec.x/winSize.x, 1 - 2*winVec.y/winSize.y);
}

void main() {
        gl_Position = vec4(winToScreen(in_offset + u_position, u_windowSize), 0.0, 1.0);
        texCoords = in_texCoords;
        size = in_size * (2.0/u_windowSize);
}

///### GEOMETRY SHADER ###///
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 texCoords[];
in vec2 size[];

out vec2 TexCoord;

void main() {
        vec2 pos = gl_in[0].gl_Position.xy;
        float depth = gl_in[0].gl_Position.z;

        // Left bottom
        gl_Position = vec4(pos.x, pos.y - size[0].y, depth, 1.0);
        TexCoord = vec2(texCoords[0].x, texCoords[0].y);
        EmitVertex();

        // Right bottom
        gl_Position = vec4(pos.x + size[0].x, pos.y - size[0].y, depth, 1.0);
        TexCoord = vec2(texCoords[0].z, texCoords[0].y);
        EmitVertex();

        // Left top
        gl_Position = vec4(pos.x, pos.y, depth, 1.0);
        TexCoord = vec2(texCoords[0].x, texCoords[0].w);
        EmitVertex();

        // Rigth top
        gl_Position = vec4(pos.x + size[0].x, pos.y, depth, 1.0);
        TexCoord = vec2(texCoords[0].z, texCoords[0].w);
        EmitVertex();

        EndPrimitive();
}

///### FRAGMENT SHADER ###///
layout (location = 0) out vec4 im_outColor;

uniform sampler2D       u_texture;

in vec2 TexCoord;

void main() {
        im_outColor = texture2D(u_texture, TexCoord);
}

)";
