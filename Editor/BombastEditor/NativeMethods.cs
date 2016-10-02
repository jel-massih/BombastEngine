using System;
using System.Runtime.InteropServices;

namespace BombastEditor
{
    static class NativeMethods
    {
        const string BombastEditorDLLName = "BombastEditorDLL.dll";

        [DllImport(BombastEditorDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern int InitializeBombastProject(IntPtr instancePtrAddress, IntPtr hPrevInstancePtrAddress, IntPtr hWndPtrAddress, int nCmdShow, int screenWidth, int screenHeight, [MarshalAs(UnmanagedType.BStr)] string projectFilepath);
        [DllImport(BombastEditorDLLName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void RenderFrame();
        [DllImport(BombastEditorDLLName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Shutdown();
        [DllImport(BombastEditorDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void WndProc(IntPtr hWndPtrAddress, int msg, int wParam, int lParam);

        [DllImport(BombastEditorDLLName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void OpenLevel([MarshalAs(UnmanagedType.BStr)] string levelResourceName);
        [DllImport(BombastEditorDLLName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SaveLevel(string fileName);

        [DllImport(BombastEditorDLLName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetActorCount();
        [DllImport(BombastEditorDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void GetActorList(IntPtr actorArrayPtr, int size);
        [DllImport(BombastEditorDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void GetActorXml(IntPtr actorXMLPtrAddress, uint actorId);
        [DllImport(BombastEditorDLLName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetActorXmlSize(uint actorId);
    }
}
