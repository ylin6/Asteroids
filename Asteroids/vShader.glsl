attribute vec4 vPosition;
attribute vec2 vTexCoord;
attribute vec3 vNormal;

varying vec4 color;
varying vec2 texCoord;

uniform vec4 AmbientProduct;
uniform vec4 DiffuseProduct;
uniform vec4 SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;
uniform int flag;
uniform mat4 model_view;
uniform mat4 projection;

void main(){

    if ( flag == 7){
        gl_Position = vPosition + vec4(1.5, 1.85, 0.0, 1.0) ;
    }

    else{
	vec3 pos = (model_view * vPosition).xyz;
	
    vec3 L = normalize( (LightPosition).xyz - pos );
    vec3 E = normalize( -pos );
    vec3 N = normalize( model_view *vec4(vNormal, 0.0) ).xyz;
    vec3 R = normalize( -reflect(L,N));
    vec4 ambient = AmbientProduct;

    float Kd = max( dot(N, L), 0.0 );
    vec4  diffuse = Kd*DiffuseProduct;
    diffuse = clamp(diffuse, 0.0, 1.0);

    float Ks = pow( max(dot(R, E), 0.0), Shininess );
    vec4  specular = Ks * SpecularProduct;
    specular = clamp(specular, 0.0, 1.0);

    if( dot(L, N) < 0.0 ) {
	specular = vec4(0.0, 0.0, 0.0, 1.0);
    } 

	gl_Position = projection * model_view * vPosition/vPosition.w;
	texCoord = vTexCoord;

	color = ambient + diffuse + specular;
	color.a = 1.0;
    }
}