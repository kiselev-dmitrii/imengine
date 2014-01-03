imEngine::String showTextureSource = R"(

///### VERTEX SHADER ###///
in vec3 in_position;
in vec4 in_texCoords;
in vec2 in_size;

out vec4 texCoords;
out vec2 size;

uniform vec2 winSize;

vec2 convertWinSpaceToScreenSpace(vec2 winVec, vec2 winSize) {
        return vec2(-1 + 2*winVec.x/winSize.x, 1 - 2*winVec.y/winSize.y);
}

void main() {
        gl_Position = vec4(convertWinSpaceToScreenSpace(in_position.xy, winSize), in_position.z, 1.0);
        texCoords = in_texCoords;
        size = in_size * (2.0/winSize);
}

///### GEOMETRY SHADER ###///
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 texCoords[];
in vec2 size[];

out vec2 TexCoord;

void main() {
        vec3 leftTopPos = gl_in[0].gl_Position.xyz;

        // Left bottom
        gl_Position = vec4(leftTopPos.x, leftTopPos.y - size[0].y, leftTopPos.z, 1.0);
        TexCoord = vec2(texCoords[0].x, texCoords[0].y);
        EmitVertex();

        // Right bottom
        gl_Position = vec4(leftTopPos.x + size[0].x, leftTopPos.y - size[0].y, leftTopPos.z, 1.0);
        TexCoord = vec2(texCoords[0].z, texCoords[0].y);
        EmitVertex();

        // Left top
        gl_Position = vec4(leftTopPos.x, leftTopPos.y, leftTopPos.z, 1.0);
        TexCoord = vec2(texCoords[0].x, texCoords[0].w);
        EmitVertex();

        // Rigth top
        gl_Position = vec4(leftTopPos.x + size[0].x, leftTopPos.y, leftTopPos.z, 1.0);
        TexCoord = vec2(texCoords[0].z, texCoords[0].w);
        EmitVertex();

        EndPrimitive();
}

///### FRAGMENT SHADER ###///
layout (location = 0) out vec4 im_outColor;
uniform sampler2D fontAtlas;
in vec2 TexCoord;

void main() {
        im_outColor = texture2D(fontAtlas, TexCoord);
}

)";
