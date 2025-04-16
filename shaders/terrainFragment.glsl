#version 330 core

out vec4 FragColor;

in float Height;

vec3 determineColour() {
	vec3 waterColour = vec3(15.0 / 255.0, 94.0 / 255.0, 156.0 / 255.0);
	vec3 brownColour = vec3(107.0 / 255.0, 84.0 / 255.0, 40.0 / 255.0);
	vec3 greyColour = vec3(128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
	vec3 grassColour = vec3(85.0 / 255.0, 130.0 / 255.0, 40.0 / 255.0);

	//float intensity = clamp((Height + 16.0) / 32.0, 0.0, 1.0);
	float intensity = smoothstep(-16.0, 30.0, Height);

	if (Height <= 0.0) {
		return waterColour*intensity;
	} else if (Height < 10.0) {
		return grassColour*intensity*1.2;
	} else if (Height > 10.0 && Height < 25.0) {
		return mix(grassColour, brownColour, smoothstep(10.0, 25.0, Height))*intensity; 
	} else if (Height < 25.0) {
		return brownColour*intensity;
	} else if (Height > 25.0 && Height < 35.0) {
		return mix(brownColour, greyColour, smoothstep(25.0, 35.0, Height))*intensity; 
	} else {
		return greyColour*intensity;
	}
}

void main() {
	vec3 h = determineColour();
	float alpha = Height == 0.0 ? 0.9 : 1.0;
	FragColor = vec4(h, alpha);
}