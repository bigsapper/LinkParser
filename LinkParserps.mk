
LinkParserps.dll: dlldata.obj LinkParser_p.obj LinkParser_i.obj
	link /dll /out:LinkParserps.dll /def:LinkParserps.def /entry:DllMain dlldata.obj LinkParser_p.obj LinkParser_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del LinkParserps.dll
	@del LinkParserps.lib
	@del LinkParserps.exp
	@del dlldata.obj
	@del LinkParser_p.obj
	@del LinkParser_i.obj
