
BrowserHookerps.dll: dlldata.obj BrowserHooker_p.obj BrowserHooker_i.obj
	link /dll /out:BrowserHookerps.dll /def:BrowserHookerps.def /entry:DllMain dlldata.obj BrowserHooker_p.obj BrowserHooker_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del BrowserHookerps.dll
	@del BrowserHookerps.lib
	@del BrowserHookerps.exp
	@del dlldata.obj
	@del BrowserHooker_p.obj
	@del BrowserHooker_i.obj
