varying vec2 texCoord;
varying vec4 color;

uniform sampler2D texMap;
uniform sampler2D texMap2;
uniform int flag;
uniform float flame;
 
void main(){
	if(flag == 0){
		gl_FragColor = color * vec4(1.0, 1.0, 1.0, 1.0) ;
	}

	else if (flag == 1){
		gl_FragColor = vec4(1.0, 0.3 + flame, 0.00 + flame, 1.0) ;
	}

	else if ( flag == 3){
		gl_FragColor = vec4(0.0, 0.0, 0.1, 1.0);
	}
	
	else if (flag == 4){
		gl_FragColor = color * vec4(0.5, 0.5, 0.5, 1.0) ;
	}

	else if (flag == 5){
		gl_FragColor = texture2D(texMap, texCoord);
	}

	else if (flag == 6){
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}

	else if (flag == 7){
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) ;
	}
}