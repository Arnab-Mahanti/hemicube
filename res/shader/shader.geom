# version 330 core
// #extension GL_ARB_gpu_shader5 : require

layout(triangles) in;
layout(triangle_strip, max_vertices=6) out;
// layout(invocations=2) in;

in vec4 v_color[];
out vec4 f_color;
void main()
{
    // f_color=v_color[0]+(gl_InvocationID)*vec4(0.6,0,0,1);
    // gl_Position=gl_in[0].gl_Position+(gl_InvocationID)*vec4(60,0,0,0);
    // EmitVertex();
    // gl_Position=gl_in[1].gl_Position+(gl_InvocationID)*vec4(60,0,0,0);
    // EmitVertex();
    // gl_Position=gl_in[2].gl_Position+(gl_InvocationID)*vec4(60,0,0,0);
    // EmitVertex();

    f_color=v_color[0]+(0.)*vec4(0.6,0,0,1);
    gl_Position=gl_in[0].gl_Position+(0.)*vec4(60,0,0,0);
    EmitVertex();
    gl_Position=gl_in[1].gl_Position+(0.)*vec4(60,0,0,0);
    EmitVertex();
    gl_Position=gl_in[2].gl_Position+(0.)*vec4(60,0,0,0);
    EmitVertex();
    EndPrimitive();

    gl_Position=gl_in[0].gl_Position+(1.0)*vec4(60,0,0,0);
    EmitVertex();
    gl_Position=gl_in[1].gl_Position+(1.0)*vec4(60,0,0,0);
    EmitVertex();
    gl_Position=gl_in[2].gl_Position+(1.0)*vec4(60,0,0,0);
    EmitVertex();
    EndPrimitive();
}