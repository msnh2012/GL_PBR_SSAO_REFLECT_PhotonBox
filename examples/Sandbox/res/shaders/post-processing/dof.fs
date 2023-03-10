#version 330
out vec4 FragColor;

in vec2 TexCoords;

//uniform float depth;

uniform sampler2D rendertexture;
uniform sampler2D gPosition;

vec2 texelSize;
const float g = 2.39996322973f;
const float INFINITY = 1.0 / 0.0;


//const float boost = 2;
//const float shallow = 1;
//const bool debug = false;
//const float d = 0;

//float c_depth;
//vec4 c_color;

//const float GOLDEN_ANGLE = 2.39996;
//const int ITERATIONS = 150;

//mat2 rot = mat2(cos(GOLDEN_ANGLE), sin(GOLDEN_ANGLE), -sin(GOLDEN_ANGLE), cos(GOLDEN_ANGLE));


/*
//-------------------------------------------------------------------------------------------
vec3 Bokeh(sampler2D tex, vec2 uv, float radius)
{
	int iterations = min(100, int(radius * 3.1415 * 2 * 1));
    vec3 acc = vec3(0);
    vec3 div = vec3(0);
    float r = 1.0;
    vec2 vangle = vec2(0.0,radius*.01 / sqrt(float(iterations)));
    
    vec3 lastcol = c_color.rgb;


    for (int j = 0; j < iterations; j++)
    {  
        // the approx increase in the scale of sqrt(0, 1, 2, 3...)
        r += 1.0 / r;
        vangle = rot * vangle;
        
		vec3 col = lastcol;

        float sampleDepth = -texture(gPosition, uv + texelSize * (r-1.0) * vangle).z;
        if(c_depth >= sampleDepth){
        	col = texture(tex, uv + texelSize * (r-1.0) * vangle).xyz; /// ... Sample the image
        }

        lastcol = col;

        //col = col * col * 1.8; // ... Contrast it for better highlights - leave this out elsewhere.
        //vec3 bokeh = pow(col, vec3(4));
        acc += pow(col, vec3(2)); // bokeh;
        //div += bokeh;
    }
    return sqrt(acc / vec3(iterations)); //div;
}

void main() 
{
    c_depth = -textureLod(gPosition, TexCoords, 0).z;
    c_depth = c_depth != 0 ? c_depth : 999;
    c_color = textureLod(rendertexture, TexCoords, 0).rgba;
    //float dist = abs(c_depth - depth);
    float dist = abs(c_depth - d);
    dist *= 500;
    texelSize = 1.0 / textureSize(rendertexture, 0);
    
    
    vec3 ret = Bokeh(rendertexture, TexCoords, dist);
    //vec3 ret = vec3(dist);

	FragColor = vec4(ret, 1);
	//FragColor = vec4(vec3(c_depth), 1);
}  
*/



float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 sphere_sample(float r, float b, vec2 pos)
{
    vec3 accum = vec3(0);

    float k = g * b;
    
    float a = 1.0f/(sqrt(g * (b+1.0f))-1.0f);
    
    int n = int(pow((sqrt(k) - 1.0f)*r+1.0f, 2.0f) * 1.0f/g - 1.0f);
    
    
    for(int i = 0; i < n; i++)
    {
        float h = float(i) * g + rand(pos);
        float R = a * (sqrt(h) - 1.0f);
        
        vec2 offset = vec2(R * cos(h), R * sin(h))*texelSize.xy;
        accum += textureLod(rendertexture, pos + offset, 0).rgb;
    }
    
    return accum / float(n);
}


void main() 
{
	texelSize = 1.0f / textureSize(rendertexture, 0);

    //float sampleDepth = -texture(gPosition, uv + texelSize * (r-1.0) * vangle).z;
	float depth = -texture(gPosition, TexCoords).z;
	if(depth <= 0.0001f){
		depth = INFINITY;
	}

	FragColor = vec4(sphere_sample(min(depth, 10.0f), 2.0f, TexCoords), 1);
	//FragColor = vec4(vec3(depth), 1);
}  