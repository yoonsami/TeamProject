#include "Render.fx"
#include "Light.fx"

ByteAddressBuffer Input;
RWByteAddressBuffer Output; //uav

struct ComputeInput
{
    uint3 groupID : SV_GroupID;
    uint3 groupThreadID : SV_GroupThreadID;
    uint3 dispatchThreadID : SV_DispatchThreadID;
    uint groupIndex : SV_GroupIndex;
};

[numthreads(10,8,3)]
void CS(ComputeInput input)
{
    uint index = input.groupID.x * (10 * 8 * 3) +  input.groupIndex;
    uint outAddress = index * 10 * 4 /* sizeof(ComputeInput)*/;
    
    uint inAddress = index * 4;
    float value = (float) Input.Load(inAddress);
    
    Output.Store3(outAddress + 0, input.groupID);
    Output.Store3(outAddress + 12, input.groupThreadID);
    Output.Store3(outAddress + 24, input.dispatchThreadID);
    Output.Store(outAddress + 36, input.groupIndex);
    Output.Store(outAddress + 40, (uint) value);

}


technique11 T0
{
    PASS_CS(P0, CS)
};