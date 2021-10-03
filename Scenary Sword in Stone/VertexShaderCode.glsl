#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

uniform int movable;
uniform mat4 defaultTransformMatrix;
uniform mat4 modelTransformMatrix;
uniform mat4 view;
uniform mat4 projection;

out vec3 theColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 out_position;
	if(movable == 1){
		out_position = projection * view * modelTransformMatrix * v;
	}else{
		out_position = projection * view * defaultTransformMatrix * v;
	}
	gl_Position = out_position;	
	theColor = vertexColor;
}

