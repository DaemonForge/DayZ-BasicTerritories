modded class ActionDeployObject : ActionContinuousBase
{			 
	protected bool m_CanPlaceHere = true;
	protected bool m_RequestedSync = false;
	protected vector m_LastCheckLocation = vector.Zero;
	
	#ifdef BASICMAP
	protected ref BasicTerritoryMapMarker BASICT_Marker;
	#endif
	
	void ~ActionDeployObject() {
		RemoveTheBasicMapMarker();
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (super.ActionCondition( player, target, item )){
			if (GetGame().IsServer()){
				return true;
			}
			PlayerBase thePlayer = PlayerBase.Cast(player);
			
			if (thePlayer && thePlayer.GetHologramLocal() && thePlayer.GetHologramLocal().GetProjectionEntity()) {
				
				if (vector.Distance(m_LastCheckLocation, thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition()) > 0.4){
					#ifdef BASICMAP
					bool ShowTerritoryOnMap = BasicMap().ShowSelfOnMap();
					if (item && item.IsInherited(TerritoryFlagKit) && ShowTerritoryOnMap){
						if (!BASICT_Marker){
							BASICT_Marker = new ref BasicTerritoryMapMarker("Territory", thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition());
							BASICT_Marker.SetRadius(GameConstants.REFRESHER_RADIUS);
							BasicMap().AddMarker(BasicMap().CLIENT_KEY, BASICT_Marker);
						} else if ( BASICT_Marker.GetPosition() != thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition() ) {
							if (BASICT_Marker.GetPosition() == vector.Zero){
								BasicMap().AddMarker(BasicMap().CLIENT_KEY, BASICT_Marker);
							}
							BASICT_Marker.SetPosition(thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition());
							BASICT_Marker.SetRadius(GameConstants.REFRESHER_RADIUS);
						}
					} else {
						if (BASICT_Marker && BASICT_Marker.GetPosition() != vector.Zero && ShowTerritoryOnMap){
							BasicMap().RemoveMarker(BasicMap().CLIENT_KEY , BASICT_Marker);
							BASICT_Marker.SetPosition(vector.Zero);
							BASICT_Marker.SetRadius(0);
						}
					}
					#endif
					string theGUID = "";
					if (thePlayer.GetIdentity()) {
						theGUID = thePlayer.GetIdentity().GetId();
					}
					return CanIPlaceHere(item, thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition(), theGUID);
				} else {
					return m_CanPlaceHere;
				}
			} else {
				RemoveTheBasicMapMarker();
			}
			return true;
		} else {
			RemoveTheBasicMapMarker();
		}
		return false;
	}
	
	override void OnEndClient( ActionData action_data  )
	{
		super.OnEndClient( action_data );
	}
	
	void RemoveTheBasicMapMarker(){
	#ifdef BASICMAP
		if (GetGame().IsClient() && BASICT_Marker && BASICT_Marker.GetPosition() != vector.Zero ){
			BasicMap().RemoveMarker(BasicMap().CLIENT_KEY , BASICT_Marker);
			BASICT_Marker.SetPosition(vector.Zero);
			BASICT_Marker.SetRadius(0);
		}
	#endif
	}
	
	protected bool CanIPlaceHere(EntityAI item, vector pos, string GUID = ""){
		m_LastCheckLocation = pos;
		if (pos == vector.Zero || !item){
			m_CanPlaceHere = false;
			return m_CanPlaceHere;
		} else if ( GetBasicTerritoriesConfig().IsInWhiteList(item.GetType()) ){
			m_CanPlaceHere = true;
			return m_CanPlaceHere;
		} else {
			ref array<Object> objects = new array<Object>;
			ref array<CargoBase> proxyCargos = new array<CargoBase>;
			float theRadius = GameConstants.REFRESHER_RADIUS * 1.05;
			if ( item.IsInherited(TerritoryFlagKit) ){
				theRadius = GameConstants.REFRESHER_RADIUS * 2.05;
			}
			GetGame().GetObjectsAtPosition(pos, theRadius, objects, proxyCargos);
			TerritoryFlag theFlag;
			if (objects){
				for (int i = 0; i < objects.Count(); i++ ){
					if (Class.CastTo( theFlag, objects.Get(i) ) ){
						
						if ( item.IsInherited(TerritoryFlagKit) ){
							string WarningMessage = "Sorry you can't build a territory this close to another territory";
							if (GetBasicTerritoriesConfig().CanWarnPlayer(WarningMessage)){
								GetBasicTerritoriesConfig().SendNotification(WarningMessage);
							}
							#ifdef BASICMAP
							if (BASICT_Marker){
								BASICT_Marker.SetOverLaping(true);
							}
							#endif
							m_CanPlaceHere = false;
							return m_CanPlaceHere;
						}
						if (!m_RequestedSync){
							m_RequestedSync = true;
							theFlag.SyncTerritory();
						}
						m_CanPlaceHere = theFlag.CheckPlayerPermission(GUID, TerritoryPerm.DEPLOY);
						string DeployWarningMessage = "Sorry you can't build this close to an enemy territory";
						if (!m_CanPlaceHere && GetBasicTerritoriesConfig().CanWarnPlayer(DeployWarningMessage) ){
							GetBasicTerritoriesConfig().SendNotification(DeployWarningMessage);
						}
						return m_CanPlaceHere;
					}
				}
			} else {
			}
			if (item && item.IsInherited(TerritoryFlagKit) ){
				#ifdef BASICMAP
				if (BASICT_Marker){
					BASICT_Marker.SetOverLaping(false);
				}
				#endif
			}
		}
		string DeSpawnWarningMessage = "You are building outside a territory, Base Building objects will despawn in two weeks without a Territory";
		if (GetBasicTerritoriesConfig().CanWarnPlayer(DeSpawnWarningMessage) && item && !item.IsItemTent() && !item.IsInherited(TerritoryFlagKit)){
			GetBasicTerritoriesConfig().SendNotification(DeSpawnWarningMessage, "BasicTerritories/images/Build.paa");
		}
		m_CanPlaceHere = true;
		return true;
	}
};