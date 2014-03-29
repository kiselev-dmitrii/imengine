#ifdef NO_SHADOW_MAPPING
struct Shadow {
		float empty;
};
float calculateShadow(in vec3 positionVS, in Shadow shadow) {
	return 1.0;
}
#endif //NO_SHADOW_MAPPING

#ifdef SHADOW_MAPPING
struct Shadow {
        sampler2DShadow map;
        mat4            VStoSTS;	//переводит вершины из View Space во текстурное пространство теневой карты (Shadow Texture Space)
};

float calculateShadow(in vec3 positionVS, in Shadow shadow) {
        vec4 shadowCoord = shadow.VStoSTS * vec4(positionVS, 1.0);
        return textureProj(shadow.map, shadowCoord);	
}

#endif //SHADOW_MAPPING

#ifdef VARIANCE_SHADOW_MAPPING
struct Shadow {
        sampler2D       map;
        mat4            VStoSTS;	//переводит вершины из View Space во текстурное пространство теневой карты (Shadow Texture Space)
};

float calculateShadow(in vec3 positionVS, in Shadow shadow) {
        vec4 shadowCoord = shadow.VStoSTS * vec4(positionVS, 1.0);
        shadowCoord /= shadowCoord.w;
        float dist = shadowCoord.z;

       	vec2 moments = texture2D(shadow.map, shadowCoord.xy).xy;
        if (dist<= moments.x) return 1.0 ;
        float variance = moments.y - (moments.x*moments.x);
        variance = max(variance,0.00002);
        float d = dist - moments.x;
        float p_max = variance / (variance + d*d);
        
        return pow(p_max, 5); 
}

#endif //VARIANCE_SHADOW_MAPPING
