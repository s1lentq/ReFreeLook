#pragma once

bool OnAmxxAttach();
bool OnMetaAttach();
void OnMetaDetach();
CGameRules *InstallGameRules(IReGameHook_InstallGameRules *chain);
