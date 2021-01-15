modded class MissionGameplay extends MissionBase
{	
	void MissionGameplay() {
		GetRPCManager().AddRPC( "BASICT", "RPCBasicTerritoriesModSettings", this, SingeplayerExecutionType.Both );
	}
	
	override void OnMissionStart(){
		super.OnMissionStart();
		Print("[BASICT][Client] Requesting Config From Server");
		GetRPCManager().SendRPC("BASICT", "RPCBasicTerritoriesModSettings", new Param1< BasicTerritoriesConfig >( NULL ), true, NULL);
	}
	
	
	void RPCBasicTerritoriesModSettings( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1< BasicTerritoriesConfig > data; 
		if ( !ctx.Read( data ) ) return;
		Print("[BASICT][Client] Received Config From Server");
		m_BasicTerritoriesConfig = data.param1;
		
		#ifdef BASICMAP
		if (GetBasicTerritoriesConfig().NoBuildZones){
			for (int i = 0; i < GetBasicTerritoriesConfig().NoBuildZones.Count(); i++){
				if (GetBasicTerritoriesConfig().NoBuildZones.Get(i) && GetBasicTerritoriesConfig().NoBuildZones.Get(i).DrawOnMap){
					BasicMapCircleMarker tmpMarker = new BasicMapCircleMarker("",  GetBasicTerritoriesConfig().NoBuildZones.Get(i).GetPos(), "BasicTerritories\\images\\NoBuild.paa", {189, 38, 78},150);
					tmpMarker.SetRadius(GetBasicTerritoriesConfig().NoBuildZones.Get(i).R);
					tmpMarker.SetShowCenterMarker(true);
					BasicMap().AddMarker("TerritoryNoBuildZones",tmpMarker);
				}
			}
		}
		#endif
	}
}