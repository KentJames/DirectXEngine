// Define Globals //

cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}


/////////////////
// Typedefs    //
///////////////

struct VertexInputType{
	float4 position: POSITION;
	float4 color: COLOR;
};

struct PixelInputType
{
	float4 position: SV_POSITION;
	float4 color : COLOR;
};


//Vertex Shader

PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	//Change position to 4 units for matrix calculation.
	input.position.w = 1.0f;

	// Calculate position of vertex against world, view,
	// projection matrices.

	output.position = mul(input.position,worldMatrix);
	output.position = mul(output.position,viewMatrix);
	output.position = mul(output.position,projectionMatrix);

	//Store color for pixel shader use.

	output.color = input.color;

	return output;

}
