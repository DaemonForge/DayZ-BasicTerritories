modded class MissionServer extends MissionBase
{
	override void OnInit() {
		super.OnInit();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.OnTerritoryInit, 500, false); //Delay so CE is Initialized 
		GetRPCManager().AddRPC( "BASICT", "RPCBasicTerritoriesModSettings", this, SingeplayerExecutionType.Both );
	}
	
	void OnTerritoryInit(){
		Print("[BasicTerritories] OnInit");
		GetBasicTerritoriesConfig();
	}
	
	
	void RPCBasicTerritoriesModSettings( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target ) {
		PlayerIdentity RequestedBy = PlayerIdentity.Cast(sender);
		if (RequestedBy){
			GetRPCManager().SendRPC("BASICT", "RPCBasicTerritoriesModSettings", new Param1< BasicTerritoriesConfig >( GetBasicTerritoriesConfig() ), true, RequestedBy);
		}
	}
}


