//type vertex
#version 330

out vec3 WorldPos;

uniform mat4 u_ProjectionView;
uniform vec3 u_CameraPosition;

uniform float u_GridSize = 100.0;

vec3 Pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),      // bottom left
    vec3( 1.0, 0.0, -1.0),      // bottom right
    vec3( 1.0, 0.0,  1.0),      // top right
    vec3(-1.0, 0.0,  1.0)       // top left
);

int Indices[6] = int[6](0, 2, 1, 2, 0, 3);

void main()
{
    int Index = Indices[gl_VertexID];
    vec3 vPos3 = Pos[Index] * u_GridSize;

    vPos3.x += u_CameraPosition.x;
    vPos3.z += u_CameraPosition.z;

    vec4 vPos4 = vec4(vPos3, 1.0);

    gl_Position = u_ProjectionView * vPos4;

    WorldPos = vPos3;
}

//type fragment
#version 330

layout (location = 0) out vec4 FragColor;

in vec3 WorldPos;

uniform vec3 u_CameraPosition;
uniform float u_GridSize = 100.0;
uniform float u_GridMinPixelsBetweenCells = 5.0;
uniform float u_GridCellSize = 2;
uniform vec4 u_GridColorThin = vec4(0.5);
uniform vec4 u_GridColorThick = vec4(vec3(0.5), 1.0);

float log10(float x) {
    return log(x) / log(10.0);
}

float satf(float x) {
    return clamp(x, 0.0, 1.0);
}

vec2 satv(vec2 x) {
    return clamp(x, vec2(0.0), vec2(1.0));
}

float max2(vec2 v) {
    return max(v.x, v.y);
}

void main() {
    vec2 dvx = vec2(dFdx(WorldPos.x), dFdy(WorldPos.x));
    vec2 dvy = vec2(dFdx(WorldPos.z), dFdy(WorldPos.z));
    float lx = length(dvx);
    float ly = length(dvy);
    vec2 dudv = vec2(lx, ly);
    float l = length(dudv);
    float LOD = max(0.0, log10(l * u_GridMinPixelsBetweenCells / u_GridCellSize) + 1.0);
    float GridCellSizeLod0 = u_GridCellSize * pow(10.0, floor(LOD));
    float GridCellSizeLod1 = GridCellSizeLod0 * 10.0;
    float GridCellSizeLod2 = GridCellSizeLod1 * 10.0;
    dudv *= 4.0;

    vec2 mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod0) / dudv;
    float Lod0a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)));
    mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod1) / dudv;
    float Lod1a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)));
    mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod2) / dudv;
    float Lod2a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)));

    float LOD_fade = fract(LOD);
    vec4 Color;

    if (Lod2a > 0.0) {
        Color = u_GridColorThick;
        Color.a *= Lod2a;
    } else if (Lod1a > 0.0) {
        Color = mix(u_GridColorThick, u_GridColorThin, LOD_fade);
        Color.a *= Lod1a;
    } else {
        Color = u_GridColorThin;
        Color.a *= (Lod0a * (1.0 - LOD_fade));
    }

    // Distance-based gradient effect
    float distanceToCamera = length(WorldPos.xz - u_CameraPosition.xz);
    float gradient = smoothstep(0.0, u_GridSize, distanceToCamera);

    // Blend the grid colors with a gradient fade-out
    vec4 gradientColor = mix(Color, vec4(0.0, 0.0, 0.0, 0.0), gradient);
    gradientColor.a *= (1.0 - gradient);

    FragColor = gradientColor;
  }
