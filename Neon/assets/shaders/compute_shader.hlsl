struct Pixel
{
    int color;
};

RWStructuredBuffer<Pixel> BufferOut : register(u0);

void writeToPixel(int x, int y)
{
    uint index = (x + y * 16);
    
    int ired   = (int)(clamp(x,0,1) * 255);
    int igreen = (int)(clamp(y,0,1) * 255) << 8;
    int iblue  = (int)(clamp(0,0,1) * 255) << 16;
    
    BufferOut[index].color = 11;
}

[numthreads(16, 16, 1)]
void main( uint3 dispatchThreadID : SV_DispatchThreadID )
{
    writeToPixel(dispatchThreadID.x, dispatchThreadID.y);
}