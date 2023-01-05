using System;

unsafe public class UnrealMonoTest
{
    private static float* JumpHeightPtr;

    public static void SetPtr(void* Ptr)
    {
        JumpHeightPtr = (float*)Ptr;
    }

    public static void ChangeVal()
    {
        (*JumpHeightPtr)++;
    }
}