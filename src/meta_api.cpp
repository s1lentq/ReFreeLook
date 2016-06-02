#include "precompiled.h"

meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;

plugin_info_t Plugin_info =
{
	META_INTERFACE_VERSION,			// ifvers
	"ReFreeLook",				// name
	"1.6",					// version
	__DATE__,				// date
	"s1lent",				// author
	"http://www.dedicated-server.ru",	// url
	"ReFreeLook",				// logtag, all caps please
	PT_ANYTIME,				// (when) loadable
	PT_NEVER,				// (when) unloadable
};

C_DLLEXPORT int Meta_Query(char *interfaceVersion, plugin_info_t* *plinfo, mutil_funcs_t *pMetaUtilFuncs)
{
	*plinfo = &Plugin_info;
	gpMetaUtilFuncs = pMetaUtilFuncs;
	return TRUE;
}

// Must provide at least one of these..
META_FUNCTIONS gMetaFunctionTable =
{
	NULL,			// pfnGetEntityAPI		HL SDK; called before game DLL
	NULL,			// pfnGetEntityAPI_Post		META; called after game DLL
	NULL,			// pfnGetEntityAPI2		HL SDK2; called before game DLL
	NULL,			// pfnGetEntityAPI2_Post	META; called after game DLL
	NULL,			// pfnGetNewDLLFunctions	HL SDK2; called before game DLL
	NULL,			// pfnGetNewDLLFunctions_Post	META; called after game DLL
	NULL,			// pfnGetEngineFunctions	META; called before HL engine
	NULL,			// pfnGetEngineFunctions_Post	META; called after HL engine
};

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs)
{
	gpMetaGlobals = pMGlobals;
	gpGamedllFuncs = pGamedllFuncs;

	if (!OnMetaAttach())
	{
		return FALSE;
	}

	memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
	return TRUE;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	OnMetaDetach();
	return TRUE;
}
