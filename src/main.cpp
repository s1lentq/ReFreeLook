#include "precompiled.h"

cvar_t cv_admin_access_flags = { "afl_admin_access_flags", "d" };

cvar_t *pcv_admin_access_flags;
CGameRules *g_pGameRules;

inline bool hasFlags(const int nIndex) {
	return (UTIL_ReadFlags(pcv_admin_access_flags->string) & g_amxxapi.GetPlayerFlags(nIndex)) != 0;
}

int GetForceCamera(IReGameHook_GetForceCamera *chain, CBasePlayer *pObserver)
{
	int retVal = chain->callNext(pObserver);

	// When set to 1, player's screen fades to black for the remainder of the round when he dies
	// (hud still works normally so player can chat and see the scores, etc.)
	// Overrides mp_forcechasecam
	if (CSGameRules()->m_flFadeToBlackValue > 0.0f

		// 0 - You can spectate anyone
		// 1 - You can spectate teammates only + No Free Look
		// 2 - Only first person mode is allowed
		// 3 - You can spectate teammates only (Free Look works)
		|| CSGameRules()->m_flForceCameraValue <= 0.0f

		// 0 - You can spectate anyone
		// 1 - You can spectate teammates only + No Free Look
		// 2 - Only first person mode is allowed
		// 3 - You can spectate teammates only (Free Look works)
		|| CSGameRules()->m_flForceChaseCamValue <= 0.0f) {
		return retVal;
	}

	if (hasFlags(pObserver->entindex())) {
		retVal = CAMERA_MODE_SPEC_ANYONE;
	}

	return retVal;
}

CBasePlayer *CBasePlayer_Observer_IsValidTarget(IReGameHook_CBasePlayer_Observer_IsValidTarget *chain, CBasePlayer *pthis, int iPlayerIndex, bool bSameTeam)
{
	if (CSGameRules()->m_flFadeToBlackValue <= 0.0f
		&& CSGameRules()->m_flForceCameraValue > 0.0f
		&& CSGameRules()->m_flForceChaseCamValue > 0.0f)
	{
		// go next hook and call the original in the end
		return chain->callNext(pthis, iPlayerIndex, bSameTeam);
	}

	if (iPlayerIndex > gpGlobals->maxClients || iPlayerIndex < 1)
		return nullptr;

	CBasePlayer *pPlayer = g_ReGameFuncs->UTIL_PlayerByIndex(iPlayerIndex);

	// Don't spec observers or players who haven't picked a class yet
	if (!pPlayer || pPlayer == pthis || pPlayer->has_disconnected || pPlayer->IsObserver() || (pPlayer->pev->effects & EF_NODRAW) || pPlayer->m_iTeam == UNASSIGNED)
		return nullptr;

	if (bSameTeam && pPlayer->m_iTeam != pthis->m_iTeam && !hasFlags(pthis->entindex()))
		return nullptr;

	return pPlayer;
}

bool OnAmxxAttach()
{
	if (!RegamedllApi_Init())
		return false;

	CVAR_REGISTER(&cv_admin_access_flags);
	pcv_admin_access_flags = CVAR_GET_POINTER(cv_admin_access_flags.name);

	g_ReGameHookchains->InstallGameRules()->registerHook(&InstallGameRules);
	g_ReGameHookchains->CBasePlayer_Observer_IsValidTarget()->registerHook(&CBasePlayer_Observer_IsValidTarget);
	g_ReGameHookchains->GetForceCamera()->registerHook(&GetForceCamera);
	return true;
}

void OnMetaDetach()
{
	g_ReGameHookchains->InstallGameRules()->unregisterHook(&InstallGameRules);
	g_ReGameHookchains->CBasePlayer_Observer_IsValidTarget()->unregisterHook(&CBasePlayer_Observer_IsValidTarget);
	g_ReGameHookchains->GetForceCamera()->unregisterHook(&GetForceCamera);
}

CGameRules *InstallGameRules(IReGameHook_InstallGameRules *chain)
{
	return g_pGameRules = chain->callNext();
}
