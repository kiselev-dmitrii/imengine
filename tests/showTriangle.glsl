imCore::String showTriangleSource = R"(

///### VERTEX SHADER ###///
in vec3 im_vPosition;

void main() {
        gl_Position.xyz = im_vPosition;
        gl_Position.w = 1.0;
}


///### FRAGMENT SHADER ###///
layout (location = 0) out vec4 im_outColor;

void main() {
#ifdef GREEN
        im_outColor = vec4(0,1,0,1);
#endif
#ifdef RED
        im_outColor = vec4(1,0,0,1);
#endif
}

)";
