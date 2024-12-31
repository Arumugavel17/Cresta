//type vertex
/*
        Copyright 2024 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#version 330

out vec3 WorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

uniform float gGridSize = 100.0;

const vec3 Pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),      // bottom left
    vec3( 1.0, 0.0, -1.0),      // bottom right
    vec3( 1.0, 0.0,  1.0),      // top right
    vec3(-1.0, 0.0,  1.0)       // top left
);

const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);


void main()
{
    int Index = Indices[gl_VertexID];
    vec3 vPos3 = Pos[Index] * gGridSize;

    vPos3.x += cameraPos.x;
    vPos3.z += cameraPos.z;

    vec4 vPos4 = vec4(vPos3, 1.0);

    gl_Position = projection * view * vPos4;

    WorldPos = vPos3;
}

//type fragment
/*

        Copyright 2024 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#version 330

in vec3 WorldPos;

layout(location = 0) out vec4 FragColor;

uniform vec3 gCameraWorldPos;
uniform float gGridSize = 100.0;
uniform float gGridMinPixelsBetweenCells = 2.0;
uniform float gGridCellSize = 0.025;
uniform vec4 gGridColorThin = vec4(0.5, 0.5, 0.5, 1.0);
uniform vec4 gGridColorThick = vec4(0.0, 0.0, 0.0, 1.0);


float log10(float x)
{
    float f = log(x) / log(10.0);
    return f;
}


float satf(float x)
{
    float f = clamp(x, 0.0, 1.0);
    return f;
}


vec2 satv(vec2 x)
{
    vec2 v = clamp(x, vec2(0.0), vec2(1.0));
    return v;
}

float max2(vec2 v)
{
    float f = max(v.x, v.y);
    return f;
}


void main()
{
    vec2 dvx = vec2(dFdx(WorldPos.x), dFdy(WorldPos.x));
    vec2 dvy = vec2(dFdx(WorldPos.z), dFdy(WorldPos.z));

    float lx = length(dvx);
    float ly = length(dvy);

    vec2 dudv = vec2(lx, ly);

    float l = length(dudv);

    float LOD = max(0.0, log10(l * gGridMinPixelsBetweenCells / gGridCellSize) + 1.0);

    float GridCellSizeLod0 = gGridCellSize * pow(10.0, floor(LOD));
    float GridCellSizeLod1 = GridCellSizeLod0 * 10.0;
    float GridCellSizeLod2 = GridCellSizeLod1 * 10.0;

    dudv *= 4.0;

    vec2 mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod0) / dudv;
    float Lod0a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );

    mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod1) / dudv;
    float Lod1a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );
    
    mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod2) / dudv;
    float Lod2a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );

    float LOD_fade = fract(LOD);
    vec4 Color;

    if (Lod2a > 0.0) {
        Color = gGridColorThick;
        Color.a *= Lod2a;
    } else {
        if (Lod1a > 0.0) {
            Color = mix(gGridColorThick, gGridColorThin, LOD_fade);
	        Color.a *= Lod1a;
        } else {
            Color = gGridColorThin;
	        Color.a *= (Lod0a * (1.0 - LOD_fade));
        }
    }
    
    float OpacityFalloff = (1.0 - satf(length(WorldPos.xz - gCameraWorldPos.xz) / gGridSize));

    Color.a *= OpacityFalloff;

    FragColor = Color;
}