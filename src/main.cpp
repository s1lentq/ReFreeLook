#include "precompiled.h"

cvar_t cv_admin_access_flags = { "afl_admin_access_flags", "d" };

cvar_t *pcv_fadetoblack;
cvar_t *pcv_forcecamera;
cvar_t *pcv_forcechasecam;
cvar_t *pcv_admin_access_flags;

inline bool ShouldRunCode()
{
	if (!pcv_fadetoblack || !pcv_forcecamera || !pcv_forcechasecam)
		return true;

	return pcv_fadetoblack->value <= 0.0f && pcv_forcecamera->value > 0.0f && pcv_forcechasecam->value > 0.0f;
}

inline bool canFreeLook(const int nIndex) {
	return (UTIL_ReadFlags(pcv_admin_access_flags->string) & g_amxxapi.GetPlayerFlags(nIndex));
}

int GetForceCamera(IReGameHook_GetForceCamera *chain, CBasePlayer *pObserver)
{
	int retVal = chain->callNext(pObserver);
	if (ShouldRunCode()) {
		if (canFreeLook(pObserver->entindex()))
			retVal = CAMERA_MODE_SPEC_ANYONE;
	}

	return retVal;
}

CBasePlayer *CBasePlayer_Observer_IsValidTarget(IReGameHook_CBasePlayer_Observer_IsValidTarget *chain, CBasePlayer *pthis, int iPlayerIndex, bool bSameTeam)
{
	if (ShouldRunCode()) {
		// go next hook and call the original in the end
		return chain->callNext(iPlayerIndex, bSameTeam);
	}

	if (iPlayerIndex > gpGlobals->maxClients || iPlayerIndex < 1)
		return nullptr;

	CBasePlayer *pPlayer = g_ReGameFuncs->UTIL_PlayerByIndex(iPlayerIndex);

	// Don't spec observers or players who haven't picked a class yet
	if (!pPlayer || pPlayer == pthis || pPlayer->has_disconnected || pPlayer->IsObserver() || (pPlayer->pev->effects & EF_NODRAW) || pPlayer->m_iTeam == UNASSIGNED)
		return nullptr;

	if (bSameTeam && pPlayer->m_iTeam != pthis->m_iTeam && !canFreeLook(pthis->entindex()))
		return nullptr;

	return pPlayer;
}

bool OnAmxxAttach()
{
	if (!RegamedllApi_Init())
		return false;

	CVAR_REGISTER(&cv_admin_access_flags);
	pcv_admin_access_flags = CVAR_GET_POINTER(cv_admin_access_flags.name);

	g_ReGameApi->GetHookchains()->CBasePlayer_Observer_IsValidTarget()->registerHook(&CBasePlayer_Observer_IsValidTarget);
	g_ReGameApi->GetHookchains()->GetForceCamera()->registerHook(&GetForceCamera);
	return true;
}

bool OnMetaAttach()
{
	return true;
}

void OnMetaDetach()
{
	g_ReGameApi->GetHookchains()->CBasePlayer_Observer_IsValidTarget()->unregisterHook(&CBasePlayer_Observer_IsValidTarget);
	g_ReGameApi->GetHookchains()->GetForceCamera()->unregisterHook(&GetForceCamera);
}

void OnPluginsLoaded()
{
	// When set to 1, player's screen fades to black for the remainder of the round when he dies
	// (hud still works normally so player can chat and see the scores, etc.)
	// Overrides mp_forcechasecam
	pcv_fadetoblack = CVAR_GET_POINTER("mp_fadetoblack");

	// 0 - You can spectate anyone
	// 1 - You can spectate teammates only + No Free Look
	// 2 - Only first person mode is allowed
	// 3 - You can spectate teammates only (Free Look works)
	pcv_forcecamera = CVAR_GET_POINTER("mp_forcecamera");

	// 0 - You can spectate anyone
	// 1 - You can spectate teammates only + No Free Look
	// 2 - Only first person mode is allowed
	// 3 - You can spectate teammates only (Free Look works)
	pcv_forcechasecam = CVAR_GET_POINTER("mp_forcechasecam");
}
