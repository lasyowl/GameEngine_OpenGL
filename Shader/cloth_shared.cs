#version 430

layout(local_size_x = 10, local_size_y = 10) in;

uniform vec3 Gravity = vec3(0,-10,0);
uniform float ParticleMass = 0.1;
uniform float ParticleInvMass = 1.0 / 0.1;
uniform float SpringK = 2500.0;
uniform float RestLengthHoriz;
uniform float RestLengthVert;
uniform float RestLengthDiag;
uniform float DeltaT = 0.000005;
uniform float DampingConst = 0.1;

layout(std430, binding= 0) buffer PosIn
{
    vec4 PositionIn [];
};
layout(std430, binding= 1) buffer PosOut
{
    vec4 PositionOut [];
};
layout(std430, binding= 2) buffer VelIn
{
    vec4 VelocityIn [];
};
layout(std430, binding= 3) buffer VelOut
{
    vec4 VelocityOut [];
};

shared vec3 p[400];
shared vec3 v[400];
shared vec3 r[400];
shared vec3 force[400];
shared vec3 a[400];

void main()
{
    uvec3 nParticles = gl_NumWorkGroups * gl_WorkGroupSize;
    uint idx = gl_GlobalInvocationID.y * nParticles.x + gl_GlobalInvocationID.x;

    p[idx] = vec3(PositionIn[idx]);
    v[idx] = vec3(VelocityIn[idx]);

    // Start with gravitational acceleration and add the spring
    // forces from each neighbor
    force[idx] = Gravity * ParticleMass;

    // Particle above
    if (gl_GlobalInvocationID.y < nParticles.y - 1)
    {
        r[idx] = PositionIn[idx + nParticles.x].xyz - p[idx];
        force[idx] += normalize(r[idx]) * SpringK * (length(r[idx]) - RestLengthVert);
    }
    // Below
    if (gl_GlobalInvocationID.y > 0)
    {
        r[idx] = PositionIn[idx - nParticles.x].xyz - p[idx];
        force[idx] += normalize(r[idx]) * SpringK * (length(r[idx]) - RestLengthVert);
    }
    // Left
    if (gl_GlobalInvocationID.x > 0)
    {
        r[idx] = PositionIn[idx - 1].xyz - p[idx];
        force[idx] += normalize(r[idx]) * SpringK * (length(r[idx]) - RestLengthHoriz);
    }
    // Right
    if (gl_GlobalInvocationID.x < nParticles.x - 1)
    {
        r[idx] = PositionIn[idx + 1].xyz - p[idx];
        force[idx] += normalize(r[idx]) * SpringK * (length(r[idx]) - RestLengthHoriz);
    }

    // Diagonals
    // Upper-left
    if (gl_GlobalInvocationID.x > 0 && gl_GlobalInvocationID.y < nParticles.y - 1)
    {
        r[idx] = PositionIn[idx + nParticles.x - 1].xyz - p[idx];
        force[idx] += normalize(r[idx]) * SpringK * (length(r[idx]) - RestLengthDiag);
    }
    // Upper-right
    if (gl_GlobalInvocationID.x < nParticles.x - 1 && gl_GlobalInvocationID.y < nParticles.y - 1)
    {
        r[idx] = PositionIn[idx + nParticles.x + 1].xyz - p[idx];
        force[idx] += normalize(r[idx]) * SpringK * (length(r[idx]) - RestLengthDiag);
    }
    // lower -left
    if (gl_GlobalInvocationID.x > 0 && gl_GlobalInvocationID.y > 0)
    {
        r[idx] = PositionIn[idx - nParticles.x - 1].xyz - p[idx];
        force[idx] += normalize(r[idx]) * SpringK * (length(r[idx]) - RestLengthDiag);
    }
    // lower-right
    if (gl_GlobalInvocationID.x < nParticles.x - 1 && gl_GlobalInvocationID.y > 0)
    {
        r[idx] = PositionIn[idx - nParticles.x + 1].xyz - p[idx];
        force[idx] += normalize(r[idx]) * SpringK * (length(r[idx]) - RestLengthDiag);
    }

    force[idx] += -DampingConst * v[idx];

    // Apply simple Euler integrator
    a[idx] = force[idx] * ParticleInvMass;
    PositionOut[idx] = vec4(
        p[idx] + v[idx] * DeltaT + 0.5 * a[idx] * DeltaT * DeltaT, 1.0);
    VelocityOut[idx] = vec4(v[idx] + a[idx] * DeltaT, 0.0);

    // Pin a few of the top verts
    if (gl_GlobalInvocationID.y == nParticles.y - 1 &&
        (gl_GlobalInvocationID.x == 0 ||
         gl_GlobalInvocationID.x == nParticles.x / 4 ||
         gl_GlobalInvocationID.x == nParticles.x * 2 / 4 ||
         gl_GlobalInvocationID.x == nParticles.x * 3 / 4 ||
         gl_GlobalInvocationID.x == nParticles.x - 1))
    {
        PositionOut[idx] = vec4(p[idx], 1.0);
        VelocityOut[idx] = vec4(0, 0, 0, 0);
    }
}