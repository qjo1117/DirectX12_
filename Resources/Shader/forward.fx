#ifndef _FORWARD_FX_
#define _FORWARD_FX_

#include "params.fx"
#include "utils.fx"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;

    row_major matrix matWorld : W;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
    uint instancingID : SV_InstanceID;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGET;
    float3 viewBinormal : BINORMAL;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    if (g_int_3 == 1)
    {
        output.pos = mul(float4(input.pos, 1.f), input.matWVP);
        output.uv = input.uv;

        output.viewPos = mul(float4(input.pos, 1.f), input.matWV).xyz;
        output.viewNormal = normalize(mul(float4(input.normal, 0.f), input.matWV).xyz);
        output.viewTangent = normalize(mul(float4(input.tangent, 0.f), input.matWV).xyz);
        output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));
    }
    else
    {
        output.pos = mul(float4(input.pos, 1.f), g_matWVP);
        output.uv = input.uv;

        output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz;
        output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);
        output.viewTangent = normalize(mul(float4(input.tangent, 0.f), g_matWV).xyz);
        output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));
    }

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (g_tex_on_0)
    {
        color = g_tex_0.Sample(g_sam_0, input.uv);
    }

    float3 viewNormal = input.viewNormal;
    if (g_tex_on_1)      // Normal이라고 정해둔 것
    {
        // [0, 255] -> [0, 1] Sample에서 스튜러스를 알아서 해줌
        float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
        // [0, 1] -> [-1, 1]
        tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.0f;

        float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
        viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
    }
 
    LightColor totalColor = (LightColor)0.f;

    for (int i = 0; i < g_lightCount; ++i)
    {
        LightColor lightColor = CalculateLightColor(i, viewNormal, input.viewPos);
            totalColor.diffuse += lightColor.diffuse;
            totalColor.ambient += lightColor.ambient;
            totalColor.specular += lightColor.specular;
    }

    color.xyz = (totalColor.diffuse.xyz * color.xyz)
        + totalColor.ambient.xyz * color.xyz
        + totalColor.specular.xyz;

    return color;
}

// SimpleTexture처럼 쓰는 녀석
struct VS_TEX_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_TEX_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_TEX_OUT VS_Tex(VS_TEX_IN input)
{
    VS_TEX_OUT output = (VS_TEX_OUT) 0;

    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.uv = input.uv;

    return output;
}

float4 PS_Tex(VS_TEX_OUT input) : SV_Target
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    if (g_tex_on_0)
        color = g_tex_0.Sample(g_sam_0, input.uv);

    return color;
}

#endif