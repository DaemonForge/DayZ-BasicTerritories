modded class PlayerBase extends ManBase {
	
	protected bool m_IsAllowedToBuildOrJoinTerritory;
	
	void PlayerBase(){
		RegisterNetSyncVariableBool("m_IsAllowedToBuildOrJoinTerritory");
	}
	
	bool IsAllowedToBuildOrJoinTerritory() {
		return m_IsAllowedToBuildOrJoinTerritory;
	}

	override void OnConnect(){
		super.OnConnect();
		SyncTerritoryInformation();
	}
	
	override void OnReconnect(){
		super.OnReconnect();
		SyncTerritoryInformation();
	}

	void SyncTerritoryInformation() {
		m_IsAllowedToBuildOrJoinTerritory = BasicTerritoriesData.GetInstance().IsAllowedToBuildOrJoinTerritory(GetIdentity().GetId());
		SetSynchDirty();
	}
}
