# version 330 core

layout(location=0) out vec4 color;
in vec4 v_color;

// uniform vec4 u_Color;
// uniform sampler2D u_Texture;

void main()
{
    // vec4 texColor= texture(u_Texture,v_texCoord);
    // vec4 texColor= u_Color;

    if(gl_FrontFacing)
    {
        color = 10000*v_color;
    }
    else
    {
    color= vec4(1.,1.,1.,1.);
    }
    // color = v_color;
    // color=vec4(0.,0.,0.392156886,1.);
}
