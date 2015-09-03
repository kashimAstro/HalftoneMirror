#version 120
#define TAU 6.28318530718
#define MAX_ITER 5

varying vec2  TexCoord;

uniform sampler2D tInput;
uniform sampler2D texture;
uniform vec2 resolution;
uniform float pixelSize;
uniform int active;
uniform vec3 cols;
uniform vec3 cols2;

varying vec3 e;
varying vec3 n;

void main(void) {
        vec2 p = TexCoord;	
	float pixelsPerRow = resolution.x / pixelSize;
	float pixelsPerCol = resolution.y / pixelSize;

	float pixelSizeX = 1.0 / pixelsPerRow;	
	float dx = mod(p.x, pixelSizeX ) - pixelSizeX *0.5;
	float pixelSizeY = 1.0 / pixelsPerCol;	
	float dy = mod(p.y, pixelSizeY ) - pixelSizeY * 0.5;
	float pixelSize = pixelSizeX;//sqrt( pixelSizeX * pixelSizeX + pixelSizeY + pixelSizeY );

	p.x -= dx;
	p.y -= dy;
	vec3 col = texture2D(tInput, p).rgb;
	vec3 luma = vec3( .299, 0.587, 0.114 );
	float bright = dot( col.rgb, luma );
	
	float dist = sqrt(dx*dx + dy*dy);
	float rad = bright * pixelSize * 1.;
	float m = step( dist, rad );

        vec3 r = reflect( e, n );
        float zm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
        vec2 uv = r.xy / m + .5;
        //float d = length(uv);
        //vec2 st = 0.2*vec2((0.071+d),  sin(0.073-d));
        vec3 xcol = texture2D(texture,uv).xyz;
        vec3 col2;
        if(active==1){
        	col2 = mix(cols, vec3(cols2), m);
        }else{
        	col2 = mix(cols, vec3(xcol), m);
        }
	gl_FragColor = vec4(col2, 1.0);
}