// 알고 잇으니 패스

cbuffer TRANSFORM_PARAMS : register(b0)
{
    row_major matrix matWVP;
};

cbuffer MATERIAL_PARAMS : register(b1)
{
    int int_0;
    int int_1;
    int int_2;
    int int_3;
    int int_4;
    float float_0;
    float float_1;
    float float_2;
    float float_3;
    float float_4;
};

Texture2D tex_0 : register(t0);
Texture2D tex_1 : register(t1);
Texture2D tex_2 : register(t2);
Texture2D tex_3 : register(t3);
Texture2D tex_4 : register(t4);

SamplerState sam_0 : register(s0);  // 색상을 골라주는 정책

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

<<<<<<< Updated upstream
    output.pos = float4(input.pos, 1.f);
    // TODO : 장난
    output.pos.x += float_0;
    output.pos.y += float_1;
    output.pos.z += float_2;
    
    output.pos += offset0;
    
    output.color = input.color;
=======
    output.pos = mul(float4(input.pos, 1.0f), matWVP);
>>>>>>> Stashed changes
    output.uv = input.uv;
    
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = tex_0.Sample(sam_0, input.uv);
    
    return color;
}