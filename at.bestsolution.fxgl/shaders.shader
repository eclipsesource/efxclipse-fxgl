cbuffer VS_CONSTANT_BUFFER : register(b0) {
	float fRot;
};

struct VOut {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR) {
	VOut output;

	matrix<float, 4, 4> a = { 1, 0, 0, 0,
							  0, 1, 0, 0,
							  0, 0, 1, 0,
							  -1, 0, 0, 1 };

	matrix<float, 4, 4> rotZ = { cos(fRot), -sin(fRot), 0, 0,
								 sin(fRot), cos(fRot), 0, 0,
								 0, 0, 1, 0,
								 0, 0, 0, 1 };
	matrix<float, 4, 4> rotX = { 1, 0, 0, 0,
								0, cos(fRot), -sin(fRot), 0,
								0, sin(fRot), cos(fRot), 0,
								0, 0, 0, 1 };
	matrix<float, 4, 4> rotY = { cos(fRot), 0, -sin(fRot), 0,
								0,         1,          0, 0,
								sin(fRot), 0,  cos(fRot), 0,
								0,         0,          0, 1 };

	matrix<float, 4, 4> b = { 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1, 1, 0, 1 };

	matrix<float, 4, 4> all = rotZ;

	output.position = mul(position, all);
	output.color = color;

	return output;
}

struct PS_OUTPUT
{
	float4 ColorA : SV_Target0;
	float4 ColorB : SV_Target1;
};

PS_OUTPUT PShader(float4 position: SV_POSITION, float4 color : COLOR) {
	PS_OUTPUT o;
	o.ColorA = color;
	o.ColorB = color;
	return o;
}