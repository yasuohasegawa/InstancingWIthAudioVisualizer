#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor = vec4(1.0);
uniform	sampler2D tex0;
uniform	sampler2D tex1;
uniform	sampler2D tex2;
uniform	sampler2D tex3;
uniform	sampler2D tex4;
uniform float timeValue=0.0;
uniform float fftvalues[128];

in vec4  position;

out vec4 colorVarying;

#define PI 3.14159265358979323846

// perlin noise from https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

void main()
{
	int iCount = 128;
	
	float instanceX = float(gl_InstanceID%(iCount) - iCount/2) / 128.0;
	float instanceY = float(gl_InstanceID/(iCount) - iCount/2) / 128.0;
	
	float fftval = fftvalues[gl_InstanceID%(iCount)];

	float time = mod(timeValue*0.3, 2.0)-1.0;

	vec2 uv = vec2(instanceX+0.5, 1.0-(instanceY+0.5));
	
	uv.x += time;
	float pixelFont = texture(tex0,uv).r;
	float scene = mod(timeValue*0.3, 8.0);
	if(scene<2.0){
		pixelFont = texture(tex0,uv).r;
	} else if(scene>=2.0 && scene<4.0) {
		pixelFont = texture(tex1,uv).r;
	} else if(scene>=4.0 && scene<5.0) {
		pixelFont = texture(tex2,uv).r;
	} else if(scene>=5.0 && scene<6.0) {
		pixelFont = texture(tex3,uv).r;
	} else if(scene>=6.0 && scene<7.0) {
		pixelFont = texture(tex4,uv).r;
	} else if(scene>=7.0 && scene<8.0) {
		pixelFont = texture(tex0,uv).r;
	}

	vec4 vPos = position;
	vPos.z -= 200;
	vPos.x = vPos.x + instanceX * 2000.0;
	vPos.y = vPos.y + instanceY * 2000.0;

	float nval = snoise(vec2(instanceX+timeValue*0.5,instanceY));
	float nval2 = snoise(vec2(instanceX+timeValue*0.3,instanceY));
	float nval3 = snoise(vec2(instanceX+timeValue*0.1,instanceY));
	float colorval = (fftval*10.0)*sin(timeValue*uv.x*nval);
	if(pixelFont>= 1.0){
		colorVarying = vec4(colorval,colorval,colorval,1.0)+vec4(nval,nval2,nval3,1.0)*0.5;
		vPos.z = (vPos.z-1.0)+(pixelFont * 300.0)+(fftval*50);
	} else {
		colorVarying = vec4(0.7,nval,nval,1.0);
		vPos.z = (vPos.z-1.0)+(pixelFont * 500.0)+(fftval*30);
	}

	vPos.z = vPos.z + (nval*100);

	gl_Position = projectionMatrix * modelViewMatrix * vPos;
}