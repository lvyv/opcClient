# opcClient

*Memory leak* occurs when the original OpenOPC package connects to an OPC DA server (WinCC etc.) and SyncRead or SyncWrite some tags.

It is a serious problem, especially in plant situation. This project is aimed to solve it, and provide a window‘s native DLL for Python, so as to leverage advantages of both programming languages. The final software should run on old windows boxes to collect PLC data in WinCC OPC DA Server.

Test enviroment is assumed as WinCC7.0 and Windows7 x86 sp1.  

