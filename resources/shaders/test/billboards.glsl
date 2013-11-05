///### VERTEX SHADER ###///
in vec3 im_vPosition;

void main() {
        gl_Position = vec4(im_vPosition, 1.0);
}

///### GEOMETRY SHADER ###///
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
uniform float halfSize;
out vec2 TexCoord;

void main() {
	// Left bottom
	gl_Position = vec4(-halfSize, -halfSize, 0.0, 0.0) + gl_in[0].gl_Position;
	TexCoord = vec2(0.0, 0.0);
	EmitVertex();

	// Right bottom
	gl_Position = vec4(halfSize, -halfSize, 0.0, 0.0) + gl_in[0].gl_Position;
	TexCoord = vec2(1.0, 0.0);
	EmitVertex();

	// Left top
	gl_Position = vec4(-halfSize, halfSize, 0.0, 0.0) + gl_in[0].gl_Position;
	TexCoord = vec2(0.0, 1.0);
	EmitVertex();

	// Rigth top
	gl_Position = vec4(halfSize, halfSize, 0.0, 0.0) + gl_in[0].gl_Position;
	TexCoord = vec2(1.0, 1.0);
	EmitVertex();


	EndPrimitive();
}

///### FRAGMENT SHADER ###///
layout (location = 0) out vec4 im_outColor;
uniform sampler2D billboardTex;
in vec2 TexCoord;

void main() {
		im_outColor = texture(billboardTex, TexCoord);
}